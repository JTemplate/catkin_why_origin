// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros_plc:msg/LidarMsgArray.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__STRUCT_H_
#define ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'objects'
#include "ros_plc/msg/detail/lidar_msg__struct.h"

/// Struct defined in msg/LidarMsgArray in the package ros_plc.
typedef struct ros_plc__msg__LidarMsgArray
{
  std_msgs__msg__Header header;
  ros_plc__msg__LidarMsg__Sequence objects;
} ros_plc__msg__LidarMsgArray;

// Struct for a sequence of ros_plc__msg__LidarMsgArray.
typedef struct ros_plc__msg__LidarMsgArray__Sequence
{
  ros_plc__msg__LidarMsgArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros_plc__msg__LidarMsgArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__STRUCT_H_
