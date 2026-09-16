#ifndef LIDAR_PERCEPTION_LIDAR_PERCEPTION_H_
#define LIDAR_PERCEPTION_LIDAR_PERCEPTION_H_

#include <memory>
#include "ros_plc/msg/lidar_msg_array.hpp"
#include "ros_plc/msg/radar_msg_array.hpp"


#include <builtin_interfaces/msg/time.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

#include "types.h"
#include "pcl_clip/pcl_clip.h"
#include "remove_ground/remove_ground.h"
#include "cluster/EuclideanCluster.h"
#include "min_box/min_box.h"
#include "hm_tracker/hm_tracker.h"
#include "lidar_perception/object.h"

namespace lidar_perception {
class lidar_perception {
public:
  explicit lidar_perception(const rclcpp::Node::SharedPtr & node);
  ~lidar_perception() = default;

private:
  void PclCallback(sensor_msgs::msg::PointCloud2::ConstSharedPtr rec);
  void PclCallback_radar(sensor_msgs::msg::PointCloud2::ConstSharedPtr rec);
  void ClearAllMarker(const builtin_interfaces::msg::Time & timestamp);
  void ClearAllMarker_radar(const builtin_interfaces::msg::Time & timestamp);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_subscriber_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_subscriber_radar;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_box_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_tracinfo_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_track_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_box_radar;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_tracinfo_radar;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_track_radar;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_pcl_cluster_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_pcl_cluster_radar;
  rclcpp::Publisher<
    ros_plc::msg::LidarMsgArray>::SharedPtr
    lidar_tracked_objects_pub_;

  rclcpp::Publisher<
    ros_plc::msg::RadarMsgArray>::SharedPtr
    radar_tracked_objects_pub_;

  int max_marker_size_ = 0;
  int max_marker_size_radar = 0;
  int picture_num_ = 0;

  pcl_clip::Pcl_clip clip_;
  remove_ground::Remove_ground rm_;
  cluster::euclidean_cluster cluster_;
  std::unique_ptr<apollo::perception::MinBoxObjectBuilder> object_builder_;
  std::unique_ptr<apollo::perception::HmObjectTracker> tracker_;
  std::unique_ptr<apollo::perception::MinBoxObjectBuilder> object_builder_radar;
  std::unique_ptr<apollo::perception::HmObjectTracker> tracker_radar;
  double timestamp_ = 0.0;
  double timestamp_radar = 0.0;
  apollo::perception::SeqId seq_num_ = 0;
  apollo::perception::SeqId seq_num_radar = 0;
};
}  // namespace lidar_perception
#endif
