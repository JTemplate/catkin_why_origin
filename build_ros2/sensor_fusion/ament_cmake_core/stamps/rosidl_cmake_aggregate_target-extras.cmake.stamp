# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target sensor_fusion::sensor_fusion
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${sensor_fusion_TARGETS}.
if(sensor_fusion_TARGETS AND NOT TARGET sensor_fusion::sensor_fusion)
  add_library(sensor_fusion::sensor_fusion INTERFACE IMPORTED)
  set_target_properties(sensor_fusion::sensor_fusion PROPERTIES
    INTERFACE_LINK_LIBRARIES "${sensor_fusion_TARGETS}")
endif()
