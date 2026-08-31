#include <memory>

#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

class VoxelGridFilterNode : public rclcpp::Node {
public:
  VoxelGridFilterNode() : Node("voxel_grid_filter_node") {
    publisher_ = create_publisher<sensor_msgs::msg::PointCloud2>(
      "output_cloud", rclcpp::SensorDataQoS());
    subscription_ = create_subscription<sensor_msgs::msg::PointCloud2>(
      "/y_cut_output_cloud_topic", rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) { process(msg); });
  }

private:
  void process(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg) {
    auto cloud = std::make_shared<pcl::PointCloud<pcl::PointXYZI>>();
    pcl::fromROSMsg(*msg, *cloud);
    pcl::VoxelGrid<pcl::PointXYZI> voxel;
    voxel.setInputCloud(cloud);
    voxel.setLeafSize(0.1F, 0.1F, 0.1F);
    auto downsampled = std::make_shared<pcl::PointCloud<pcl::PointXYZI>>();
    voxel.filter(*downsampled);
    pcl::StatisticalOutlierRemoval<pcl::PointXYZI> outlier_filter;
    outlier_filter.setInputCloud(downsampled);
    outlier_filter.setMeanK(50);
    outlier_filter.setStddevMulThresh(1.0);
    pcl::PointCloud<pcl::PointXYZI> filtered;
    outlier_filter.filter(filtered);
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
  rclcpp::spin(std::make_shared<VoxelGridFilterNode>());
  rclcpp::shutdown();
  return 0;
}
