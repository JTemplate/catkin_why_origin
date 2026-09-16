#include "lidar_perception.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <sstream>
#include <utility>

#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <visualization_msgs/msg/marker.hpp>

using namespace std;

namespace lidar_perception {

lidar_perception::lidar_perception(const rclcpp::Node::SharedPtr & node)
    : node_(node), clip_(node), rm_(node), cluster_(node) {
    pcl_subscriber_ = node_->create_subscription<sensor_msgs::msg::PointCloud2>(
        "output_cloud", rclcpp::SensorDataQoS(),
        std::bind(&lidar_perception::PclCallback, this, std::placeholders::_1));
    pcl_subscriber_radar = node_->create_subscription<sensor_msgs::msg::PointCloud2>(
        "/mmwaveFiltered", rclcpp::SensorDataQoS(),
        std::bind(&lidar_perception::PclCallback_radar, this, std::placeholders::_1));
    marker_pub_box_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/lidar_perception/marker_box", 1);
    marker_pub_tracinfo_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/lidar_perception/marker_info", 1);
    marker_pub_track_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/lidar_perception/marker_track", 1);
    marker_pub_box_radar = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/radar_perception/marker_box", 1);
    marker_pub_tracinfo_radar = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/radar_perception/marker_info", 1);
    marker_pub_track_radar = node_->create_publisher<visualization_msgs::msg::MarkerArray>("/radar_perception/marker_track", 1);
    pub_pcl_cluster_ = node_->create_publisher<sensor_msgs::msg::PointCloud2>(
        "/lidar_perception/cluster_points", rclcpp::SensorDataQoS());
    pub_pcl_cluster_radar = node_->create_publisher<sensor_msgs::msg::PointCloud2>(
        "/radar_perception/cluster_points", rclcpp::SensorDataQoS());
    lidar_tracked_objects_pub_ =
        node_->create_publisher<ros_plc::msg::LidarMsgArray>(
            "/lidar_perception/tracked_objects", 10);
    radar_tracked_objects_pub_ =
        node_->create_publisher<ros_plc::msg::RadarMsgArray>(
            "/radar_perception/tracked_objects", 10);

    max_marker_size_ = 0;
    max_marker_size_radar=0;
    picture_num_ = 0;

    object_builder_.reset(new apollo::perception::MinBoxObjectBuilder);
    object_builder_radar.reset(new apollo::perception::MinBoxObjectBuilder);


    timestamp_ = 0.0;
    timestamp_radar = 0.0;

    seq_num_ = 0;
    seq_num_radar = 0;

    tracker_.reset(new apollo::perception::HmObjectTracker(node_));
    tracker_->Init();


    tracker_radar.reset(new apollo::perception::HmObjectTracker(node_));
    tracker_radar->Init();
}

void lidar_perception::PclCallback(sensor_msgs::msg::PointCloud2::ConstSharedPtr rec) {
    if (rec->width * rec->height > 0) {
        clock_t time_begin = clock();

        // 将 PointCloud2 转换为 PCL 点云
        pcl::PointCloud<pcl::PointXYZI>::Ptr rec_point(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::fromROSMsg(*rec, *rec_point);

        // 聚类
        std::vector<pcl::PointIndices> clusters;
        cluster_.cluster_vector(rec_point, clusters);
        int counter = clusters.size();
        float intensity = 50.0;
        std::vector<pcl::PointCloud<pcl::PointXYZI>::Ptr> points_vector;
        
        if (counter > 0) {
            points_vector.clear();
            for (int i = 0; i < counter; i++) {
                pcl::PointCloud<pcl::PointXYZI>::Ptr temp_cluster(new pcl::PointCloud<pcl::PointXYZI>);
                pcl::copyPointCloud(*rec_point, clusters[i], *temp_cluster);
                int points_size = temp_cluster->size();

                for (int i = 0; i < points_size; i++) {
                    temp_cluster->at(i).intensity = intensity;
                }
                intensity += 50;
                points_vector.push_back(temp_cluster);
            }
        }
        std::cout << "cluster done !" << std::endl;

        // 显示聚类点云
        pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cluster_ptr(new pcl::PointCloud<pcl::PointXYZI>);
        for (int i = 0; i < counter; i++) {
            *pcl_cluster_ptr += *points_vector[i];
        }
        sensor_msgs::msg::PointCloud2 pcl_cluster_msg;
        pcl::toROSMsg(*pcl_cluster_ptr, pcl_cluster_msg);
        pcl_cluster_msg.header.stamp = rec->header.stamp;
        pcl_cluster_msg.header.frame_id = "velodyne";
        pub_pcl_cluster_->publish(pcl_cluster_msg);

        // 生成最小包围盒
        std::vector<std::shared_ptr<apollo::perception::Object>> objects;
        for (int i = 0; i < counter; i++) {

            std::shared_ptr<apollo::perception::Object> out_obj(new apollo::perception::Object);
            out_obj->cloud = points_vector[i];
            objects.push_back(out_obj);
        }
        if (object_builder_!= nullptr) {
            apollo::perception::ObjectBuilderOptions object_builder_options;
            if (!object_builder_->Build(object_builder_options, &objects)) {
                return;
            }
        }
        std::cout << "minbox done !" << std::endl;

        // 跟踪
        std::shared_ptr<apollo::perception::SensorObjects> out_sensor_objects(new apollo::perception::SensorObjects);
        if (objects.size() > 0) {
            const double kTimeStamp = rclcpp::Time(rec->header.stamp).seconds();
            timestamp_ = kTimeStamp;
            ++seq_num_;
            std::shared_ptr<Eigen::Matrix4d> velodyne_trans = std::make_shared<Eigen::Matrix4d>();
            *velodyne_trans << 1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1;
            out_sensor_objects->timestamp = timestamp_;
            out_sensor_objects->sensor2world_pose = *velodyne_trans;
            out_sensor_objects->seq_num = seq_num_;
            if (tracker_ != nullptr) {
                apollo::perception::TrackerOptions tracker_options;
                tracker_options.velodyne_trans = velodyne_trans;
                if (!tracker_->Track(objects, timestamp_, tracker_options, &(out_sensor_objects->objects))) {
                    std::cout << "tracker running error !" << std::endl;
                }
            }
            std::cout << "tracked size = " << out_sensor_objects->objects.size() << std::endl;
            // Remove objects without points
            out_sensor_objects->objects.erase(
                std::remove_if(
                    out_sensor_objects->objects.begin(),
                    out_sensor_objects->objects.end(),
                    [](const std::shared_ptr<apollo::perception::Object>& obj) {
                        return obj->cloud->empty();
                    }),
                out_sensor_objects->objects.end());
        }
        std::cout << "tracker done !" << std::endl;

        // 显示标记
        if (out_sensor_objects->objects.size() > 0) {
            ClearAllMarker(rec->header.stamp);
            visualization_msgs::msg::MarkerArray marker_array_box, marker_array_info, marker_array_track;

            visualization_msgs::msg::Marker line_strip;
            line_strip.header.frame_id = "velodyne";
            line_strip.header.stamp =rec->header.stamp;
            line_strip.ns = "apollo::perception";
            line_strip.type = visualization_msgs::msg::Marker::LINE_STRIP;
            line_strip.scale.x = 0.01;
            line_strip.color.r = 0.0;
            line_strip.color.g = 1.0;
            line_strip.color.b = 0.0;
            line_strip.color.a = 1.0;
            line_strip.points.resize(5);
            int marker_id = 0;
            int trackerd_num = out_sensor_objects->objects.size();
            max_marker_size_=trackerd_num;
            for (int i = 0; i < trackerd_num; i++) {
                geometry_msgs::msg::Point p1, p2, p3, p4, p5, p6, p7, p8;
                p1.x = p5.x = out_sensor_objects->objects[i]->vertex1[0];
                p1.y = p5.y = out_sensor_objects->objects[i]->vertex1[1];
                p1.z = out_sensor_objects->objects[i]->min_height;
                p5.z = out_sensor_objects->objects[i]->max_height;
                p2.x = p6.x = out_sensor_objects->objects[i]->vertex2[0];
                p2.y = p6.y = out_sensor_objects->objects[i]->vertex2[1];
                p2.z = out_sensor_objects->objects[i]->min_height;
                p6.z = out_sensor_objects->objects[i]->max_height;
                p3.x = p7.x = out_sensor_objects->objects[i]->vertex3[0];
                p3.y = p7.y = out_sensor_objects->objects[i]->vertex3[1];
                p3.z = out_sensor_objects->objects[i]->min_height;
                p7.z = out_sensor_objects->objects[i]->max_height;
                p4.x = p8.x = out_sensor_objects->objects[i]->vertex4[0];
                p4.y = p8.y = out_sensor_objects->objects[i]->vertex4[1];
                p4.z = out_sensor_objects->objects[i]->min_height;
                p8.z = out_sensor_objects->objects[i]->max_height;
                line_strip.id = marker_id;
                line_strip.points[0] = p1;
                line_strip.points[1] = p2;
                line_strip.points[2] = p4;
                line_strip.points[3] = p3;
                line_strip.points[4] = p1;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p5;
                line_strip.points[1] = p6;
                line_strip.points[2] = p8;
                line_strip.points[3] = p7;
                line_strip.points[4] = p5;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p1;
                line_strip.points[1] = p5;
                line_strip.points[2] = p7;
                line_strip.points[3] = p3;
                line_strip.points[4] = p1;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p2;
                line_strip.points[1] = p6;
                line_strip.points[2] = p8;
                line_strip.points[3] = p4;
                line_strip.points[4] = p2;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
            }

            visualization_msgs::msg::Marker marker_txt;
            marker_txt.header.frame_id = "velodyne";
            marker_txt.header.stamp =rec->header.stamp;
            marker_txt.ns = "apollo::perception";
            marker_txt.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
            marker_txt.scale.z = 0.1;
            marker_txt.color.r = 1.0;
            marker_txt.color.g = 1.0;
            marker_txt.color.b = 1.0;
            marker_txt.color.a = 1.0;
            ros_plc::msg::LidarMsgArray tracked_array;
            tracked_array.header = rec->header;
            tracked_array.header.frame_id = "velodyne";
            for (int i = 0; i < trackerd_num; i++) {
                marker_txt.pose.position.x = out_sensor_objects->objects[i]->anchor_point[0];
                marker_txt.pose.position.y = out_sensor_objects->objects[i]->anchor_point[1];
                marker_txt.pose.position.z = out_sensor_objects->objects[i]->max_height + 0.3;
                marker_txt.id = i;
                const double speed = std::sqrt(
                    X2(out_sensor_objects->objects[i]->velocity[0]) +
                    X2(out_sensor_objects->objects[i]->velocity[1]));

                marker_txt.text = std::string("id: ") + std::to_string(out_sensor_objects->objects[i]->track_id) + "\n";
                marker_txt.text += std::string("speed: ") + std::to_string(speed) + " m/s\n";
                marker_txt.text += std::string("age: ") + std::to_string(static_cast<int>(out_sensor_objects->objects[i]->tracking_time)) + "s\n";
                marker_array_info.markers.push_back(marker_txt);

                ros_plc::msg::LidarMsg object;
                object.current_id = out_sensor_objects->objects[i]->track_id;
                object.original_id = out_sensor_objects->objects[i]->track_id;
                object.x = static_cast<float>(out_sensor_objects->objects[i]->anchor_point[0]);
                object.y = static_cast<float>(out_sensor_objects->objects[i]->anchor_point[1]);
                object.z = static_cast<float>(out_sensor_objects->objects[i]->max_height + 0.3);
                object.speed = static_cast<float>(speed);
                object.timestamp = rec->header.stamp;
                tracked_array.objects.push_back(object);
            }

            if (!tracked_array.objects.empty()) {
                lidar_tracked_objects_pub_->publish(tracked_array);
            }

            for (int i = 0; i < trackerd_num; i++) {
                if (out_sensor_objects->objects[i]->drops.size() > 0) {
                    visualization_msgs::msg::Marker marker_track;
                    marker_track.header.frame_id = "velodyne";
                    marker_track.header.stamp =rec->header.stamp;
                    marker_track.ns = "apollo::perception";
                    marker_track.type = visualization_msgs::msg::Marker::LINE_STRIP;
                    marker_track.scale.x = 0.01;
                    marker_track.color.g = (1.0 / trackerd_num) * i;
                    marker_track.color.a = 1.0;
                    marker_track.id = i;
                    for (auto p : out_sensor_objects->objects[i]->drops) {
                        geometry_msgs::msg::Point p1;
                        p1.x = p[0];
                        p1.y = p[1];
                        p1.z = p[2];
                        marker_track.points.push_back(p1);
                    }
                    marker_array_track.markers.push_back(marker_track);
                }
            }

            marker_pub_box_->publish(marker_array_box);
            marker_pub_tracinfo_->publish(marker_array_info);
            marker_pub_track_->publish(marker_array_track);
        }
        clock_t time_end = clock();
        std::cout << "all time = " << (double)(time_end - time_begin) / CLOCKS_PER_SEC << std::endl;
    }
}
void lidar_perception::PclCallback_radar(sensor_msgs::msg::PointCloud2::ConstSharedPtr rec) {
    if (rec->width * rec->height > 0) {
        clock_t time_begin = clock();

        // 将 PointCloud2 转换为 PCL 点云
        pcl::PointCloud<pcl::PointXYZI>::Ptr rec_point(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::fromROSMsg(*rec, *rec_point);
        // 解析velocity信息
        std::vector<float> velocities;
        sensor_msgs::PointCloud2ConstIterator<float> iter_vel(*rec, "velocity");
        for (; iter_vel != iter_vel.end(); ++iter_vel) {
            velocities.push_back(*iter_vel);
        }

         // 聚类
        std::vector<pcl::PointIndices> clusters;
        cluster_.cluster_vector(rec_point, clusters);
        int counter = clusters.size();
        float intensity = 50.0;
        std::vector<pcl::PointCloud<pcl::PointXYZI>::Ptr> points_vector;
        std::vector<std::vector<float>> cluster_velocities; // 存储每个聚类的velocity信息
        std::vector<std::vector<float>> cluster_x;
        std::vector<std::vector<float>> cluster_y;
        if (counter > 0) {
            points_vector.clear();
            cluster_velocities.clear();
            cluster_x.clear();
            cluster_y.clear();
            for (int i = 0; i < counter; i++) {
                pcl::PointCloud<pcl::PointXYZI>::Ptr temp_cluster(new pcl::PointCloud<pcl::PointXYZI>);
                pcl::copyPointCloud(*rec_point, clusters[i], *temp_cluster);
                std::vector<float> temp_velocities; // 当前聚类的velocity信息
                int points_size = temp_cluster->size();
                std::vector<float> temp_x; // 当前聚类的x信息
                std::vector<float> temp_y; // 当前聚类的y信息
                for (int j = 0; j < points_size; j++) {
                    temp_cluster->at(j).intensity = intensity;
                    temp_velocities.push_back(velocities[clusters[i].indices[j]]); // 对应的velocity
                    temp_x.push_back(velocities[clusters[i].indices[j]]);
                    temp_y.push_back(velocities[clusters[i].indices[j]]);
                }
                intensity += 50;
                points_vector.push_back(temp_cluster);
                cluster_velocities.push_back(temp_velocities); // 保存当前聚类的velocity信息
                cluster_x.push_back(temp_x);
                cluster_y.push_back(temp_y);
            }
        }
        std::cout << "cluster done !" << std::endl;

        // 显示聚类点云
        pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cluster_ptr(new pcl::PointCloud<pcl::PointXYZI>);
        for (int i = 0; i < counter; i++) {
            *pcl_cluster_ptr += *points_vector[i];
        }
        sensor_msgs::msg::PointCloud2 pcl_cluster_msg;
        pcl::toROSMsg(*pcl_cluster_ptr, pcl_cluster_msg);
        pcl_cluster_msg.header.frame_id = "velodyne";
        pub_pcl_cluster_radar->publish(pcl_cluster_msg);

        // 生成最小包围盒
        std::vector<std::shared_ptr<apollo::perception::Object>> objects;
        std::map<int, std::pair<float, float>> cluster_avg_coords; 
        std::map<int, float> cluster_velocity_map; 

        for (int i = 0; i < counter; i++) {
            std::shared_ptr<apollo::perception::Object> out_obj(new apollo::perception::Object);
            out_obj->cloud = points_vector[i];
            // 计算聚类的平均velocity
            float avg_velocity = 0.0;
            if (!cluster_velocities[i].empty()) {
                for (float vel : cluster_velocities[i]) {
                    avg_velocity += vel;
                }
                avg_velocity /= cluster_velocities[i].size(); // 确保这里不会除以零
            } else {
                avg_velocity = 0.0; // 如果没有速度信息，设为0
            }
            //out_obj->original_velocity = avg_velocity; // 这里应该是实际的平均速度
            
            // 计算聚类的平均x, y坐标
            float avg_x = 0.0, avg_y = 0.0;
            int points_size = points_vector[i]->points.size();
            for (int j = 0; j < points_size; j++) {
                avg_x += points_vector[i]->points[j].x;
                avg_y += points_vector[i]->points[j].y;
            }
            if (points_size > 0) {
                avg_x /= points_size;
                avg_y /= points_size;
            }
            // 将 original_velocity 和 (avg_x, avg_y) 存入 map
             cluster_avg_coords[i] = std::make_pair(avg_x, avg_y);
             cluster_velocity_map[i] =avg_velocity;

            objects.push_back(out_obj);
        }
        if (object_builder_radar != nullptr) {
            apollo::perception::ObjectBuilderOptions object_builder_options;
            if (!object_builder_radar->Build(object_builder_options, &objects)) {
                return;
            }
        }
        std::cout << "minbox done !" << std::endl;

        // 跟踪
        std::shared_ptr<apollo::perception::SensorObjects> out_sensor_objects(new apollo::perception::SensorObjects);
        if (objects.size() > 0) {
            const double kTimeStamp = rclcpp::Time(rec->header.stamp).seconds();
            timestamp_radar = kTimeStamp;
            ++seq_num_radar;
            std::shared_ptr<Eigen::Matrix4d> velodyne_trans = std::make_shared<Eigen::Matrix4d>();
            *velodyne_trans << 1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1;
            out_sensor_objects->timestamp = timestamp_radar;
            out_sensor_objects->sensor2world_pose = *velodyne_trans;
            out_sensor_objects->seq_num = seq_num_radar;
            if (tracker_radar != nullptr) {
                apollo::perception::TrackerOptions tracker_options;
                tracker_options.velodyne_trans = velodyne_trans;
                if (!tracker_radar->Track(objects, timestamp_radar, tracker_options, &(out_sensor_objects->objects))) {
                    std::cout << "tracker running error !" << std::endl;
                }
            }
            std::cout << "tracked size = " << out_sensor_objects->objects.size() << std::endl;
            // Remove objects without points
            out_sensor_objects->objects.erase(
                std::remove_if(
                    out_sensor_objects->objects.begin(),
                    out_sensor_objects->objects.end(),
                    [](const std::shared_ptr<apollo::perception::Object>& obj) {
                        return obj->cloud->empty();
                    }),
                out_sensor_objects->objects.end());
        }
        std::cout << "tracker done !" << std::endl;

        // 显示标记
        if (out_sensor_objects->objects.size() > 0) {
            ClearAllMarker_radar(rec->header.stamp);
            visualization_msgs::msg::MarkerArray marker_array_box, marker_array_info, marker_array_track;

            visualization_msgs::msg::Marker line_strip;
            line_strip.header.frame_id = "velodyne";
            line_strip.header.stamp =rec->header.stamp;
            line_strip.ns = "apollo::perception";
            line_strip.type = visualization_msgs::msg::Marker::LINE_STRIP;
            line_strip.scale.x = 0.01;
            line_strip.color.r = 0.0;
            line_strip.color.g = 1.0;
            line_strip.color.b = 0.0;
            line_strip.color.a = 1.0;
            line_strip.points.resize(5);
            int marker_id = 0;
            int trackerd_num = out_sensor_objects->objects.size();
            max_marker_size_radar=trackerd_num;
            for (int i = 0; i < trackerd_num; i++) {
                geometry_msgs::msg::Point p1, p2, p3, p4, p5, p6, p7, p8;
                p1.x = p5.x = out_sensor_objects->objects[i]->vertex1[0];
                p1.y = p5.y = out_sensor_objects->objects[i]->vertex1[1];
                p1.z = out_sensor_objects->objects[i]->min_height;
                p5.z = out_sensor_objects->objects[i]->max_height;
                p2.x = p6.x = out_sensor_objects->objects[i]->vertex2[0];
                p2.y = p6.y = out_sensor_objects->objects[i]->vertex2[1];
                p2.z = out_sensor_objects->objects[i]->min_height;
                p6.z = out_sensor_objects->objects[i]->max_height;
                p3.x = p7.x = out_sensor_objects->objects[i]->vertex3[0];
                p3.y = p7.y = out_sensor_objects->objects[i]->vertex3[1];
                p3.z = out_sensor_objects->objects[i]->min_height;
                p7.z = out_sensor_objects->objects[i]->max_height;
                p4.x = p8.x = out_sensor_objects->objects[i]->vertex4[0];
                p4.y = p8.y = out_sensor_objects->objects[i]->vertex4[1];
                p4.z = out_sensor_objects->objects[i]->min_height;
                p8.z = out_sensor_objects->objects[i]->max_height;
                line_strip.id = marker_id;
                line_strip.points[0] = p1;
                line_strip.points[1] = p2;
                line_strip.points[2] = p4;
                line_strip.points[3] = p3;
                line_strip.points[4] = p1;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p5;
                line_strip.points[1] = p6;
                line_strip.points[2] = p8;
                line_strip.points[3] = p7;
                line_strip.points[4] = p5;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p1;
                line_strip.points[1] = p5;
                line_strip.points[2] = p7;
                line_strip.points[3] = p3;
                line_strip.points[4] = p1;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
                line_strip.id = marker_id;
                line_strip.points[0] = p2;
                line_strip.points[1] = p6;
                line_strip.points[2] = p8;
                line_strip.points[3] = p4;
                line_strip.points[4] = p2;
                marker_array_box.markers.push_back(line_strip);
                marker_id++;
            }

            visualization_msgs::msg::Marker marker_txt;
            marker_txt.header.frame_id = "velodyne";
            marker_txt.header.stamp =rec->header.stamp;
            marker_txt.ns = "apollo::perception";
            marker_txt.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
            marker_txt.scale.z = 0.1;
            marker_txt.color.r = 1.0;
            marker_txt.color.g = 1.0;
            marker_txt.color.b = 1.0;
            marker_txt.color.a = 1.0;
            ros_plc::msg::RadarMsgArray tracked_array;
            tracked_array.header = rec->header;
            tracked_array.header.frame_id = "velodyne";
            for (int i = 0; i < trackerd_num; i++) {
                marker_txt.pose.position.x = out_sensor_objects->objects[i]->anchor_point[0];
                marker_txt.pose.position.y = out_sensor_objects->objects[i]->anchor_point[1];
                marker_txt.pose.position.z = out_sensor_objects->objects[i]->max_height + 0.3;
                marker_txt.id = i;

                // 找到与当前位置最近的聚类
                float min_distance = std::numeric_limits<float>::max();
                int closest_id = -1;

                for (const auto &entry : cluster_avg_coords) {
                    float dist = std::hypot(
                        entry.second.first - marker_txt.pose.position.x,
                        entry.second.second - marker_txt.pose.position.y);
                    if (dist < min_distance) {
                        min_distance = dist;
                        closest_id = entry.first;
                    }
                }

                float radar_speed = 0.0F;
                const auto velocity_it = cluster_velocity_map.find(closest_id);
                if (velocity_it != cluster_velocity_map.end()) {
                    radar_speed = velocity_it->second;
                }

                marker_txt.text = std::string("id: ") + std::to_string(out_sensor_objects->objects[i]->track_id) + "\n";
                marker_txt.text += std::string("speed: ") + std::to_string(radar_speed) + " m/s\n";
                marker_txt.text += std::string("age: ") + std::to_string(static_cast<int>(out_sensor_objects->objects[i]->tracking_time)) + "s\n";
                marker_array_info.markers.push_back(marker_txt);

                ros_plc::msg::RadarMsg object;
                object.current_id = out_sensor_objects->objects[i]->track_id;
                object.original_id = out_sensor_objects->objects[i]->track_id;
                object.x = static_cast<float>(out_sensor_objects->objects[i]->anchor_point[0]);
                object.y = static_cast<float>(out_sensor_objects->objects[i]->anchor_point[1]);
                object.phi = std::atan2(object.y, object.x);
                object.speed = radar_speed;
                object.timestamp = rec->header.stamp;
                tracked_array.objects.push_back(object);
            }

            if (!tracked_array.objects.empty()) {
                radar_tracked_objects_pub_->publish(tracked_array);
            }

            for (int i = 0; i < trackerd_num; i++) {
                if (out_sensor_objects->objects[i]->drops.size() > 0) {
                    visualization_msgs::msg::Marker marker_track;
                    marker_track.header.frame_id = "velodyne";
                    marker_track.header.stamp =rec->header.stamp;
                    marker_track.ns = "apollo::perception";
                    marker_track.type = visualization_msgs::msg::Marker::LINE_STRIP;
                    marker_track.scale.x = 0.01;
                    marker_track.color.g = (1.0 / trackerd_num) * i;
                    marker_track.color.a = 1.0;
                    marker_track.id = i;
                    for (auto p : out_sensor_objects->objects[i]->drops) {
                        geometry_msgs::msg::Point p1;
                        p1.x = p[0];
                        p1.y = p[1];
                        p1.z = p[2];
                        marker_track.points.push_back(p1);
                    }
                    marker_array_track.markers.push_back(marker_track);
                }
            }

            marker_pub_box_radar->publish(marker_array_box);
            marker_pub_tracinfo_radar->publish(marker_array_info);
            marker_pub_track_radar->publish(marker_array_track);
        }
        clock_t time_end = clock();
        std::cout << "all time = " << (double)(time_end - time_begin) / CLOCKS_PER_SEC << std::endl;
    }
}

void lidar_perception::ClearAllMarker(const builtin_interfaces::msg::Time & timestamp) {
    visualization_msgs::msg::MarkerArray delete_marker_array;
    visualization_msgs::msg::MarkerArray delete_marker_array_box;
    int marker_box_id = 0;  // Box类型的Marker ID计数器
    for (int i = 0; i < max_marker_size_; i++) {
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = "velodyne";
        marker.header.stamp =timestamp;
        marker.ns = "apollo::perception";
        marker.id = i;
        marker.action = visualization_msgs::msg::Marker::DELETE;
        delete_marker_array.markers.push_back(marker);
        // Box类型Marker清除（每个物体分配4个ID）
        for (int j = 0; j < 4; j++) {  // 每个物体4个box Marker
            visualization_msgs::msg::Marker marker_box;
            marker_box.header.frame_id = "velodyne";
            marker_box.header.stamp = timestamp;
            marker_box.ns = "apollo::perception";
            marker_box.id = marker_box_id++;
            marker_box.action = visualization_msgs::msg::Marker::DELETE;
            delete_marker_array_box.markers.push_back(marker_box);
        }
    }
    marker_pub_box_->publish(delete_marker_array_box);
    marker_pub_tracinfo_->publish(delete_marker_array);
    marker_pub_track_->publish(delete_marker_array);
    
}
void lidar_perception::ClearAllMarker_radar(const builtin_interfaces::msg::Time & timestamp) {
    visualization_msgs::msg::MarkerArray delete_marker_array;
    visualization_msgs::msg::MarkerArray delete_marker_array_box;
    int marker_box_id = 0;  // Box类型的Marker ID计数器
    for (int i = 0; i < max_marker_size_radar; i++) {
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = "velodyne";
        marker.header.stamp =timestamp;
        marker.ns = "apollo::perception";
        marker.id = i;
        marker.action = visualization_msgs::msg::Marker::DELETE;
        delete_marker_array.markers.push_back(marker);

        // Box类型Marker清除（每个物体分配4个ID）
        for (int j = 0; j < 4; j++) {  // 每个物体4个box Marker
            visualization_msgs::msg::Marker marker_box;
            marker_box.header.frame_id = "velodyne";
            marker_box.header.stamp = timestamp;
            marker_box.ns = "apollo::perception";
            marker_box.id = marker_box_id++;
            marker_box.action = visualization_msgs::msg::Marker::DELETE;
            delete_marker_array_box.markers.push_back(marker_box);
        }
    }
    marker_pub_box_radar->publish(delete_marker_array_box);
    marker_pub_tracinfo_radar->publish(delete_marker_array);
    marker_pub_track_radar->publish(delete_marker_array);
}

} // namespace lidar_perception
