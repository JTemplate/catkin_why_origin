#include <deque>
#include <memory>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/register_point_struct.h>
#include <pcl_conversions/pcl_conversions.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

struct EIGEN_ALIGN16 PointXYZIV {
  PCL_ADD_POINT4D;
  float intensity;
  float velocity;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIV,
  (float, x, x)(float, y, y)(float, z, z)
  (float, intensity, intensity)(float, velocity, velocity))

class PointCloudAssembler : public rclcpp::Node {
public:
  PointCloudAssembler() : Node("pointcloud_assembler_node") {
    publisher_ = create_publisher<sensor_msgs::msg::PointCloud2>(
      "/mmwaveFiltered", rclcpp::SensorDataQoS());
    subscription_ = create_subscription<sensor_msgs::msg::PointCloud2>(
      "/ti_mmwave/radar_scan_pcl_0", rclcpp::SensorDataQoS(),
      [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) { process(msg); });
  }

private:
  void process(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg) {
    RCLCPP_DEBUG(get_logger(), "Received point cloud");
    auto current = std::make_shared<pcl::PointCloud<PointXYZIV>>();
    pcl::fromROSMsg(*msg, *current);
    auto filtered = std::make_shared<pcl::PointCloud<PointXYZIV>>();
    for (auto point : current->points) {
      if (point.intensity >= 15.0F) {
        point.z = 0.0F;
        point.x += 0.26F;
        point.y += 0.06F;
        filtered->push_back(point);
      }
    }
    filtered->height = 1;
    filtered->is_dense = true;
    cloud_queue_.push_back(filtered);
    if (cloud_queue_.size() > 10) {
      cloud_queue_.pop_front();
    }
    pcl::PointCloud<PointXYZIV> assembled;
    for (const auto & cloud : cloud_queue_) {
      assembled += *cloud;
    }
    sensor_msgs::msg::PointCloud2 output;
    pcl::toROSMsg(assembled, output);
    output.header = msg->header;
    publisher_->publish(output);
  }

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr publisher_;
  std::deque<pcl::PointCloud<PointXYZIV>::Ptr> cloud_queue_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PointCloudAssembler>());
  rclcpp::shutdown();
  return 0;
}
