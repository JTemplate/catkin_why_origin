#include <memory>

#include <pcl/filters/passthrough.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

class GroundCutNode : public rclcpp::Node {
public:
  GroundCutNode() : Node("ground_removal") {
    publisher_ = create_publisher<sensor_msgs::msg::PointCloud2>(
      "/ground_cut_output_cloud_topic", rclcpp::SensorDataQoS());
    subscription_ = create_subscription<sensor_msgs::msg::PointCloud2>(
      "/rslidar_points", rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) { process(msg); });
  }

private:
  void process(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg) {
    auto cloud = std::make_shared<pcl::PointCloud<pcl::PointXYZI>>();
    pcl::fromROSMsg(*msg, *cloud);
    pcl::PassThrough<pcl::PointXYZI> pass;
    pass.setInputCloud(cloud);
    pass.setFilterFieldName("z");
    pass.setFilterLimits(-0.2, 2.0);
    pcl::PointCloud<pcl::PointXYZI> filtered;
    pass.filter(filtered);
    sensor_msgs::msg::PointCloud2 output;
    pcl::toROSMsg(filtered, output);
    output.header = msg->header;
    publisher_->publish(output);
  }

  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GroundCutNode>());
  rclcpp::shutdown();
  return 0;
}
