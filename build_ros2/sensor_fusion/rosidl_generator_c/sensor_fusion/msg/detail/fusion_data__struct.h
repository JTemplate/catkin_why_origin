// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sensor_fusion:msg/FusionData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_H_
#define SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/FusionData in the package sensor_fusion.
typedef struct sensor_fusion__msg__FusionData
{
  double x;
  double y;
  double vx;
  double vy;
} sensor_fusion__msg__FusionData;

// Struct for a sequence of sensor_fusion__msg__FusionData.
typedef struct sensor_fusion__msg__FusionData__Sequence
{
  sensor_fusion__msg__FusionData * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sensor_fusion__msg__FusionData__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_H_
