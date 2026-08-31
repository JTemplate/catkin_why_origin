#ifndef OBSTACLE_DETECTION_IFM_PCL_CLIP_H
#define OBSTACLE_DETECTION_IFM_PCL_CLIP_H

#include <rclcpp/rclcpp.hpp>
#include "../types.h"

namespace pcl_clip
{
class Pcl_clip
{
public:
  explicit Pcl_clip(const rclcpp::Node::SharedPtr & node);
  ~Pcl_clip(){};
  VPointCloud::Ptr GetPcl(const VPointCloud::ConstPtr in);
  VPointCloud::Ptr Clip_vehicle(const VPointCloud::ConstPtr in);
  
private:
  double roi_x_min_;
  double roi_x_max_;
  double roi_y_min_;
  double roi_y_max_;
  double roi_z_min_;
  double roi_z_max_;
  double vehicle_x_min_;
  double vehicle_x_max_;
  double vehicle_y_min_;
  double vehicle_y_max_;
  double vehicle_z_min_;
  double vehicle_z_max_;
  bool IsIn(const float x, const float x_min, const float x_max) {
    return (x < x_max) && (x > x_min);
  }


};
}//namespace pcl_clip
#endif
