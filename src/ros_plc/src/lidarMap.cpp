#include <cmath>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <ros_plc/msg/lidar_msg.hpp>
#include <ros_plc/msg/lidar_msg_array.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

struct LidarObjectInfo {
  float x;
  float y;
  float z;
  float speed;
  int original_id;
  int current_id;
  rclcpp::Time timestamp;
};

class LidarMarkerProcessor : public rclcpp::Node {
public:
  LidarMarkerProcessor() : Node("lidar_marker_processor_node") {
    publisher_ = create_publisher<ros_plc::msg::LidarMsgArray>("/lidar_objects", 10);
    subscription_ = create_subscription<visualization_msgs::msg::MarkerArray>(
      "/lidar_perception/marker_info", 10,
      [this](visualization_msgs::msg::MarkerArray::ConstSharedPtr msg) { process(msg); });
  }

private:
  void process(const visualization_msgs::msg::MarkerArray::ConstSharedPtr & marker_array) {
    if (marker_array->markers.empty()) {
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
      update(id, marker.pose.position.x, marker.pose.position.y,
        marker.pose.position.z, speed, timestamp);
      if (timestamp > latest) {
        latest = timestamp;
      }
    }
    cleanup(latest);
    publish(latest);
  }

  void update(int new_id, float x, float y, float z, float speed,
      const rclcpp::Time & timestamp) {
    for (auto & [unused_id, info] : objects_) {
      (void)unused_id;
      if (info.current_id == new_id) {
        info.x = x;
        info.y = y;
        info.z = z;
        info.speed = speed;
        info.timestamp = timestamp;
        return;
      }
    }
    constexpr float threshold = 1.0F;
    float minimum = threshold;
    LidarObjectInfo * closest = nullptr;
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
      closest->z = z;
      closest->speed = speed;
      closest->timestamp = timestamp;
    } else {
      objects_.emplace(new_id, LidarObjectInfo{x, y, z, speed, new_id, new_id, timestamp});
    }
  }

  void publish(const rclcpp::Time & timestamp) {
    ros_plc::msg::LidarMsgArray array;
    array.header.stamp = timestamp;
    array.header.frame_id = "velodyne";
    for (const auto & [unused_id, info] : objects_) {
      (void)unused_id;
      if (std::abs((info.timestamp - timestamp).seconds()) > 1.0) {
        continue;
      }
      ros_plc::msg::LidarMsg object;
      object.current_id = info.current_id;
      object.original_id = info.original_id;
      object.x = info.x;
      object.y = info.y;
      object.z = info.z;
      object.speed = info.speed;
      object.timestamp = info.timestamp;
      array.objects.push_back(object);
    }
    publisher_->publish(array);
  }

  void cleanup(const rclcpp::Time & latest) {
    for (auto it = objects_.begin(); it != objects_.end();) {
      if ((latest - it->second.timestamp).seconds() > 2.0) {
        RCLCPP_INFO(get_logger(), "Removing inactive LiDAR object %d",
          it->second.original_id);
        it = objects_.erase(it);
      } else {
        ++it;
      }
    }
  }

  rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr subscription_;
  rclcpp::Publisher<ros_plc::msg::LidarMsgArray>::SharedPtr publisher_;
  std::map<int, LidarObjectInfo> objects_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LidarMarkerProcessor>());
  rclcpp::shutdown();
  return 0;
}
