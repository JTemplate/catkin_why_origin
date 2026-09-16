#include <cmath>
#include <map>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <ros_plc/msg/lidar_msg.hpp>
#include <ros_plc/msg/lidar_msg_array.hpp>

struct LidarObjectInfo {
  float x;
  float y;
  float z;
  float speed;
  int original_id;
  int current_id;
  rclcpp::Time timestamp;
};

class LidarObjectProcessor : public rclcpp::Node {
public:
  LidarObjectProcessor()
  : Node("lidar_object_processor_node")
  {
    publisher_ =
      create_publisher<ros_plc::msg::LidarMsgArray>(
        "/lidar_objects", 10);

    subscription_ =
      create_subscription<ros_plc::msg::LidarMsgArray>(
        "/lidar_perception/tracked_objects",
        10,
        [this](ros_plc::msg::LidarMsgArray::ConstSharedPtr msg) {
          process(msg);
        });
  }

private:
  void process(
    const ros_plc::msg::LidarMsgArray::ConstSharedPtr & input)
  {
    // The old data path only produced a data message when there were
    // tracked object markers. Preserve that behavior here.
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
          "Ignoring LiDAR tracked object %d with invalid timestamp",
          id);
        continue;
      }

      update(
        id,
        object.x,
        object.y,
        object.z,
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
    float z,
    float speed,
    const rclcpp::Time & timestamp)
  {
    // Preserve the existing second-stage ID repair behavior.
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
      closest->z = z;
      closest->speed = speed;
      closest->timestamp = timestamp;
    }
    else {
      objects_.emplace(
        new_id,
        LidarObjectInfo{
          x,
          y,
          z,
          speed,
          new_id,
          new_id,
          timestamp});
    }
  }

  void publish(const rclcpp::Time & timestamp)
  {
    ros_plc::msg::LidarMsgArray array;

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

  void cleanup(const rclcpp::Time & latest)
  {
    for (auto it = objects_.begin(); it != objects_.end();) {
      if (
        (latest - it->second.timestamp).seconds() >
        2.0)
      {
        RCLCPP_INFO(
          get_logger(),
          "Removing inactive LiDAR object %d",
          it->second.original_id);

        it = objects_.erase(it);
      }
      else {
        ++it;
      }
    }
  }

  rclcpp::Subscription<
    ros_plc::msg::LidarMsgArray>::SharedPtr
    subscription_;

  rclcpp::Publisher<
    ros_plc::msg::LidarMsgArray>::SharedPtr
    publisher_;

  std::map<int, LidarObjectInfo> objects_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  rclcpp::spin(
    std::make_shared<LidarObjectProcessor>());

  rclcpp::shutdown();

  return 0;
}
