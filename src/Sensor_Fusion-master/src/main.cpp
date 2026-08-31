#include <rclcpp/rclcpp.hpp>
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
const double DISTANCE_THRESHOLD =1.0;  // 距离阈值，根据实际需求设置
const double MAX_TIMEOUT = 5.0; // Timeout in seconds
std::set<int> previous_marker_ids; // 记录上一帧的所有Marker ID
std::set<int> previous_marker_text_ids; // 记录上一帧的所有Marker_text ID
std::int64_t last_callback_timestamp_ns = 0;
volatile std::sig_atomic_t stop_requested = 0;

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
        if (time_diff > MAX_TIMEOUT) {
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

void sensorCallback(const ros_plc::msg::RadarMsgArray::ConstSharedPtr & msg,
                    const ros_plc::msg::LidarMsgArray::ConstSharedPtr & lidar_msg) {
    std::set<int> updated_ids;
    std::set<int> MatchLidar_ids;
    const std::int64_t timestamp_ns = latestInputTimestamp(*msg, *lidar_msg);
    if (timestamp_ns <= 0) {
        RCLCPP_WARN(logger, "Dropping synchronized input with an invalid zero timestamp");
        return;
    }
    if (timestamp_ns < last_callback_timestamp_ns) {
        RCLCPP_WARN(
          logger,
          "Dropping out-of-order synchronized input: current=%lld ns, last=%lld ns, lag=%.3f ms",
          static_cast<long long>(timestamp_ns),
          static_cast<long long>(last_callback_timestamp_ns),
          (last_callback_timestamp_ns - timestamp_ns) / 1000000.0);
        return;
    }
    last_callback_timestamp_ns = timestamp_ns;
    const rclcpp::Time timestamp_ros(timestamp_ns, RCL_ROS_TIME);
    std::vector<visualization_msgs::msg::Marker> markers; // 用于存储所有Marker
    std::vector<visualization_msgs::msg::Marker> markers_text; // 用于存储所有Marker_text
    visualization_msgs::msg::MarkerArray delete_markers;// 用于删除之前的Marker
    visualization_msgs::msg::MarkerArray delete_markers_text;// 用于删除之前的Marker_text

    //清除之前的fusion_markers
    for (int id : previous_marker_ids) {
        visualization_msgs::msg::Marker delete_marker;
        delete_marker.action = visualization_msgs::msg::Marker::DELETE; // 删除操作
        delete_marker.id = id;                                    // 指定要删除的ID
        delete_marker.header.frame_id = "velodyne";          // 替换为实际的frame_id
        delete_marker.ns = "fusion"; 
        delete_marker.header.stamp = timestamp_ros;
        delete_markers.markers.push_back(delete_marker);
    }
    marker_pub->publish(delete_markers); // 发布删除消息
    previous_marker_ids.clear();

    //清除之前的fusion_markers
    for (int id : previous_marker_text_ids) {
        visualization_msgs::msg::Marker delete_marker;
        delete_marker.action = visualization_msgs::msg::Marker::DELETE; // 删除操作
        delete_marker.id = id;                                    // 指定要删除的ID
        delete_marker.header.frame_id = "velodyne";          // 替换为实际的frame_id
        delete_marker.ns = "fusion_text"; 
        delete_marker.header.stamp = timestamp_ros;
        delete_markers_text.markers.push_back(delete_marker);
    }
    marker_pub_text->publish(delete_markers_text); // 发布删除消息
    previous_marker_text_ids.clear();

    //处理接受数据
    if (!msg->objects.empty()) {
        for (const auto& radar_msg : msg->objects) {
            int original_id = radar_msg.original_id;
            int closest_lidar_id = -1;
            double min_distance=DISTANCE_THRESHOLD;
           const ros_plc::msg::LidarMsg* closest_lidar_obj = nullptr; // 保存最近的 lidar_obj
            bool ifMatchLidar =false;

            //找到lidar对应物体id
            for (const auto& lidar_obj : lidar_msg->objects) {
                double distance = calculateDistance(radar_msg.x, radar_msg.y, lidar_obj.x, lidar_obj.y);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_lidar_obj = &lidar_obj;
                    closest_lidar_id=lidar_obj.original_id;
                }
            }
            const std::int64_t current_timestamp =
              rclcpp::Time(radar_msg.timestamp).nanoseconds();
            
            // Check if the fuser exists in the map
            if (fuser_map.find(original_id) != fuser_map.end()) {
                // Ensure meas_package is initialized
                MeasurementPackage meas_package;
                meas_package.sensor_type_ = MeasurementPackage::RADAR;
                meas_package.raw_measurements_ = Eigen::VectorXd(3);
                double distance = std::sqrt(radar_msg.x * radar_msg.x + radar_msg.y * radar_msg.y);
                meas_package.raw_measurements_ << distance,radar_msg.phi,radar_msg.speed;
                meas_package.timestamp_ = current_timestamp;
               
                // Process the measurement with the existing fuser
                auto & fuser_data = fuser_map[original_id];

                const ros_plc::msg::LidarMsg* selected_lidar_obj = nullptr;

                // 优先尝试沿用已经匹配的 LiDAR ID。
                if (fuser_data.lidar_id != -1) {
                    for (const auto& lidar_obj : lidar_msg->objects) {
                        if (lidar_obj.original_id == fuser_data.lidar_id) {
                            const double matched_distance =
                                calculateDistance(
                                    radar_msg.x,
                                    radar_msg.y,
                                    lidar_obj.x,
                                    lidar_obj.y);

                            if (matched_distance < DISTANCE_THRESHOLD) {
                                selected_lidar_obj = &lidar_obj;
                            }
                            break;
                        }
                    }
                }

                // 原匹配不存在或距离过大时，退回当前最近的 LiDAR。
                if (selected_lidar_obj == nullptr && closest_lidar_obj != nullptr) {
                    selected_lidar_obj = closest_lidar_obj;
                }

                bool radar_processed = false;
                bool lidar_processed = false;

                if (selected_lidar_obj != nullptr) {
                    MeasurementPackage lidar_meas_package;
                    lidar_meas_package.sensor_type_ = MeasurementPackage::LASER;
                    lidar_meas_package.raw_measurements_ = Eigen::VectorXd(2);
                    lidar_meas_package.raw_measurements_
                        << selected_lidar_obj->x, selected_lidar_obj->y;

                    const auto lidar_timestamp =
                        rclcpp::Time(selected_lidar_obj->timestamp).nanoseconds();
                    lidar_meas_package.timestamp_ = lidar_timestamp;

                    // 同一个 fuser 必须按照真实测量时间顺序处理。
                    if (ShouldProcessLidarFirst(lidar_timestamp, current_timestamp)) {
                        lidar_processed =
                            fuser_data.sensor_fusion.Process(lidar_meas_package);
                        if (lidar_processed) {
                            markFuserUpdated(fuser_data, lidar_timestamp);
                        }

                        radar_processed =
                            fuser_data.sensor_fusion.Process(meas_package);
                        if (radar_processed) {
                            markFuserUpdated(fuser_data, current_timestamp);
                        }
                    } else {
                        radar_processed =
                            fuser_data.sensor_fusion.Process(meas_package);
                        if (radar_processed) {
                            markFuserUpdated(fuser_data, current_timestamp);
                        }

                        lidar_processed =
                            fuser_data.sensor_fusion.Process(lidar_meas_package);
                        if (lidar_processed) {
                            markFuserUpdated(fuser_data, lidar_timestamp);
                        }
                    }

                    if (lidar_processed) {
                        fuser_data.lidar_id = selected_lidar_obj->original_id;
                        ifMatchLidar = true;
                        MatchLidar_ids.insert(selected_lidar_obj->original_id);

                        RCLCPP_INFO(
                            logger,
                            "Matched lidar id=%d,x:%f,y:%f",
                            selected_lidar_obj->original_id,
                            selected_lidar_obj->x,
                            selected_lidar_obj->y);
                    }
                } else {
                    radar_processed =
                        fuser_data.sensor_fusion.Process(meas_package);

                    if (radar_processed) {
                        markFuserUpdated(fuser_data, current_timestamp);
                    }

                    RCLCPP_WARN(logger, "No closed lidarObject");
                }

                if (radar_processed) {
                    RCLCPP_INFO(
                        logger,
                        "Radar Data id=%d,x:%f,y:%f",
                        original_id,
                        radar_msg.x,
                        radar_msg.y);
                }

                 // Access the fusion result
                 Eigen::Vector4d x_out = fuser_map[original_id].sensor_fusion.kf_.GetX();
                 if(ifMatchLidar==true){
                    RCLCPP_INFO(logger, "Fusion result:id=%d x=%f, y=%f, vx=%f, vy=%f",original_id,x_out(0), x_out(1), x_out(2), x_out(3));
                 }else{
                    RCLCPP_INFO(logger, "Single Radar result:id=%d x=%f, y=%f, vx=%f, vy=%f",original_id,x_out(0), x_out(1), x_out(2), x_out(3));
                 }
                  // 保存融合结果到Marker
                 visualization_msgs::msg::Marker marker;
                 marker.header.frame_id = "velodyne";
                 marker.header.stamp =timestamp_ros;
                 marker.ns = "fusion";
                 marker.id = original_id;
                 marker.type = visualization_msgs::msg::Marker::SPHERE;
                 marker.action = visualization_msgs::msg::Marker::ADD;
                 marker.pose.position.x = x_out(0);
                 marker.pose.position.y = x_out(1);
                 marker.pose.position.z = 0.0;
                 marker.scale.x = 0.5;
                 marker.scale.y = 0.5;
                 marker.scale.z = 0.5;
                 marker.color.a = 1.0;
                 if(ifMatchLidar==true){
                    marker.color.r = 0.0;
                    marker.color.g = 1.0;
                    marker.color.b = 0.0;
                 }else{
                    marker.color.r = 1.0;
                    marker.color.g = 1.0;
                    marker.color.b = 0.0;
                 }
                 markers.push_back(marker);
                 previous_marker_ids.insert(original_id);
                 // 为当前物体创建一个文本标记
                 visualization_msgs::msg::Marker text_marker;
                 text_marker.header.frame_id = "velodyne";
                 text_marker.header.stamp = timestamp_ros;
                 text_marker.ns = "fusion_text";
                 text_marker.id = original_id; // 确保 ID 不冲突
                 text_marker.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
                 text_marker.action = visualization_msgs::msg::Marker::ADD;
                 text_marker.pose.position.x = x_out(0);
                 text_marker.pose.position.y = x_out(1);
                 text_marker.pose.position.z = 0.5; // 将文本显示在物体上方
                 text_marker.scale.z = 0.1; // 文本大小
                 text_marker.color.a = 1.0;
                if(ifMatchLidar==true){
                    text_marker.color.r = 0.0;
                    text_marker.color.g = 1.0;
                    text_marker.color.b = 0.0;
                 }else{
                    text_marker.color.r = 1.0;
                    text_marker.color.g = 1.0;
                    text_marker.color.b = 0.0;
                 }

                 // 设置文本内容为 ID 和坐标
                 std::ostringstream oss;
                 oss << "ID: " << original_id << "\n";
                 oss << "X: " << x_out(0) << ", Y: " << x_out(1);
                 text_marker.text = oss.str();
                 // 将文本标记添加到 markers 数组
                 markers_text.push_back(text_marker);
                 previous_marker_text_ids.insert(original_id);
            } else {
                // If the fuser doesn't exist, create a new one
                RCLCPP_INFO(logger, "Creating new fuser for Radar id: %d", original_id);
                 // Ensure meas_package is initialized
                MeasurementPackage meas_package;
                meas_package.sensor_type_ = MeasurementPackage::RADAR;
                meas_package.raw_measurements_ = Eigen::VectorXd(3);
                double distance = std::sqrt(radar_msg.x * radar_msg.x + radar_msg.y * radar_msg.y);
                meas_package.raw_measurements_ << distance,radar_msg.phi,radar_msg.speed;
                meas_package.timestamp_ = current_timestamp;

                SensorFusion new_fuser;

                // 如果找到对应的激光雷达物体
                if (closest_lidar_id != -1) {
                    MeasurementPackage lidar_meas_package;
                    lidar_meas_package.sensor_type_ = MeasurementPackage::LASER;
                    lidar_meas_package.raw_measurements_ = Eigen::VectorXd(2);
                    lidar_meas_package.raw_measurements_
                        << closest_lidar_obj->x, closest_lidar_obj->y;

                    const auto lidar_timestamp =
                        rclcpp::Time(closest_lidar_obj->timestamp).nanoseconds();
                    lidar_meas_package.timestamp_ = lidar_timestamp;

                    bool radar_processed = false;
                    bool lidar_processed = false;

                    if (ShouldProcessLidarFirst(lidar_timestamp, current_timestamp)) {
                        lidar_processed = new_fuser.Process(lidar_meas_package);
                        radar_processed = new_fuser.Process(meas_package);
                    } else {
                        radar_processed = new_fuser.Process(meas_package);
                        lidar_processed = new_fuser.Process(lidar_meas_package);
                    }

                    const int lidar_id =
                        lidar_processed ? closest_lidar_obj->original_id : -1;

                    fuser_map[original_id] = {
                        new_fuser,
                        rclcpp::Time(new_fuser.LastTimestamp(), RCL_ROS_TIME),
                        lidar_id};

                    if (lidar_processed) {
                        RCLCPP_INFO(
                        logger,
                        "Creating closed lidarObject id:%d",
                        closest_lidar_obj->original_id);
                    }

                    if (!radar_processed) {
                        RCLCPP_WARN(
                        logger,
                        "Radar measurement was not processed for new fuser id:%d",
                        original_id);
                    }
                } else {
                    RCLCPP_WARN(logger, "No closed lidarObject");

                    new_fuser.Process(meas_package);

                    fuser_map[original_id] = {
                        new_fuser,
                        rclcpp::Time(new_fuser.LastTimestamp(), RCL_ROS_TIME),
                        -1};
                }
            }
            //记录处理更新的id
            updated_ids.insert(original_id);
        }
    } else {
        RCLCPP_WARN(logger, "Received empty RadarMsgArray.");
    }
    //map过期内容清除
     removeOutdatedFusers(timestamp_ros); 

     //对数据未接收物体进行预测追踪
     for (auto it = fuser_map.begin(); it != fuser_map.end();) {
        int original_id = it->first;
        if (updated_ids.find(original_id) == updated_ids.end()) { 
            SensorFusion& fuser = it->second.sensor_fusion; 
            bool ifMatchLidar_predict=false;
            //判断激光雷达是不是有数据，是则用激光雷达进行输入
            if(it->second.lidar_id != -1){
                int matched_lidar_id = it->second.lidar_id; 
                 const ros_plc::msg::LidarMsg* matched_lidar_obj = nullptr;
                 // 遍历 lidar_msg 查找匹配的 lidar 对象
                 for (const auto& lidar_obj : lidar_msg->objects) {
                     if (lidar_obj.original_id == matched_lidar_id) {
                         matched_lidar_obj = &lidar_obj;
                         break;
                     }
                 }
                 //如果对应id雷达数据存在则更新
                 if (matched_lidar_obj&&MatchLidar_ids.find(matched_lidar_obj->original_id) == MatchLidar_ids.end()) {
                     MeasurementPackage lidar_meas_package;
                     lidar_meas_package.sensor_type_ = MeasurementPackage::LASER;
                     lidar_meas_package.raw_measurements_ = Eigen::VectorXd(2);
                     lidar_meas_package.raw_measurements_ << matched_lidar_obj->x, matched_lidar_obj->y;

                    const auto lidar_timestamp =
                        rclcpp::Time(matched_lidar_obj->timestamp).nanoseconds();
                    lidar_meas_package.timestamp_ = lidar_timestamp;

                    // 对 fuser 执行 Process 操作
                    ifMatchLidar_predict = fuser.Process(lidar_meas_package);
                    if (ifMatchLidar_predict) {
                        markFuserUpdated(it->second, lidar_timestamp);
                    }
                 } else {
                     fuser.Predict(timestamp_ns);
                 }
            } else {
                 fuser.Predict(timestamp_ns);
            }
            //获取预测追踪数据
            Eigen::Vector4d x_out = fuser.kf_.GetX();  
            if(ifMatchLidar_predict==false){
                RCLCPP_INFO(logger, "Kalman Predict result: id=%d, x=%f, y=%f, vx=%f, vy=%f", original_id, x_out(0), x_out(1), x_out(2), x_out(3));
            }else{
                RCLCPP_INFO(logger, "Single Lidar result: id=%d, x=%f, y=%f, vx=%f, vy=%f", original_id, x_out(0), x_out(1), x_out(2), x_out(3));
            }
            // 保存预测结果到Marker
            visualization_msgs::msg::Marker marker;
            marker.header.frame_id = "velodyne";
            marker.header.stamp =timestamp_ros;
            marker.ns = "fusion";
            marker.id = original_id;
            marker.type = visualization_msgs::msg::Marker::CUBE;
            marker.action = visualization_msgs::msg::Marker::ADD;
            marker.pose.position.x = x_out(0);
            marker.pose.position.y = x_out(1);
            marker.pose.position.z = 0.0;
            marker.scale.x = 0.5;
            marker.scale.y = 0.5;
            marker.scale.z = 0.5;
            marker.color.a = 1.0;
            if(ifMatchLidar_predict==true){
               marker.color.r = 1.0;
               marker.color.g = 0.0;
               marker.color.b = 0.0;
            }else{
               marker.color.r = 0.0;
               marker.color.g = 1.0;
               marker.color.b = 1.0;
            }
            
            markers.push_back(marker);
            previous_marker_ids.insert(original_id);

            // 为当前物体创建一个文本标记
            visualization_msgs::msg::Marker text_marker;
            text_marker.header.frame_id = "velodyne";
            text_marker.header.stamp = timestamp_ros;
            text_marker.ns = "fusion_text";
            text_marker.id = original_id; // 确保 ID 不冲突
            text_marker.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
            text_marker.action = visualization_msgs::msg::Marker::ADD;
            text_marker.pose.position.x = x_out(0);
            text_marker.pose.position.y = x_out(1);
            text_marker.pose.position.z = 0.5; // 将文本显示在物体上方
            text_marker.scale.z = 0.1; // 文本大小
            text_marker.color.a = 1.0;
            if(ifMatchLidar_predict==true){
               text_marker.color.r = 1.0;
               text_marker.color.g = 0.0;
               text_marker.color.b = 0.0;
            }else{
               text_marker.color.r = 0.0;
               text_marker.color.g = 1.0;
               text_marker.color.b = 1.0;
            }

            // 设置文本内容为 ID 和坐标
            std::ostringstream oss;
            oss << "ID: " << original_id << "\n";
            oss << "X: " << x_out(0) << ", Y: " << x_out(1);
            text_marker.text = oss.str();

            // 将文本标记添加到 markers 数组
            markers_text.push_back(text_marker);
            previous_marker_text_ids.insert(original_id);

            ++it;
        } else {
            ++it;  
        }
    }
    // 发布所有Marker
    visualization_msgs::msg::MarkerArray marker_array;
    visualization_msgs::msg::MarkerArray marker_array_text;
    marker_array.markers = markers;
    marker_array_text.markers = markers_text;
    // 发布消息
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

    {
        auto node = std::make_shared<rclcpp::Node>("fusion_data_publisher");
        // 清空 fuser_map，确保每次运行时重新开始
        fuser_map.clear();
        last_callback_timestamp_ns = 0;

        marker_pub = node->create_publisher<visualization_msgs::msg::MarkerArray>("fusion_markers", 10);
        marker_pub_text = node->create_publisher<visualization_msgs::msg::MarkerArray>("fusion_markers_text", 10);
        // 订阅目标话题并传入回调函数,时间同步
        message_filters::Subscriber<ros_plc::msg::RadarMsgArray> radar_sub(node, "/radar_objects");
        message_filters::Subscriber<ros_plc::msg::LidarMsgArray> lidar_sub(node, "/lidar_objects");
        using MySyncPolicy = message_filters::sync_policies::ApproximateTime<
          ros_plc::msg::RadarMsgArray, ros_plc::msg::LidarMsgArray>;
        message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), radar_sub, lidar_sub);
        sync.registerCallback(std::bind(&safeSensorCallback, std::placeholders::_1, std::placeholders::_2));

        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node);
        while (rclcpp::ok() && !stop_requested) {
            executor.spin_once(std::chrono::milliseconds(100));
        }
        executor.remove_node(node);

        // DDS 实体必须在 context shutdown 之前销毁，否则 Fast DDS 清理时会崩溃。
        marker_pub.reset();
        marker_pub_text.reset();
        fuser_map.clear();
    }

    rclcpp::shutdown();
    return 0;
}
