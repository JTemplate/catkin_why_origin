# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target ros_plc::ros_plc
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${ros_plc_TARGETS}.
if(ros_plc_TARGETS AND NOT TARGET ros_plc::ros_plc)
  add_library(ros_plc::ros_plc INTERFACE IMPORTED)
  set_target_properties(ros_plc::ros_plc PROPERTIES
    INTERFACE_LINK_LIBRARIES "${ros_plc_TARGETS}")
endif()
