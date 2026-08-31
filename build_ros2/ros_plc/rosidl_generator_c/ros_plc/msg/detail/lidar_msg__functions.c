// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ros_plc:msg/LidarMsg.idl
// generated code does not contain a copyright notice
#include "ros_plc/msg/detail/lidar_msg__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `timestamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
ros_plc__msg__LidarMsg__init(ros_plc__msg__LidarMsg * msg)
{
  if (!msg) {
    return false;
  }
  // current_id
  // original_id
  // x
  // y
  // z
  // speed
  // timestamp
  if (!builtin_interfaces__msg__Time__init(&msg->timestamp)) {
    ros_plc__msg__LidarMsg__fini(msg);
    return false;
  }
  return true;
}

void
ros_plc__msg__LidarMsg__fini(ros_plc__msg__LidarMsg * msg)
{
  if (!msg) {
    return;
  }
  // current_id
  // original_id
  // x
  // y
  // z
  // speed
  // timestamp
  builtin_interfaces__msg__Time__fini(&msg->timestamp);
}

bool
ros_plc__msg__LidarMsg__are_equal(const ros_plc__msg__LidarMsg * lhs, const ros_plc__msg__LidarMsg * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // current_id
  if (lhs->current_id != rhs->current_id) {
    return false;
  }
  // original_id
  if (lhs->original_id != rhs->original_id) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // z
  if (lhs->z != rhs->z) {
    return false;
  }
  // speed
  if (lhs->speed != rhs->speed) {
    return false;
  }
  // timestamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->timestamp), &(rhs->timestamp)))
  {
    return false;
  }
  return true;
}

bool
ros_plc__msg__LidarMsg__copy(
  const ros_plc__msg__LidarMsg * input,
  ros_plc__msg__LidarMsg * output)
{
  if (!input || !output) {
    return false;
  }
  // current_id
  output->current_id = input->current_id;
  // original_id
  output->original_id = input->original_id;
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // z
  output->z = input->z;
  // speed
  output->speed = input->speed;
  // timestamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->timestamp), &(output->timestamp)))
  {
    return false;
  }
  return true;
}

ros_plc__msg__LidarMsg *
ros_plc__msg__LidarMsg__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__LidarMsg * msg = (ros_plc__msg__LidarMsg *)allocator.allocate(sizeof(ros_plc__msg__LidarMsg), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ros_plc__msg__LidarMsg));
  bool success = ros_plc__msg__LidarMsg__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ros_plc__msg__LidarMsg__destroy(ros_plc__msg__LidarMsg * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ros_plc__msg__LidarMsg__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ros_plc__msg__LidarMsg__Sequence__init(ros_plc__msg__LidarMsg__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__LidarMsg * data = NULL;

  if (size) {
    data = (ros_plc__msg__LidarMsg *)allocator.zero_allocate(size, sizeof(ros_plc__msg__LidarMsg), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ros_plc__msg__LidarMsg__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ros_plc__msg__LidarMsg__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
ros_plc__msg__LidarMsg__Sequence__fini(ros_plc__msg__LidarMsg__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      ros_plc__msg__LidarMsg__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

ros_plc__msg__LidarMsg__Sequence *
ros_plc__msg__LidarMsg__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__LidarMsg__Sequence * array = (ros_plc__msg__LidarMsg__Sequence *)allocator.allocate(sizeof(ros_plc__msg__LidarMsg__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ros_plc__msg__LidarMsg__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ros_plc__msg__LidarMsg__Sequence__destroy(ros_plc__msg__LidarMsg__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ros_plc__msg__LidarMsg__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ros_plc__msg__LidarMsg__Sequence__are_equal(const ros_plc__msg__LidarMsg__Sequence * lhs, const ros_plc__msg__LidarMsg__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ros_plc__msg__LidarMsg__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ros_plc__msg__LidarMsg__Sequence__copy(
  const ros_plc__msg__LidarMsg__Sequence * input,
  ros_plc__msg__LidarMsg__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ros_plc__msg__LidarMsg);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ros_plc__msg__LidarMsg * data =
      (ros_plc__msg__LidarMsg *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ros_plc__msg__LidarMsg__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ros_plc__msg__LidarMsg__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ros_plc__msg__LidarMsg__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
