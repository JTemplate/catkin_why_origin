cmake_minimum_required(VERSION 3.8)
project(ros_plc)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS common filters)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/LidarMsg.msg"
  "msg/LidarMsgArray.msg"
  "msg/RadarMsg.msg"
  "msg/RadarMsgArray.msg"
  DEPENDENCIES builtin_interfaces std_msgs
)

set(FILTER_NODES
  voxel_grid_filter
  ground_cut
  x_cut
  y_cut
  mmwave_filter
)
foreach(target IN LISTS FILTER_NODES)
  add_executable(${target} src/${target}.cpp)
  target_include_directories(${target} PRIVATE ${PCL_INCLUDE_DIRS})
  target_link_libraries(${target} ${PCL_LIBRARIES})
  ament_target_dependencies(${target} rclcpp sensor_msgs pcl_conversions)
endforeach()

set(MESSAGE_NODES radarMap lidarMap)
foreach(target IN LISTS MESSAGE_NODES)
  add_executable(${target} src/${target}.cpp)
  ament_target_dependencies(${target} rclcpp visualization_msgs)
  rosidl_get_typesupport_target(typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
  target_link_libraries(${target} "${typesupport_target}")
endforeach()

install(TARGETS ${FILTER_NODES} ${MESSAGE_NODES}
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_export_dependencies(rosidl_default_runtime)
ament_package()
