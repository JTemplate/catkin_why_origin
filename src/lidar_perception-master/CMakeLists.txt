cmake_minimum_required(VERSION 3.8)
project(lidar_perception)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ros_plc REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(lidar_perception_node
  src/lidar_perception_node.cpp
  src/lidar_perception.cpp
  src/pcl_clip/pcl_clip.cpp
  src/remove_ground/remove_ground.cpp
  src/cluster/region_growing_segmentation.cpp
  src/min_box/min_box.cc
  src/cluster/EuclideanCluster.cpp
  src/cluster/dbscan.cpp
  include/lidar_perception/object.cc
  src/hm_tracker/hm_tracker.cc
  src/hm_tracker/feature_descriptor.cc
  src/hm_tracker/hungarian_matcher.cc
  src/hm_tracker/kalman_filter.cc
  src/hm_tracker/object_track.cc
  src/hm_tracker/track_object_distance.cc
  src/hm_tracker/tracked_object.cc
  include/lidar_perception/hungarian/hungarian_bigraph_matcher.cc
  include/lidar_perception/common/graph_util.cc
  include/lidar_perception/common/geometry_util.cc
)
target_include_directories(lidar_perception_node PRIVATE
  include
  src
  ${PCL_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)
target_link_libraries(lidar_perception_node ${PCL_LIBRARIES} ${OpenCV_LIBRARIES})
ament_target_dependencies(lidar_perception_node
  rclcpp ros_plc builtin_interfaces sensor_msgs std_msgs geometry_msgs visualization_msgs pcl_conversions
)

install(TARGETS lidar_perception_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch params urdf DESTINATION share/${PROJECT_NAME})

ament_package()
