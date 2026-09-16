#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>

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


struct EIGEN_ALIGN16 PointXYZIV
{
  PCL_ADD_POINT4D;
  float intensity;
  float velocity;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

POINT_CLOUD_REGISTER_POINT_STRUCT(
  PointXYZIV,
  (float, x, x)
  (float, y, y)
  (float, z, z)
  (float, intensity, intensity)
  (float, velocity, velocity)
)


// Output point type.
// time_offset is relative to the newest Radar frame:
//   0.0  = current frame
//   <0.0 = historical frame
//
// Downstream code that only consumes x/y/z/intensity/velocity can ignore
// this extra field, while the temporal origin of accumulated points remains
// observable in the PointCloud2 message.
struct EIGEN_ALIGN16 PointXYZIVT
{
  PCL_ADD_POINT4D;
  float intensity;
  float velocity;
  float time_offset;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

POINT_CLOUD_REGISTER_POINT_STRUCT(
  PointXYZIVT,
  (float, x, x)
  (float, y, y)
  (float, z, z)
  (float, intensity, intensity)
  (float, velocity, velocity)
  (float, time_offset, time_offset)
)


struct RadarFrame
{
  rclcpp::Time stamp;
  pcl::PointCloud<PointXYZIV>::Ptr cloud;

  // Radar sensor origin expressed in target_frame_ for this frame.
  // Doppler velocity is radial with respect to this origin, not with
  // respect to the target frame origin (e.g. velodyne).
  double radar_origin_x = 0.0;
  double radar_origin_y = 0.0;
};


class PointCloudAssembler : public rclcpp::Node
{
public:
  PointCloudAssembler()
  : Node("pointcloud_assembler_node")
  {
    target_frame_ =
      declare_parameter<std::string>(
        "target_frame",
        "velodyne");

    intensity_min_ =
      declare_parameter<double>(
        "filter.intensity_min",
        15.0);

    project_to_xy_ =
      declare_parameter<bool>(
        "filter.project_to_xy",
        true);

    accumulation_enabled_ =
      declare_parameter<bool>(
        "accumulation.enabled",
        true);

    accumulation_window_s_ =
      declare_parameter<double>(
        "accumulation.window_s",
        0.25);

    accumulation_max_frames_ =
      declare_parameter<int>(
        "accumulation.max_frames",
        4);

    doppler_compensation_enabled_ =
      declare_parameter<bool>(
        "motion_compensation.doppler_enabled",
        true);

    if (accumulation_window_s_ < 0.0) {
      RCLCPP_WARN(
        get_logger(),
        "accumulation.window_s must be >= 0; clamping to 0");
      accumulation_window_s_ = 0.0;
    }

    if (accumulation_max_frames_ < 1) {
      RCLCPP_WARN(
        get_logger(),
        "accumulation.max_frames must be >= 1; clamping to 1");
      accumulation_max_frames_ = 1;
    }

    tf_buffer_ =
      std::make_unique<tf2_ros::Buffer>(
        get_clock());

    tf_listener_ =
      std::make_shared<tf2_ros::TransformListener>(
        *tf_buffer_);

    publisher_ =
      create_publisher<sensor_msgs::msg::PointCloud2>(
        "/mmwaveFiltered",
        rclcpp::SensorDataQoS());

    subscription_ =
      create_subscription<sensor_msgs::msg::PointCloud2>(
        "/ti_mmwave/radar_scan_pcl_0",
        rclcpp::SensorDataQoS(),
        [this](
          sensor_msgs::msg::PointCloud2::ConstSharedPtr msg)
        {
          process(msg);
        });

    RCLCPP_INFO(
      get_logger(),
      "Radar filter configured: target_frame='%s', "
      "intensity_min=%.1f, project_to_xy=%s, "
      "accumulation.enabled=%s, window=%.3f s, max_frames=%d, "
      "doppler_compensation=%s",
      target_frame_.c_str(),
      intensity_min_,
      project_to_xy_ ? "true" : "false",
      accumulation_enabled_ ? "true" : "false",
      accumulation_window_s_,
      accumulation_max_frames_,
      doppler_compensation_enabled_ ? "true" : "false");
  }


private:
  static std::string normalizeFrameId(
    std::string frame_id)
  {
    while (!frame_id.empty() &&
           frame_id.front() == '/')
    {
      frame_id.erase(
        frame_id.begin());
    }

    return frame_id;
  }


  void clearAccumulation(
    const char * reason)
  {
    if (!cloud_queue_.empty()) {
      RCLCPP_WARN(
        get_logger(),
        "Clearing %zu accumulated Radar frame(s): %s",
        cloud_queue_.size(),
        reason);
    }

    cloud_queue_.clear();
  }


  void pruneAccumulation(
    const rclcpp::Time & current_stamp)
  {
    if (!accumulation_enabled_) {
      while (cloud_queue_.size() > 1) {
        cloud_queue_.pop_front();
      }

      return;
    }

    // Time-window pruning.
    while (!cloud_queue_.empty()) {
      const double age_s =
        (current_stamp -
         cloud_queue_.front().stamp).seconds();

      if (age_s <= accumulation_window_s_) {
        break;
      }

      cloud_queue_.pop_front();
    }

    // Hard safety cap.
    while (
      static_cast<int>(cloud_queue_.size()) >
      accumulation_max_frames_)
    {
      cloud_queue_.pop_front();
    }
  }


  void process(
    const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg)
  {
    const std::string radar_frame =
      normalizeFrameId(
        msg->header.frame_id);

    RCLCPP_DEBUG_THROTTLE(
      get_logger(),
      *get_clock(),
      2000,
      "Radar callback active: stamp=%lld.%09u, frame='%s'",
      static_cast<long long>(msg->header.stamp.sec),
      msg->header.stamp.nanosec,
      radar_frame.c_str());

    if (radar_frame.empty()) {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        5000,
        "Radar PointCloud2 has an empty frame_id");

      return;
    }

    RCLCPP_INFO_ONCE(
      get_logger(),
      "Radar input frame_id='%s', normalized='%s'",
      msg->header.frame_id.c_str(),
      radar_frame.c_str());


    // Normalize legacy ROS 1 frame IDs before TF2 lookup.
    sensor_msgs::msg::PointCloud2 normalized_msg =
      *msg;

    normalized_msg.header.frame_id =
      radar_frame;

    sensor_msgs::msg::PointCloud2 transformed_msg;

    // Radar origin expressed in target_frame_.  The point cloud is transformed
    // into target_frame_, but the Doppler direction must still be computed
    // relative to the physical Radar origin.
    double radar_origin_x = 0.0;
    double radar_origin_y = 0.0;


    try {
      const auto transform =
        tf_buffer_->lookupTransform(
          target_frame_,
          radar_frame,
          tf2::TimePointZero);

      radar_origin_x =
        transform.transform.translation.x;
      radar_origin_y =
        transform.transform.translation.y;

      RCLCPP_INFO_ONCE(
        get_logger(),
        "Using TF '%s' -> '%s': "
        "translation=(%.3f, %.3f, %.3f)",
        radar_frame.c_str(),
        target_frame_.c_str(),
        transform.transform.translation.x,
        transform.transform.translation.y,
        transform.transform.translation.z);

      tf2::doTransform(
        normalized_msg,
        transformed_msg,
        transform);

      // Keep the original Radar measurement time.
      transformed_msg.header.stamp =
        msg->header.stamp;

      transformed_msg.header.frame_id =
        target_frame_;

    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        5000,
        "Cannot transform Radar cloud from '%s' "
        "to '%s': %s",
        radar_frame.c_str(),
        target_frame_.c_str(),
        ex.what());

      return;
    }


    const rclcpp::Time current_stamp(
      transformed_msg.header.stamp,
      RCL_ROS_TIME);

    const std::int64_t current_stamp_ns =
      current_stamp.nanoseconds();

    if (current_stamp_ns <= 0) {
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        5000,
        "Radar cloud has an invalid zero timestamp");

      return;
    }

    // Rosbag loop / time reset protection.
    constexpr std::int64_t kBackwardJumpNs =
      1000LL * 1000LL * 1000LL;

    if (
      last_input_stamp_ns_ > 0 &&
      current_stamp_ns <
        last_input_stamp_ns_ - kBackwardJumpNs)
    {
      // Rosbag loop / simulated-time reset:
      // discard historical accumulation AND accept this frame as the new
      // timeline origin.  Do not fall through to the ordinary out-of-order
      // rejection below, otherwise every frame of the new bag loop would be
      // dropped until its timestamp caught up with the previous loop.
      clearAccumulation(
        "detected Radar timestamp reset/backward jump");

      RCLCPP_WARN(
        get_logger(),
        "Radar timestamp reset: old=%lld ns, new=%lld ns; "
        "starting a new accumulation timeline",
        static_cast<long long>(last_input_stamp_ns_),
        static_cast<long long>(current_stamp_ns));

      last_input_stamp_ns_ =
        current_stamp_ns;
    }
    else if (
      last_input_stamp_ns_ > 0 &&
      current_stamp_ns < last_input_stamp_ns_)
    {
      // Small reordering inside the same timeline is still rejected.
      RCLCPP_WARN_THROTTLE(
        get_logger(),
        *get_clock(),
        5000,
        "Dropping mildly out-of-order Radar frame: "
        "current=%lld ns, last=%lld ns",
        static_cast<long long>(current_stamp_ns),
        static_cast<long long>(last_input_stamp_ns_));

      return;
    }
    else
    {
      last_input_stamp_ns_ =
        current_stamp_ns;
    }


    auto current =
      std::make_shared<
        pcl::PointCloud<PointXYZIV>>();

    pcl::fromROSMsg(
      transformed_msg,
      *current);


    auto filtered =
      std::make_shared<
        pcl::PointCloud<PointXYZIV>>();

    filtered->reserve(
      current->size());

    for (auto point : current->points) {
      if (
        point.intensity <
        static_cast<float>(intensity_min_))
      {
        continue;
      }

      if (project_to_xy_) {
        point.z = 0.0F;
      }

      filtered->push_back(point);
    }


    filtered->width =
      static_cast<std::uint32_t>(
        filtered->points.size());

    filtered->height = 1;
    filtered->is_dense = true;


    // A disabled accumulator behaves as a true single-frame filter.
    if (!accumulation_enabled_) {
      cloud_queue_.clear();
    }

    cloud_queue_.push_back(
      RadarFrame{
        current_stamp,
        filtered,
        radar_origin_x,
        radar_origin_y});

    pruneAccumulation(
      current_stamp);


    // Assemble points while preserving the age of every source frame.
    pcl::PointCloud<PointXYZIVT> assembled;

    std::size_t total_points = 0;

    for (const auto & frame : cloud_queue_) {
      total_points +=
        frame.cloud->size();
    }

    assembled.reserve(
      total_points);

    std::size_t compensated_points = 0;
    double max_abs_compensation_m = 0.0;

    for (const auto & frame : cloud_queue_) {
      const float time_offset =
        static_cast<float>(
          (frame.stamp -
           current_stamp).seconds());

      // Historical frames have negative time_offset.
      const double age_s =
        std::max(
          0.0,
          -static_cast<double>(time_offset));

      for (const auto & point :
           frame.cloud->points)
      {
        PointXYZIVT output_point;

        output_point.x = point.x;
        output_point.y = point.y;
        output_point.z = point.z;
        output_point.intensity =
          point.intensity;
        output_point.velocity =
          point.velocity;
        output_point.time_offset =
          time_offset;

        // The recorded platform is stationary, so ego-motion is zero.
        // Compensate only the target's measured radial Doppler motion.
        //
        // TI convention:
        //   velocity > 0 : target moving away from Radar
        //   velocity < 0 : target moving towards Radar
        //
        // The point has already been transformed into target_frame_, therefore
        // its radial direction must be measured from the transformed Radar
        // origin, not from (0, 0) of target_frame_.
        if (
          doppler_compensation_enabled_ &&
          age_s > 0.0 &&
          std::isfinite(point.velocity))
        {
          const double dx =
            static_cast<double>(point.x) -
            frame.radar_origin_x;

          const double dy =
            static_cast<double>(point.y) -
            frame.radar_origin_y;

          const double planar_range =
            std::hypot(dx, dy);

          constexpr double kMinRangeM = 1.0e-3;

          if (planar_range > kMinRangeM) {
            const double radial_shift_m =
              static_cast<double>(point.velocity) *
              age_s;

            const double ux =
              dx / planar_range;
            const double uy =
              dy / planar_range;

            output_point.x =
              static_cast<float>(
                static_cast<double>(point.x) +
                radial_shift_m * ux);

            output_point.y =
              static_cast<float>(
                static_cast<double>(point.y) +
                radial_shift_m * uy);

            ++compensated_points;

            max_abs_compensation_m =
              std::max(
                max_abs_compensation_m,
                std::abs(radial_shift_m));
          }
        }

        assembled.push_back(
          output_point);
      }
    }


    assembled.width =
      static_cast<std::uint32_t>(
        assembled.points.size());

    assembled.height = 1;
    assembled.is_dense = true;


    sensor_msgs::msg::PointCloud2 output;

    pcl::toROSMsg(
      assembled,
      output);

    // The output coordinate frame and timestamp always describe
    // the newest Radar frame in this assembled cloud.
    output.header =
      transformed_msg.header;

    publisher_->publish(
      output);


    const double oldest_age_s =
      cloud_queue_.empty()
      ? 0.0
      : std::max(
          0.0,
          (current_stamp -
           cloud_queue_.front().stamp).seconds());

    RCLCPP_INFO_THROTTLE(
      get_logger(),
      *get_clock(),
      2000,
      "Radar accumulation: current=%zu, frames=%zu, points=%zu, "
      "oldest_age=%.3f s, doppler_compensated=%zu, "
      "max_abs_shift=%.3f m",
      filtered->size(),
      cloud_queue_.size(),
      assembled.size(),
      oldest_age_s,
      compensated_points,
      max_abs_compensation_m);
  }


  std::string target_frame_;

  double intensity_min_ = 15.0;

  bool project_to_xy_ = true;

  bool accumulation_enabled_ = true;

  double accumulation_window_s_ = 0.25;

  int accumulation_max_frames_ = 4;

  bool doppler_compensation_enabled_ = true;


  std::unique_ptr<
    tf2_ros::Buffer> tf_buffer_;

  std::shared_ptr<
    tf2_ros::TransformListener> tf_listener_;

  rclcpp::Subscription<
    sensor_msgs::msg::PointCloud2>::SharedPtr
    subscription_;

  rclcpp::Publisher<
    sensor_msgs::msg::PointCloud2>::SharedPtr
    publisher_;

  std::deque<RadarFrame>
    cloud_queue_;

  std::int64_t
    last_input_stamp_ns_ = 0;
};


int main(
  int argc,
  char ** argv)
{
  rclcpp::init(
    argc,
    argv);

  rclcpp::spin(
    std::make_shared<
      PointCloudAssembler>());

  rclcpp::shutdown();

  return 0;
}
