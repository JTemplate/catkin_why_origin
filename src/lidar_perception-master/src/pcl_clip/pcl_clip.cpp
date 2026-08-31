#include "pcl_clip.h"

namespace pcl_clip
{
Pcl_clip::Pcl_clip(const rclcpp::Node::SharedPtr & node)
{  
    roi_x_min_ = node->declare_parameter("roi_x_min", 0.0);
    roi_x_max_ = node->declare_parameter("roi_x_max", 10.0);
    roi_y_min_ = node->declare_parameter("roi_y_min", -5.0);
    roi_y_max_ = node->declare_parameter("roi_y_max", 5.0);
    roi_z_min_ = node->declare_parameter("roi_z_min", -1.0);
    roi_z_max_ = node->declare_parameter("roi_z_max", 10.0);
    vehicle_x_min_ = node->declare_parameter("vehicle_x_min", -1.2);
    vehicle_x_max_ = node->declare_parameter("vehicle_x_max", 3.0);
    vehicle_y_min_ = node->declare_parameter("vehicle_y_min", -1.0);
    vehicle_y_max_ = node->declare_parameter("vehicle_y_max", 1.0);
    vehicle_z_min_ = node->declare_parameter("vehicle_z_min", -0.5);
    vehicle_z_max_ = node->declare_parameter("vehicle_z_max", 2.2);
}
VPointCloud::Ptr Pcl_clip::GetPcl(const VPointCloud::ConstPtr in)
{
    if(in->points.size() > 0)
    {
        VPointCloud::Ptr out(new VPointCloud);
        for (const VPoint &p : in->points)
        {
            if (IsIn(p.x, roi_x_min_, roi_x_max_) && IsIn(p.y, roi_y_min_, roi_y_max_) && IsIn(p.z, roi_z_min_, roi_z_max_))
            out->push_back(p);
        }
        return out;
    }
    return nullptr; 
}
VPointCloud::Ptr Pcl_clip::Clip_vehicle(const VPointCloud::ConstPtr in){
    if(in->points.size() > 0)
    {
        VPointCloud::Ptr out(new VPointCloud);
        for (const VPoint &p : in->points)
        {
            if (IsIn(p.x, vehicle_x_min_, vehicle_x_max_) && IsIn(p.y, vehicle_y_min_, vehicle_y_max_) && IsIn(p.z, vehicle_z_min_, vehicle_z_max_)){
                
            }
            else out->push_back(p);
        }
        return out;
    }
    return nullptr; 
}
}//namespace remove_ground
