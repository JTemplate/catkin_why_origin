#include <deque>
#include <memory>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/register_point_struct.h>
#include <pcl_conversions/pcl_conversions.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include <tf2/exceptions.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

#include <tf2_sensor_msgs/tf2_sensor_msgs.hpp>



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

    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(get_clock());
    tf_listener_ =
        std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);


    publisher_ = create_publisher<sensor_msgs::msg::PointCloud2>(
        "/mmwaveFiltered", rclcpp::SensorDataQoS());

    subscription_ = create_subscription<sensor_msgs::msg::PointCloud2>(
        "/ti_mmwave/radar_scan_pcl_0",
        rclcpp::SensorDataQoS(),
        [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) {
          process(msg);
        });
}


private:
  void process(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg) {
    RCLCPP_INFO_ONCE(
      get_logger(),
      "Radar input frame_id='%s'",
      msg->header.frame_id.c_str());

    sensor_msgs::msg::PointCloud2 transformed_msg;

    try {
      const auto transform = tf_buffer_->lookupTransform(
        "velodyne",
        msg->header.frame_id,
        tf2::TimePointZero);

      RCLCPP_INFO_ONCE(
        get_logger(),
        "Using TF '%s' -> 'velodyne': "
        "translation=(%.3f, %.3f, %.3f)",
        msg->header.frame_id.c_str(),
        transform.transform.translation.x,
        transform.transform.translation.y,
        transform.transform.translation.z);

      tf2::doTransform(*msg, transformed_msg, transform);

      // Preserve the original measurement timestamp.
      transformed_msg.header.stamp = msg->header.stamp;
      transformed_msg.header.frame_id = "velodyne";
    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        5000,
        "Cannot transform Radar cloud from '%s' to 'velodyne': %s",
        msg->header.frame_id.c_str(),
        ex.what());

      return;
    }

    RCLCPP_DEBUG(get_logger(), "Received point cloud");

    auto current = std::make_shared<pcl::PointCloud<PointXYZIV>>();
    pcl::fromROSMsg(transformed_msg, *current);

    auto filtered = std::make_shared<pcl::PointCloud<PointXYZIV>>();

    for (auto point : current->points) {
      if (point.intensity >= 15.0F) {
        point.z = 0.0F;
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

    output.header = transformed_msg.header;

    publisher_->publish(output);
}


std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

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
