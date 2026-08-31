#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "lidar_perception.h"

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("lidar_perception_node");
  auto perception = std::make_unique<lidar_perception::lidar_perception>(node);
  rclcpp::spin(node);
  perception.reset();
  rclcpp::shutdown();
  return 0;
}
