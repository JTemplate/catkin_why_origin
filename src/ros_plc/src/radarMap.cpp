#include <chrono>
#include <cmath>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <ros_plc/msg/radar_msg.hpp>
#include <ros_plc/msg/radar_msg_array.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

using namespace std::chrono_literals;

struct RadarObjectInfo {
  float x;
  float y;
  float speed;
  int original_id;
  int current_id;
  rclcpp::Time timestamp;
};

class RadarMarkerProcessor : public rclcpp::Node {
public:
  RadarMarkerProcessor()
  : Node("radar_marker_processor_node"), last_message_time_(get_clock()->now()) {
    publisher_ = create_publisher<ros_plc::msg::RadarMsgArray>("/radar_objects", 10);
    subscription_ = create_subscription<visualization_msgs::msg::MarkerArray>(
      "/radar_perception/marker_info", 10,
      [this](visualization_msgs::msg::MarkerArray::ConstSharedPtr msg) { process(msg); });
    timer_ = create_wall_timer(500ms, [this]() { publish_empty_if_stale(); });
  }

private:
  void process(const visualization_msgs::msg::MarkerArray::ConstSharedPtr & marker_array) {
    if (marker_array->markers.empty()) {
      RCLCPP_WARN(get_logger(), "Received empty MarkerArray");
      return;
    }
    rclcpp::Time latest(0, 0, RCL_ROS_TIME);
    for (const auto & marker : marker_array->markers) {
      if (marker.action == visualization_msgs::msg::Marker::DELETE) {
        continue;
      }
      int id = -1;
      float speed = 0.0F;
      std::istringstream input(marker.text);
      std::string line;
      while (std::getline(input, line)) {
        if (line.find("id: ") != std::string::npos) {
          id = std::stoi(line.substr(line.find(": ") + 2));
        } else if (line.find("speed: ") != std::string::npos) {
          speed = std::stof(line.substr(line.find(": ") + 2));
        }
      }
      if (id < 0) {
        continue;
      }
      const rclcpp::Time timestamp(marker.header.stamp);
      update(id, marker.pose.position.x, marker.pose.position.y, speed, timestamp);
      if (timestamp > latest) {
        latest = timestamp;
      }
    }
    publish(latest);
    cleanup(latest);
    last_message_time_ = get_clock()->now();
  }

  void publish_empty_if_stale() {
    const auto current = get_clock()->now();
    if ((current - last_message_time_).seconds() <= 0.5) {
      return;
    }
    ros_plc::msg::RadarMsgArray empty;
    empty.header.stamp = current;
    empty.header.frame_id = "velodyne";
    publisher_->publish(empty);
    last_message_time_ = current;
  }

  void update(int new_id, float x, float y, float speed,
      const rclcpp::Time & timestamp) {
    for (auto & [unused_id, info] : objects_) {
      (void)unused_id;
      if (info.current_id == new_id) {
        info.x = x;
        info.y = y;
        info.speed = speed;
        info.timestamp = timestamp;
        return;
      }
    }
    constexpr float threshold = 1.0F;
    float minimum = threshold;
    RadarObjectInfo * closest = nullptr;
    for (auto & [unused_id, info] : objects_) {
      (void)unused_id;
      const float distance = std::hypot(info.x - x, info.y - y);
      if (distance < minimum) {
        minimum = distance;
        closest = &info;
      }
    }
    if (closest) {
      closest->current_id = new_id;
      closest->x = x;
      closest->y = y;
      closest->speed = speed;
      closest->timestamp = timestamp;
    } else {
      objects_.emplace(new_id, RadarObjectInfo{x, y, speed, new_id, new_id, timestamp});
    }
  }

  void publish(const rclcpp::Time & timestamp) {
    ros_plc::msg::RadarMsgArray array;
    array.header.stamp = timestamp;
    array.header.frame_id = "velodyne";
    for (const auto & [unused_id, info] : objects_) {
      (void)unused_id;
      if (std::abs((info.timestamp - timestamp).seconds()) > 1.0) {
        continue;
      }
      ros_plc::msg::RadarMsg object;
      object.current_id = info.current_id;
      object.original_id = info.original_id;
      object.x = info.x;
      object.y = info.y;
      object.speed = info.speed;
      object.phi = std::atan2(info.y, info.x);
      object.timestamp = info.timestamp;
      array.objects.push_back(object);
    }
    publisher_->publish(array);
  }

  void cleanup(const rclcpp::Time & latest) {
    for (auto it = objects_.begin(); it != objects_.end();) {
      if ((latest - it->second.timestamp).seconds() > 2.0) {
        RCLCPP_INFO(get_logger(), "Removing inactive radar object %d",
          it->second.original_id);
        it = objects_.erase(it);
      } else {
        ++it;
      }
    }
  }

  rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr subscription_;
  rclcpp::Publisher<ros_plc::msg::RadarMsgArray>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time last_message_time_;
  std::map<int, RadarObjectInfo> objects_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RadarMarkerProcessor>());
  rclcpp::shutdown();
  return 0;
}
