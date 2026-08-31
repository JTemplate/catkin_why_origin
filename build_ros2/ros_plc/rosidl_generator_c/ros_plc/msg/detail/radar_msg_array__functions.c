// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ros_plc:msg/RadarMsgArray.idl
// generated code does not contain a copyright notice
#include "ros_plc/msg/detail/radar_msg_array__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `objects`
#include "ros_plc/msg/detail/radar_msg__functions.h"

bool
ros_plc__msg__RadarMsgArray__init(ros_plc__msg__RadarMsgArray * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    ros_plc__msg__RadarMsgArray__fini(msg);
    return false;
  }
  // objects
  if (!ros_plc__msg__RadarMsg__Sequence__init(&msg->objects, 0)) {
    ros_plc__msg__RadarMsgArray__fini(msg);
    return false;
  }
  return true;
}

void
ros_plc__msg__RadarMsgArray__fini(ros_plc__msg__RadarMsgArray * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // objects
  ros_plc__msg__RadarMsg__Sequence__fini(&msg->objects);
}

bool
ros_plc__msg__RadarMsgArray__are_equal(const ros_plc__msg__RadarMsgArray * lhs, const ros_plc__msg__RadarMsgArray * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // objects
  if (!ros_plc__msg__RadarMsg__Sequence__are_equal(
      &(lhs->objects), &(rhs->objects)))
  {
    return false;
  }
  return true;
}

bool
ros_plc__msg__RadarMsgArray__copy(
  const ros_plc__msg__RadarMsgArray * input,
  ros_plc__msg__RadarMsgArray * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // objects
  if (!ros_plc__msg__RadarMsg__Sequence__copy(
      &(input->objects), &(output->objects)))
  {
    return false;
  }
  return true;
}

ros_plc__msg__RadarMsgArray *
ros_plc__msg__RadarMsgArray__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsgArray * msg = (ros_plc__msg__RadarMsgArray *)allocator.allocate(sizeof(ros_plc__msg__RadarMsgArray), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ros_plc__msg__RadarMsgArray));
  bool success = ros_plc__msg__RadarMsgArray__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ros_plc__msg__RadarMsgArray__destroy(ros_plc__msg__RadarMsgArray * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ros_plc__msg__RadarMsgArray__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ros_plc__msg__RadarMsgArray__Sequence__init(ros_plc__msg__RadarMsgArray__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsgArray * data = NULL;

  if (size) {
    data = (ros_plc__msg__RadarMsgArray *)allocator.zero_allocate(size, sizeof(ros_plc__msg__RadarMsgArray), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ros_plc__msg__RadarMsgArray__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ros_plc__msg__RadarMsgArray__fini(&data[i - 1]);
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
ros_plc__msg__RadarMsgArray__Sequence__fini(ros_plc__msg__RadarMsgArray__Sequence * array)
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
      ros_plc__msg__RadarMsgArray__fini(&array->data[i]);
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

ros_plc__msg__RadarMsgArray__Sequence *
ros_plc__msg__RadarMsgArray__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ros_plc__msg__RadarMsgArray__Sequence * array = (ros_plc__msg__RadarMsgArray__Sequence *)allocator.allocate(sizeof(ros_plc__msg__RadarMsgArray__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ros_plc__msg__RadarMsgArray__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ros_plc__msg__RadarMsgArray__Sequence__destroy(ros_plc__msg__RadarMsgArray__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ros_plc__msg__RadarMsgArray__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ros_plc__msg__RadarMsgArray__Sequence__are_equal(const ros_plc__msg__RadarMsgArray__Sequence * lhs, const ros_plc__msg__RadarMsgArray__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ros_plc__msg__RadarMsgArray__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ros_plc__msg__RadarMsgArray__Sequence__copy(
  const ros_plc__msg__RadarMsgArray__Sequence * input,
  ros_plc__msg__RadarMsgArray__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(ros_plc__msg__RadarMsgArray);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ros_plc__msg__RadarMsgArray * data =
      (ros_plc__msg__RadarMsgArray *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ros_plc__msg__RadarMsgArray__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ros_plc__msg__RadarMsgArray__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ros_plc__msg__RadarMsgArray__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
