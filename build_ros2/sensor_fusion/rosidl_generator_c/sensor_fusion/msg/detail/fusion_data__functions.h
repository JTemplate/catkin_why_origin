// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from sensor_fusion:msg/FusionData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__FUNCTIONS_H_
#define SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "sensor_fusion/msg/rosidl_generator_c__visibility_control.h"

#include "sensor_fusion/msg/detail/fusion_data__struct.h"

/// Initialize msg/FusionData message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * sensor_fusion__msg__FusionData
 * )) before or use
 * sensor_fusion__msg__FusionData__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__init(sensor_fusion__msg__FusionData * msg);

/// Finalize msg/FusionData message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
void
sensor_fusion__msg__FusionData__fini(sensor_fusion__msg__FusionData * msg);

/// Create msg/FusionData message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * sensor_fusion__msg__FusionData__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
sensor_fusion__msg__FusionData *
sensor_fusion__msg__FusionData__create();

/// Destroy msg/FusionData message.
/**
 * It calls
 * sensor_fusion__msg__FusionData__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
void
sensor_fusion__msg__FusionData__destroy(sensor_fusion__msg__FusionData * msg);

/// Check for msg/FusionData message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__are_equal(const sensor_fusion__msg__FusionData * lhs, const sensor_fusion__msg__FusionData * rhs);

/// Copy a msg/FusionData message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__copy(
  const sensor_fusion__msg__FusionData * input,
  sensor_fusion__msg__FusionData * output);

/// Initialize array of msg/FusionData messages.
/**
 * It allocates the memory for the number of elements and calls
 * sensor_fusion__msg__FusionData__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__Sequence__init(sensor_fusion__msg__FusionData__Sequence * array, size_t size);

/// Finalize array of msg/FusionData messages.
/**
 * It calls
 * sensor_fusion__msg__FusionData__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
void
sensor_fusion__msg__FusionData__Sequence__fini(sensor_fusion__msg__FusionData__Sequence * array);

/// Create array of msg/FusionData messages.
/**
 * It allocates the memory for the array and calls
 * sensor_fusion__msg__FusionData__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
sensor_fusion__msg__FusionData__Sequence *
sensor_fusion__msg__FusionData__Sequence__create(size_t size);

/// Destroy array of msg/FusionData messages.
/**
 * It calls
 * sensor_fusion__msg__FusionData__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
void
sensor_fusion__msg__FusionData__Sequence__destroy(sensor_fusion__msg__FusionData__Sequence * array);

/// Check for msg/FusionData message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__Sequence__are_equal(const sensor_fusion__msg__FusionData__Sequence * lhs, const sensor_fusion__msg__FusionData__Sequence * rhs);

/// Copy an array of msg/FusionData messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_sensor_fusion
bool
sensor_fusion__msg__FusionData__Sequence__copy(
  const sensor_fusion__msg__FusionData__Sequence * input,
  sensor_fusion__msg__FusionData__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__FUNCTIONS_H_
