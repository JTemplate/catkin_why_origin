#include <rclcpp/rclcpp.hpp>

#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/exceptions.h>
#include <tf2/time.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <csignal>
#include <exception>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include "Eigen/Eigen"
#include "interface/measurement_package.h"
#include <message_filters/subscriber.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/synchronizer.h>
#include "sensorfusion.h"
#include "association.h"
#include "ros_plc/msg/lidar_msg_array.hpp"
#include "ros_plc/msg/lidar_msg.hpp"
#include "ros_plc/msg/radar_msg_array.hpp"
#include "ros_plc/msg/radar_msg.hpp"
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

using namespace std;

struct FuserData {
    SensorFusion sensor_fusion;
    rclcpp::Time timestamp;
    int lidar_id;
};
// Map to hold each fuser with last updated time
std::map<int,FuserData> fuser_map;
rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub;
rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_text;
rclcpp::Logger logger = rclcpp::get_logger("sensor_fusion");
double association_max_distance_m = 1.0;
double fuser_timeout_s = 5.0;
SensorFusionConfig sensor_fusion_config;
std::set<int> previous_marker_ids; // 记录上一帧的所有Marker ID
std::set<int> previous_marker_text_ids; // 记录上一帧的所有Marker_text ID
std::int64_t last_callback_timestamp_ns = 0;
volatile std::sig_atomic_t stop_requested = 0;

struct RadarExtrinsic {
    double translation_x = 0.0;
    double translation_y = 0.0;
    double yaw = 0.0;
};

RadarExtrinsic radar_extrinsic;

bool loadRadarExtrinsicFromTf(
    tf2_ros::Buffer & tf_buffer,
    const std::string & fusion_frame,
    const std::string & radar_frame)
{
    try {
        const auto transform = tf_buffer.lookupTransform(
            fusion_frame,
            radar_frame,
            tf2::TimePointZero,
            tf2::durationFromSec(5.0));

        const auto & translation =
            transform.transform.translation;

        const auto & rotation =
            transform.transform.rotation;

        tf2::Quaternion quaternion(
            rotation.x,
            rotation.y,
            rotation.z,
            rotation.w);

        double roll = 0.0;
        double pitch = 0.0;
        double yaw = 0.0;

        tf2::Matrix3x3(quaternion).getRPY(
            roll,
            pitch,
            yaw);

        if (!std::isfinite(translation.x) ||
            !std::isfinite(translation.y) ||
            !std::isfinite(roll) ||
            !std::isfinite(pitch) ||
            !std::isfinite(yaw))
        {
            RCLCPP_ERROR(
                logger,
                "Radar extrinsic TF '%s' -> '%s' contains "
                "non-finite values",
                radar_frame.c_str(),
                fusion_frame.c_str());

            return false;
        }

        // Current fusion model is planar:
        // [px, py, vx, vy] + Radar [rho, phi, rho_dot].
        // mmwave_filter may apply a full 3-D TF, but a non-zero
        // Radar roll/pitch cannot be represented consistently here.
        constexpr double kPlanarAngleTolerance = 1.0e-3;

        if (std::fabs(roll) > kPlanarAngleTolerance ||
            std::fabs(pitch) > kPlanarAngleTolerance)
        {
            RCLCPP_ERROR(
                logger,
                "Radar TF '%s' -> '%s' is not planar: "
                "roll=%.6f rad, pitch=%.6f rad. "
                "The current 2-D fusion model requires both "
                "to be approximately zero.",
                radar_frame.c_str(),
                fusion_frame.c_str(),
                roll,
                pitch);

            return false;
        }

        radar_extrinsic.translation_x =
            translation.x;

        radar_extrinsic.translation_y =
            translation.y;

        radar_extrinsic.yaw =
            yaw;

        RCLCPP_INFO(
            logger,
            "Loaded Radar extrinsic from TF '%s' -> '%s': "
            "translation=(%.6f, %.6f, %.6f), "
            "rpy=(%.6f, %.6f, %.6f)",
            radar_frame.c_str(),
            fusion_frame.c_str(),
            translation.x,
            translation.y,
            translation.z,
            roll,
            pitch,
            yaw);

        return true;

    } catch (const tf2::TransformException & ex) {
        RCLCPP_ERROR(
            logger,
            "Failed to load Radar extrinsic TF "
            "'%s' -> '%s': %s",
            radar_frame.c_str(),
            fusion_frame.c_str(),
            ex.what());

        return false;
    }
}

void requestStop(int) {
    stop_requested = 1;
}

// 计算两个点之间的欧几里得距离
double calculateDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// Function to remove outdated fusers
void removeOutdatedFusers(const rclcpp::Time & current_time) {
    for (auto it = fuser_map.begin(); it != fuser_map.end(); ) {
        double time_diff = (current_time - it->second.timestamp).seconds();
        if (time_diff > fuser_timeout_s) {
            RCLCPP_INFO(logger, "Removing outdated fuser with ID: %d, last updated %.2f seconds ago", it->first, time_diff);
            it = fuser_map.erase(it); // Remove outdated entry
        } else {
            ++it;
        }
    } 
}

std::int64_t latestInputTimestamp(
    const ros_plc::msg::RadarMsgArray & radar_msg,
    const ros_plc::msg::LidarMsgArray & lidar_msg) {
    std::int64_t latest = std::max(
      rclcpp::Time(radar_msg.header.stamp).nanoseconds(),
      rclcpp::Time(lidar_msg.header.stamp).nanoseconds());
    for (const auto & object : radar_msg.objects) {
        latest = std::max(latest, rclcpp::Time(object.timestamp).nanoseconds());
    }
    for (const auto & object : lidar_msg.objects) {
        latest = std::max(latest, rclcpp::Time(object.timestamp).nanoseconds());
    }
    return latest;
}

void markFuserUpdated(FuserData & fuser, std::int64_t timestamp_ns) {
    const rclcpp::Time update_time(timestamp_ns, RCL_ROS_TIME);
    if (update_time > fuser.timestamp) {
        fuser.timestamp = update_time;
    }
}

void sensorCallback(
    const ros_plc::msg::RadarMsgArray::ConstSharedPtr & msg,
    const ros_plc::msg::LidarMsgArray::ConstSharedPtr & lidar_msg)
{
    // Snapshot the Radar -> fusion-frame extrinsic loaded from TF
    // during startup. TF is the single numerical calibration source.
    const double kRadarTranslationX =
        radar_extrinsic.translation_x;

    const double kRadarTranslationY =
        radar_extrinsic.translation_y;

    const double kRadarYaw =
        radar_extrinsic.yaw;

    std::set<int> updated_ids;
    std::set<int> MatchLidar_ids;

    const std::int64_t timestamp_ns =
        latestInputTimestamp(*msg, *lidar_msg);

    if (timestamp_ns <= 0) {
        RCLCPP_WARN(
            logger,
            "Dropping synchronized input with an invalid zero timestamp");
        return;
    }

    if (timestamp_ns < last_callback_timestamp_ns) {
        RCLCPP_WARN(
            logger,
            "Dropping out-of-order synchronized input: "
            "current=%lld ns, last=%lld ns, lag=%.3f ms",
            static_cast<long long>(timestamp_ns),
            static_cast<long long>(last_callback_timestamp_ns),
            (last_callback_timestamp_ns - timestamp_ns) / 1000000.0);
        return;
    }

    last_callback_timestamp_ns = timestamp_ns;

    const rclcpp::Time timestamp_ros(
        timestamp_ns,
        RCL_ROS_TIME);

    std::vector<visualization_msgs::msg::Marker> markers;
    std::vector<visualization_msgs::msg::Marker> markers_text;

    visualization_msgs::msg::MarkerArray delete_markers;
    visualization_msgs::msg::MarkerArray delete_markers_text;

    // Clear previous fusion markers.
    for (int id : previous_marker_ids) {
        visualization_msgs::msg::Marker delete_marker;

        delete_marker.action =
            visualization_msgs::msg::Marker::DELETE;

        delete_marker.id = id;
        delete_marker.header.frame_id = "velodyne";
        delete_marker.ns = "fusion";
        delete_marker.header.stamp = timestamp_ros;

        delete_markers.markers.push_back(delete_marker);
    }

    marker_pub->publish(delete_markers);
    previous_marker_ids.clear();

    // Clear previous fusion text markers.
    for (int id : previous_marker_text_ids) {
        visualization_msgs::msg::Marker delete_marker;

        delete_marker.action =
            visualization_msgs::msg::Marker::DELETE;

        delete_marker.id = id;
        delete_marker.header.frame_id = "velodyne";
        delete_marker.ns = "fusion_text";
        delete_marker.header.stamp = timestamp_ros;

        delete_markers_text.markers.push_back(delete_marker);
    }

    marker_pub_text->publish(delete_markers_text);
    previous_marker_text_ids.clear();
        // 对当前同步帧执行一次全局 Radar-LiDAR assignment。
    std::vector<AssociationObject> radar_objects;
    radar_objects.reserve(msg->objects.size());

    for (const auto & radar_obj : msg->objects) {
        radar_objects.push_back(
            {
                radar_obj.original_id,
                radar_obj.x,
                radar_obj.y
            });
    }

    std::vector<AssociationObject> lidar_objects;
    lidar_objects.reserve(lidar_msg->objects.size());

    for (const auto & lidar_obj : lidar_msg->objects) {
        lidar_objects.push_back(
            {
                lidar_obj.original_id,
                lidar_obj.x,
                lidar_obj.y
            });
    }

    const auto association_matches =
        AssociateRadarToLidar(
            radar_objects,
            lidar_objects,
            association_max_distance_m);

    std::map<
        int,
        const ros_plc::msg::LidarMsg *
    > assigned_lidar_by_radar;

    for (const auto & match : association_matches) {
        if (match.lidar_id == -1) {
            continue;
        }

        const auto lidar_it =
            std::find_if(
                lidar_msg->objects.begin(),
                lidar_msg->objects.end(),
                [&match](
                    const ros_plc::msg::LidarMsg & lidar_obj)
                {
                    return
                        lidar_obj.original_id ==
                        match.lidar_id;
                });

        if (lidar_it != lidar_msg->objects.end()) {
            assigned_lidar_by_radar[
                match.radar_id
            ] = &(*lidar_it);
        }
    }

    // Process Radar objects.
    if (!msg->objects.empty()) {
        for (const auto & radar_msg : msg->objects) {
            const int original_id = radar_msg.original_id;

            const ros_plc::msg::LidarMsg * assigned_lidar_obj = nullptr;

            const auto assigned_it =
                assigned_lidar_by_radar.find(original_id);

            if (assigned_it != assigned_lidar_by_radar.end()) {
                assigned_lidar_obj = assigned_it->second;
            }
            bool ifMatchLidar = false;

            const std::int64_t current_timestamp =
                rclcpp::Time(radar_msg.timestamp).nanoseconds();

            // Existing fuser.
            if (fuser_map.find(original_id) !=
                fuser_map.end())
            {
                auto & fuser_data =
                    fuser_map[original_id];

                // Keep the EKF Radar measurement geometry synchronized
                // with the Cartesian -> Radar conversion.
                fuser_data.sensor_fusion.SetRadarExtrinsic(
                    kRadarTranslationX,
                    kRadarTranslationY,
                    kRadarYaw);

                bool radar_processed = false;
                bool lidar_processed = false;

                if (assigned_lidar_obj != nullptr) {
                    MeasurementPackage lidar_meas_package;

                    lidar_meas_package.sensor_type_ =
                        MeasurementPackage::LASER;

                    lidar_meas_package.raw_measurements_ =
                        Eigen::VectorXd(2);

                    lidar_meas_package.raw_measurements_
                        << assigned_lidar_obj->x,
                           assigned_lidar_obj->y;

                    const std::int64_t lidar_timestamp =
                        rclcpp::Time(
                            assigned_lidar_obj->timestamp)
                            .nanoseconds();

                    lidar_meas_package.timestamp_ =
                        lidar_timestamp;

                    // Measurements belonging to the same fuser must
                    // always be processed in real timestamp order.
                    if (ShouldProcessLidarFirst(
                            lidar_timestamp,
                            current_timestamp))
                    {
                        lidar_processed =
                            fuser_data.sensor_fusion.Process(
                                lidar_meas_package);

                        if (lidar_processed) {
                            markFuserUpdated(
                                fuser_data,
                                lidar_timestamp);
                        }

                        radar_processed =
                            fuser_data.sensor_fusion.ProcessRadarCartesian(
                                radar_msg.x,
                                radar_msg.y,
                                radar_msg.speed,
                                current_timestamp);

                        if (radar_processed) {
                            markFuserUpdated(
                                fuser_data,
                                current_timestamp);
                        }

                    } else {
                        radar_processed =
                            fuser_data.sensor_fusion.ProcessRadarCartesian(
                                radar_msg.x,
                                radar_msg.y,
                                radar_msg.speed,
                                current_timestamp);

                        if (radar_processed) {
                            markFuserUpdated(
                                fuser_data,
                                current_timestamp);
                        }

                        lidar_processed =
                            fuser_data.sensor_fusion.Process(
                                lidar_meas_package);

                        if (lidar_processed) {
                            markFuserUpdated(
                                fuser_data,
                                lidar_timestamp);
                        }
                    }

                    if (lidar_processed) {
                        fuser_data.lidar_id =
                            assigned_lidar_obj->original_id;

                        ifMatchLidar = true;

                        MatchLidar_ids.insert(
                            assigned_lidar_obj->original_id);

                        RCLCPP_INFO(
                            logger,
                            "Matched lidar id=%d,x:%f,y:%f",
                            assigned_lidar_obj->original_id,
                            assigned_lidar_obj->x,
                            assigned_lidar_obj->y);
                    }

                } else {
                    radar_processed =
                        fuser_data.sensor_fusion.ProcessRadarCartesian(
                            radar_msg.x,
                            radar_msg.y,
                            radar_msg.speed,
                            current_timestamp);

                    if (radar_processed) {
                        markFuserUpdated(
                            fuser_data,
                            current_timestamp);
                    }

                    RCLCPP_WARN(
                        logger,
                        "No closed lidarObject");
                }

                if (radar_processed) {
                    RCLCPP_INFO(
                        logger,
                        "Radar Data id=%d,x:%f,y:%f",
                        original_id,
                        radar_msg.x,
                        radar_msg.y);
                }

                Eigen::Vector4d x_out =
                    fuser_data.sensor_fusion.kf_.GetX();

                if (ifMatchLidar) {
                    RCLCPP_INFO(
                        logger,
                        "Fusion result:id=%d "
                        "x=%f, y=%f, vx=%f, vy=%f",
                        original_id,
                        x_out(0),
                        x_out(1),
                        x_out(2),
                        x_out(3));
                } else {
                    RCLCPP_INFO(
                        logger,
                        "Single Radar result:id=%d "
                        "x=%f, y=%f, vx=%f, vy=%f",
                        original_id,
                        x_out(0),
                        x_out(1),
                        x_out(2),
                        x_out(3));
                }

                // Fusion position marker.
                visualization_msgs::msg::Marker marker;

                marker.header.frame_id = "velodyne";
                marker.header.stamp = timestamp_ros;
                marker.ns = "fusion";
                marker.id = original_id;

                marker.type =
                    visualization_msgs::msg::Marker::SPHERE;

                marker.action =
                    visualization_msgs::msg::Marker::ADD;

                marker.pose.position.x = x_out(0);
                marker.pose.position.y = x_out(1);
                marker.pose.position.z = 0.0;

                marker.scale.x = 0.5;
                marker.scale.y = 0.5;
                marker.scale.z = 0.5;

                marker.color.a = 1.0;

                if (ifMatchLidar) {
                    marker.color.r = 0.0;
                    marker.color.g = 1.0;
                    marker.color.b = 0.0;
                } else {
                    marker.color.r = 1.0;
                    marker.color.g = 1.0;
                    marker.color.b = 0.0;
                }

                markers.push_back(marker);
                previous_marker_ids.insert(original_id);

                // Fusion text marker.
                visualization_msgs::msg::Marker text_marker;

                text_marker.header.frame_id = "velodyne";
                text_marker.header.stamp = timestamp_ros;
                text_marker.ns = "fusion_text";
                text_marker.id = original_id;

                text_marker.type =
                    visualization_msgs::msg::Marker::
                        TEXT_VIEW_FACING;

                text_marker.action =
                    visualization_msgs::msg::Marker::ADD;

                text_marker.pose.position.x = x_out(0);
                text_marker.pose.position.y = x_out(1);
                text_marker.pose.position.z = 0.5;

                text_marker.scale.z = 0.1;
                text_marker.color.a = 1.0;

                if (ifMatchLidar) {
                    text_marker.color.r = 0.0;
                    text_marker.color.g = 1.0;
                    text_marker.color.b = 0.0;
                } else {
                    text_marker.color.r = 1.0;
                    text_marker.color.g = 1.0;
                    text_marker.color.b = 0.0;
                }

                std::ostringstream oss;

                oss << "ID: " << original_id << "\n";
                oss << "X: " << x_out(0)
                    << ", Y: " << x_out(1);

                text_marker.text = oss.str();

                markers_text.push_back(text_marker);
                previous_marker_text_ids.insert(original_id);

            } else {
                // New fuser.
                RCLCPP_INFO(
                    logger,
                    "Creating new fuser for Radar id: %d",
                    original_id);

                SensorFusion new_fuser(sensor_fusion_config);

                // IMPORTANT:
                // Set the extrinsic before the first Radar measurement.
                // This also makes Radar-first initialization use the
                // correct velodyne-frame state.
                new_fuser.SetRadarExtrinsic(
                    kRadarTranslationX,
                    kRadarTranslationY,
                    kRadarYaw);

                if (assigned_lidar_obj != nullptr){
                    MeasurementPackage lidar_meas_package;

                    lidar_meas_package.sensor_type_ =
                        MeasurementPackage::LASER;

                    lidar_meas_package.raw_measurements_ =
                        Eigen::VectorXd(2);

                    lidar_meas_package.raw_measurements_
                        << assigned_lidar_obj->x,
                           assigned_lidar_obj->y;

                    const std::int64_t lidar_timestamp =
                        rclcpp::Time(
                            assigned_lidar_obj->timestamp)
                            .nanoseconds();

                    lidar_meas_package.timestamp_ =
                        lidar_timestamp;

                    bool radar_processed = false;
                    bool lidar_processed = false;

                    if (ShouldProcessLidarFirst(
                            lidar_timestamp,
                            current_timestamp))
                    {
                        lidar_processed =
                            new_fuser.Process(
                                lidar_meas_package);

                        radar_processed =
                            new_fuser.ProcessRadarCartesian(
                                radar_msg.x,
                                radar_msg.y,
                                radar_msg.speed,
                                current_timestamp);

                    } else {
                        radar_processed =
                            new_fuser.ProcessRadarCartesian(
                                radar_msg.x,
                                radar_msg.y,
                                radar_msg.speed,
                                current_timestamp);

                        lidar_processed =
                            new_fuser.Process(
                                lidar_meas_package);
                    }

                    const int lidar_id =
                        lidar_processed
                            ? assigned_lidar_obj->original_id
                            : -1;

                    fuser_map[original_id] = {
                        new_fuser,
                        rclcpp::Time(
                            new_fuser.LastTimestamp(),
                            RCL_ROS_TIME),
                        lidar_id};

                    if (lidar_processed) {
                        RCLCPP_INFO(
                            logger,
                            "Creating closed lidarObject id:%d",
                            assigned_lidar_obj->original_id);
                    }

                    if (!radar_processed) {
                        RCLCPP_WARN(
                            logger,
                            "Radar measurement was not processed "
                            "for new fuser id:%d",
                            original_id);
                    }

                } else {
                    RCLCPP_WARN(
                        logger,
                        "No closed lidarObject");

                    const bool radar_processed =
                        new_fuser.ProcessRadarCartesian(
                            radar_msg.x,
                            radar_msg.y,
                            radar_msg.speed,
                            current_timestamp);

                    if (!radar_processed) {
                        RCLCPP_WARN(
                            logger,
                            "Radar measurement was not processed "
                            "for new fuser id:%d",
                            original_id);
                    }

                    fuser_map[original_id] = {
                        new_fuser,
                        rclcpp::Time(
                            new_fuser.LastTimestamp(),
                            RCL_ROS_TIME),
                        -1};
                }
            }

            updated_ids.insert(original_id);
        }

    } else {
        RCLCPP_WARN(
            logger,
            "Received empty RadarMsgArray.");
    }

    // Remove stale fusers.
    removeOutdatedFusers(timestamp_ros);

    // For objects that did not receive a Radar update in this callback,
    // use the matched LiDAR measurement if available; otherwise predict.
    for (auto it = fuser_map.begin();
         it != fuser_map.end();)
    {
        const int original_id = it->first;

        if (updated_ids.find(original_id) ==
            updated_ids.end())
        {
            SensorFusion & fuser =
                it->second.sensor_fusion;

            fuser.SetRadarExtrinsic(
                kRadarTranslationX,
                kRadarTranslationY,
                kRadarYaw);

            bool ifMatchLidar_predict = false;

            if (it->second.lidar_id != -1) {
                const int matched_lidar_id =
                    it->second.lidar_id;

                const ros_plc::msg::LidarMsg *
                    matched_lidar_obj = nullptr;

                for (const auto & lidar_obj :
                     lidar_msg->objects)
                {
                    if (lidar_obj.original_id ==
                        matched_lidar_id)
                    {
                        matched_lidar_obj =
                            &lidar_obj;
                        break;
                    }
                }

                if (matched_lidar_obj != nullptr &&
                    MatchLidar_ids.find(
                        matched_lidar_obj->original_id) ==
                        MatchLidar_ids.end())
                {
                    MeasurementPackage lidar_meas_package;

                    lidar_meas_package.sensor_type_ =
                        MeasurementPackage::LASER;

                    lidar_meas_package.raw_measurements_ =
                        Eigen::VectorXd(2);

                    lidar_meas_package.raw_measurements_
                        << matched_lidar_obj->x,
                           matched_lidar_obj->y;

                    const std::int64_t lidar_timestamp =
                        rclcpp::Time(
                            matched_lidar_obj->timestamp)
                            .nanoseconds();

                    lidar_meas_package.timestamp_ =
                        lidar_timestamp;

                    ifMatchLidar_predict =
                        fuser.Process(
                            lidar_meas_package);

                    if (ifMatchLidar_predict) {
                        markFuserUpdated(
                            it->second,
                            lidar_timestamp);
                    }

                } else {
                    fuser.Predict(timestamp_ns);
                }

            } else {
                fuser.Predict(timestamp_ns);
            }

            Eigen::Vector4d x_out =
                fuser.kf_.GetX();

            if (!ifMatchLidar_predict) {
                RCLCPP_INFO(
                    logger,
                    "Kalman Predict result: "
                    "id=%d, x=%f, y=%f, vx=%f, vy=%f",
                    original_id,
                    x_out(0),
                    x_out(1),
                    x_out(2),
                    x_out(3));
            } else {
                RCLCPP_INFO(
                    logger,
                    "Single Lidar result: "
                    "id=%d, x=%f, y=%f, vx=%f, vy=%f",
                    original_id,
                    x_out(0),
                    x_out(1),
                    x_out(2),
                    x_out(3));
            }

            // Prediction / LiDAR-only marker.
            visualization_msgs::msg::Marker marker;

            marker.header.frame_id = "velodyne";
            marker.header.stamp = timestamp_ros;
            marker.ns = "fusion";
            marker.id = original_id;

            marker.type =
                visualization_msgs::msg::Marker::CUBE;

            marker.action =
                visualization_msgs::msg::Marker::ADD;

            marker.pose.position.x = x_out(0);
            marker.pose.position.y = x_out(1);
            marker.pose.position.z = 0.0;

            marker.scale.x = 0.5;
            marker.scale.y = 0.5;
            marker.scale.z = 0.5;

            marker.color.a = 1.0;

            if (ifMatchLidar_predict) {
                marker.color.r = 1.0;
                marker.color.g = 0.0;
                marker.color.b = 0.0;
            } else {
                marker.color.r = 0.0;
                marker.color.g = 1.0;
                marker.color.b = 1.0;
            }

            markers.push_back(marker);
            previous_marker_ids.insert(original_id);

            // Prediction / LiDAR-only text marker.
            visualization_msgs::msg::Marker text_marker;

            text_marker.header.frame_id = "velodyne";
            text_marker.header.stamp = timestamp_ros;
            text_marker.ns = "fusion_text";
            text_marker.id = original_id;

            text_marker.type =
                visualization_msgs::msg::Marker::
                    TEXT_VIEW_FACING;

            text_marker.action =
                visualization_msgs::msg::Marker::ADD;

            text_marker.pose.position.x = x_out(0);
            text_marker.pose.position.y = x_out(1);
            text_marker.pose.position.z = 0.5;

            text_marker.scale.z = 0.1;
            text_marker.color.a = 1.0;

            if (ifMatchLidar_predict) {
                text_marker.color.r = 1.0;
                text_marker.color.g = 0.0;
                text_marker.color.b = 0.0;
            } else {
                text_marker.color.r = 0.0;
                text_marker.color.g = 1.0;
                text_marker.color.b = 1.0;
            }

            std::ostringstream oss;

            oss << "ID: " << original_id << "\n";
            oss << "X: " << x_out(0)
                << ", Y: " << x_out(1);

            text_marker.text = oss.str();

            markers_text.push_back(text_marker);
            previous_marker_text_ids.insert(original_id);

            ++it;

        } else {
            ++it;
        }
    }

    visualization_msgs::msg::MarkerArray marker_array;
    visualization_msgs::msg::MarkerArray marker_array_text;

    marker_array.markers = markers;
    marker_array_text.markers = markers_text;

    marker_pub->publish(marker_array);
    marker_pub_text->publish(marker_array_text);
}



void safeSensorCallback(
    const ros_plc::msg::RadarMsgArray::ConstSharedPtr & radar_msg,
    const ros_plc::msg::LidarMsgArray::ConstSharedPtr & lidar_msg) {
    try {
        sensorCallback(radar_msg, lidar_msg);
    } catch (const std::exception & error) {
        RCLCPP_ERROR(logger, "Fusion callback failed; dropping this input pair: %s", error.what());
    } catch (...) {
        RCLCPP_ERROR(logger, "Fusion callback failed with an unknown error; dropping this input pair");
    }
}

int main(int argc, char **argv) {
    rclcpp::init(
      argc, argv, rclcpp::InitOptions(), rclcpp::SignalHandlerOptions::None);

    std::signal(SIGINT, requestStop);
    std::signal(SIGTERM, requestStop);
    stop_requested = 0;

    int exit_code = 0;

    {
        auto node =
            std::make_shared<rclcpp::Node>(
                "fusion_data_publisher");

        // Frame names are configurable, while the numerical
        // Radar extrinsic itself comes exclusively from TF.
        const std::string fusion_frame =
            node->declare_parameter<std::string>(
                "fusion_frame",
                "velodyne");

        const std::string radar_frame =
            node->declare_parameter<std::string>(
                "radar_frame",
                "ti_mmwave_0");

        association_max_distance_m =
            node->declare_parameter<double>(
                "association.max_distance_m",
                association_max_distance_m);

        fuser_timeout_s =
            node->declare_parameter<double>(
                "fuser.timeout_s",
                fuser_timeout_s);

        sensor_fusion_config.lidar_measurement_variance_x =
            node->declare_parameter<double>(
                "lidar.measurement_variance_x",
                sensor_fusion_config.lidar_measurement_variance_x);

        sensor_fusion_config.lidar_measurement_variance_y =
            node->declare_parameter<double>(
                "lidar.measurement_variance_y",
                sensor_fusion_config.lidar_measurement_variance_y);

        sensor_fusion_config.radar_measurement_variance_range =
            node->declare_parameter<double>(
                "radar.measurement_variance_range",
                sensor_fusion_config.radar_measurement_variance_range);

        sensor_fusion_config.radar_measurement_variance_bearing =
            node->declare_parameter<double>(
                "radar.measurement_variance_bearing",
                sensor_fusion_config.radar_measurement_variance_bearing);

        sensor_fusion_config.radar_measurement_variance_range_rate =
            node->declare_parameter<double>(
                "radar.measurement_variance_range_rate",
                sensor_fusion_config.radar_measurement_variance_range_rate);

        sensor_fusion_config.process_acceleration_variance_x =
            node->declare_parameter<double>(
                "process.acceleration_variance_x",
                sensor_fusion_config.process_acceleration_variance_x);

        sensor_fusion_config.process_acceleration_variance_y =
            node->declare_parameter<double>(
                "process.acceleration_variance_y",
                sensor_fusion_config.process_acceleration_variance_y);

        auto tf_buffer =
            std::make_unique<tf2_ros::Buffer>(
                node->get_clock());

        [[maybe_unused]] auto tf_listener =
            std::make_shared<tf2_ros::TransformListener>(
                *tf_buffer);

        // 清空 fuser_map，确保每次运行时重新开始
        fuser_map.clear();
        last_callback_timestamp_ns = 0;

        if (!loadRadarExtrinsicFromTf(
                *tf_buffer,
                fusion_frame,
                radar_frame))
        {
            RCLCPP_FATAL(
                node->get_logger(),
                "Sensor fusion will not start without a valid "
                "Radar extrinsic TF");

            exit_code = 1;

        } else {
            marker_pub =
                node->create_publisher<
                    visualization_msgs::msg::MarkerArray>(
                    "fusion_markers",
                    10);

            marker_pub_text =
                node->create_publisher<
                    visualization_msgs::msg::MarkerArray>(
                    "fusion_markers_text",
                    10);

            // 订阅目标话题并传入回调函数，时间同步
            message_filters::Subscriber<
                ros_plc::msg::RadarMsgArray>
                radar_sub(
                    node,
                    "/radar_objects");

            message_filters::Subscriber<
                ros_plc::msg::LidarMsgArray>
                lidar_sub(
                    node,
                    "/lidar_objects");

            using MySyncPolicy =
                message_filters::sync_policies::
                    ApproximateTime<
                        ros_plc::msg::RadarMsgArray,
                        ros_plc::msg::LidarMsgArray>;

            message_filters::Synchronizer<
                MySyncPolicy>
                sync(
                    MySyncPolicy(10),
                    radar_sub,
                    lidar_sub);

            sync.registerCallback(
                std::bind(
                    &safeSensorCallback,
                    std::placeholders::_1,
                    std::placeholders::_2));

            rclcpp::executors::SingleThreadedExecutor executor;

            executor.add_node(node);

            while (rclcpp::ok() &&
                   !stop_requested)
            {
                executor.spin_once(
                    std::chrono::milliseconds(100));
            }

            executor.remove_node(node);
        }

        // DDS 实体必须在 context shutdown 之前销毁，
        // 否则 Fast DDS 清理时可能崩溃。
        marker_pub.reset();
        marker_pub_text.reset();
        fuser_map.clear();
    }

    rclcpp::shutdown();

    return exit_code;
}
