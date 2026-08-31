// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ros_plc:msg/RadarMsg.idl
// generated code does not contain a copyright notice
#include "ros_plc/msg/detail/radar_msg__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `timestamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
ros_plc__msg__RadarMsg__init(ros_plc__msg__RadarMsg * msg)
{
  if (!msg) {
    return false;
  }
  // current_id
  // original_id
  // x
  // y
  // phi
  // speed
  // timestamp
  if (!builtin_interfaces__msg__Time__init(&msg->timestamp)) {
    ros_plc__msg__RadarMsg__fini(msg);
    return false;
  }
  return true;
}

void
ros_plc__msg__RadarMsg__fini(ros_plc__msg__RadarMsg * msg)
{
  if (!msg) {
    return;
  }
  // current_id
  // original_id
  // x
  // y
  // phi
  // speed
  // timestamp
  builtin_interfaces__msg__Time__fini(&msg->timestamp);
}

bool
ros_plc__msg__RadarMsg__are_equal(const ros_plc__msg__RadarMsg * lhs, const ros_plc__msg__RadarMsg * rhs)
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
  // phi
  if (lhs->phi != rhs->phi) {
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
ros_plc__msg__RadarMsg__copy(
  const ros_plc__msg__RadarMsg * input,
  ros_plc__msg__RadarMsg * output)
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
  // phi
  output->phi = input->phi;
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

ros_plc__msg__RadarMsg *
ros_plc__msg__RadarMsg__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsg * msg = (ros_plc__msg__RadarMsg *)allocator.allocate(sizeof(ros_plc__msg__RadarMsg), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ros_plc__msg__RadarMsg));
  bool success = ros_plc__msg__RadarMsg__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ros_plc__msg__RadarMsg__destroy(ros_plc__msg__RadarMsg * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ros_plc__msg__RadarMsg__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ros_plc__msg__RadarMsg__Sequence__init(ros_plc__msg__RadarMsg__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsg * data = NULL;

  if (size) {
    data = (ros_plc__msg__RadarMsg *)allocator.zero_allocate(size, sizeof(ros_plc__msg__RadarMsg), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ros_plc__msg__RadarMsg__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ros_plc__msg__RadarMsg__fini(&data[i - 1]);
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
ros_plc__msg__RadarMsg__Sequence__fini(ros_plc__msg__RadarMsg__Sequence * array)
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
      ros_plc__msg__RadarMsg__fini(&array->data[i]);
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

ros_plc__msg__RadarMsg__Sequence *
ros_plc__msg__RadarMsg__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsg__Sequence * array = (ros_plc__msg__RadarMsg__Sequence *)allocator.allocate(sizeof(ros_plc__msg__RadarMsg__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ros_plc__msg__RadarMsg__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ros_plc__msg__RadarMsg__Sequence__destroy(ros_plc__msg__RadarMsg__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ros_plc__msg__RadarMsg__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ros_plc__msg__RadarMsg__Sequence__are_equal(const ros_plc__msg__RadarMsg__Sequence * lhs, const ros_plc__msg__RadarMsg__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ros_plc__msg__RadarMsg__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ros_plc__msg__RadarMsg__Sequence__copy(
  const ros_plc__msg__RadarMsg__Sequence * input,
  ros_plc__msg__RadarMsg__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ros_plc__msg__RadarMsg);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ros_plc__msg__RadarMsg * data =
      (ros_plc__msg__RadarMsg *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ros_plc__msg__RadarMsg__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ros_plc__msg__RadarMsg__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ros_plc__msg__RadarMsg__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
