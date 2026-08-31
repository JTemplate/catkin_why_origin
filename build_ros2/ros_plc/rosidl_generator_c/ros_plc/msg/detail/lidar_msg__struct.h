// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros_plc:msg/LidarMsg.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__LIDAR_MSG__STRUCT_H_
#define ROS_PLC__MSG__DETAIL__LIDAR_MSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'timestamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/LidarMsg in the package ros_plc.
typedef struct ros_plc__msg__LidarMsg
{
  int32_t current_id;
  int32_t original_id;
  float x;
  float y;
  float z;
  float speed;
  builtin_interfaces__msg__Time timestamp;
} ros_plc__msg__LidarMsg;

// Struct for a sequence of ros_plc__msg__LidarMsg.
typedef struct ros_plc__msg__LidarMsg__Sequence
{
  ros_plc__msg__LidarMsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros_plc__msg__LidarMsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS_PLC__MSG__DETAIL__LIDAR_MSG__STRUCT_H_
