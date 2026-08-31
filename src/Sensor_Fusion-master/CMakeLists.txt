cmake_minimum_required(VERSION 3.8)
project(sensor_fusion)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(message_filters REQUIRED)
find_package(ros_plc REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/FusionData.msg"
  "msg/SourceData.msg"
)

add_executable(SensorFusion
  src/main.cpp
  src/sensorfusion.cpp
  src/kalmanfilter.cpp
)
target_include_directories(SensorFusion PRIVATE src)
ament_target_dependencies(SensorFusion
  rclcpp message_filters ros_plc std_msgs sensor_msgs visualization_msgs
)

install(TARGETS SensorFusion DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(sensor_fusion_timestamp_test
    test/sensor_fusion_timestamp_test.cpp
    src/sensorfusion.cpp
    src/kalmanfilter.cpp
  )
  target_include_directories(sensor_fusion_timestamp_test PRIVATE src)
  ament_target_dependencies(sensor_fusion_timestamp_test rclcpp)
endif()

ament_export_dependencies(rosidl_default_runtime)
ament_package()
