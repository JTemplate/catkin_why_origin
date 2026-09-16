#include <cmath>
#include <map>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <ros_plc/msg/radar_msg.hpp>
#include <ros_plc/msg/radar_msg_array.hpp>

struct RadarObjectInfo {
  float x;
  float y;
  float speed;
  int original_id;
  int current_id;
  rclcpp::Time timestamp;
};

class RadarObjectProcessor : public rclcpp::Node {
public:
  RadarObjectProcessor()
  : Node("radar_object_processor_node")
  {
    publisher_ =
      create_publisher<ros_plc::msg::RadarMsgArray>(
        "/radar_objects", 10);

    subscription_ =
      create_subscription<ros_plc::msg::RadarMsgArray>(
        "/radar_perception/tracked_objects",
        10,
        [this](ros_plc::msg::RadarMsgArray::ConstSharedPtr msg) {
          process(msg);
        });

    // Deliberately no wall-clock stale timer:
    // bag sensor time must not be mixed with wall-clock time.
  }

private:
  void process(
    const ros_plc::msg::RadarMsgArray::ConstSharedPtr & input)
  {
    if (input->objects.empty()) {
      return;
    }

    rclcpp::Time latest(input->header.stamp, RCL_ROS_TIME);
    bool has_object = false;

    for (const auto & object : input->objects) {
      const int id = object.current_id;
      if (id < 0) {
        continue;
      }

      rclcpp::Time timestamp(object.timestamp, RCL_ROS_TIME);
      if (timestamp.nanoseconds() <= 0) {
        timestamp = rclcpp::Time(input->header.stamp, RCL_ROS_TIME);
      }

      if (timestamp.nanoseconds() <= 0) {
        RCLCPP_WARN(
          get_logger(),
          "Ignoring Radar tracked object %d with invalid timestamp",
          id);
        continue;
      }

      update(
        id,
        object.x,
        object.y,
        object.speed,
        timestamp);

      if (timestamp > latest) {
        latest = timestamp;
      }

      has_object = true;
    }

    if (!has_object || latest.nanoseconds() <= 0) {
      return;
    }

    cleanup(latest);
    publish(latest);
  }

  void update(
    int new_id,
    float x,
    float y,
    float speed,
    const rclcpp::Time & timestamp)
  {
    // Preserve the existing second-stage ID repair behavior.
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

      const float distance =
        std::hypot(
          info.x - x,
          info.y - y);

      if (distance < minimum) {
        minimum = distance;
        closest = &info;
      }
    }

    if (closest != nullptr) {
      closest->current_id = new_id;
      closest->x = x;
      closest->y = y;
      closest->speed = speed;
      closest->timestamp = timestamp;
    }
    else {
      objects_.emplace(
        new_id,
        RadarObjectInfo{
          x,
          y,
          speed,
          new_id,
          new_id,
          timestamp});
    }
  }

  void publish(const rclcpp::Time & timestamp)
  {
    ros_plc::msg::RadarMsgArray array;

    array.header.stamp = timestamp;
    array.header.frame_id = "velodyne";

    for (const auto & [unused_id, info] : objects_) {
      (void)unused_id;

      if (
        std::abs(
          (info.timestamp - timestamp).seconds()) >
        1.0)
      {
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

  void cleanup(const rclcpp::Time & latest)
  {
    for (auto it = objects_.begin(); it != objects_.end();) {
      if (
        (latest - it->second.timestamp).seconds() >
        2.0)
      {
        RCLCPP_INFO(
          get_logger(),
          "Removing inactive Radar object %d",
          it->second.original_id);

        it = objects_.erase(it);
      }
      else {
        ++it;
      }
    }
  }

  rclcpp::Subscription<
    ros_plc::msg::RadarMsgArray>::SharedPtr
    subscription_;

  rclcpp::Publisher<
    ros_plc::msg::RadarMsgArray>::SharedPtr
    publisher_;

  std::map<int, RadarObjectInfo> objects_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  rclcpp::spin(
    std::make_shared<RadarObjectProcessor>());

  rclcpp::shutdown();

  return 0;
}
