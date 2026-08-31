// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sensor_fusion:msg/FusionData.idl
// generated code does not contain a copyright notice
#include "sensor_fusion/msg/detail/fusion_data__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
sensor_fusion__msg__FusionData__init(sensor_fusion__msg__FusionData * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  // vx
  // vy
  return true;
}

void
sensor_fusion__msg__FusionData__fini(sensor_fusion__msg__FusionData * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
  // vx
  // vy
}

bool
sensor_fusion__msg__FusionData__are_equal(const sensor_fusion__msg__FusionData * lhs, const sensor_fusion__msg__FusionData * rhs)
{
  if (!lhs || !rhs) {
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
  // vx
  if (lhs->vx != rhs->vx) {
    return false;
  }
  // vy
  if (lhs->vy != rhs->vy) {
    return false;
  }
  return true;
}

bool
sensor_fusion__msg__FusionData__copy(
  const sensor_fusion__msg__FusionData * input,
  sensor_fusion__msg__FusionData * output)
{
  if (!input || !output) {
    return false;
  }
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // vx
  output->vx = input->vx;
  // vy
  output->vy = input->vy;
  return true;
}

sensor_fusion__msg__FusionData *
sensor_fusion__msg__FusionData__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sensor_fusion__msg__FusionData * msg = (sensor_fusion__msg__FusionData *)allocator.allocate(sizeof(sensor_fusion__msg__FusionData), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sensor_fusion__msg__FusionData));
  bool success = sensor_fusion__msg__FusionData__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sensor_fusion__msg__FusionData__destroy(sensor_fusion__msg__FusionData * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sensor_fusion__msg__FusionData__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sensor_fusion__msg__FusionData__Sequence__init(sensor_fusion__msg__FusionData__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sensor_fusion__msg__FusionData * data = NULL;

  if (size) {
    data = (sensor_fusion__msg__FusionData *)allocator.zero_allocate(size, sizeof(sensor_fusion__msg__FusionData), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sensor_fusion__msg__FusionData__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sensor_fusion__msg__FusionData__fini(&data[i - 1]);
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
sensor_fusion__msg__FusionData__Sequence__fini(sensor_fusion__msg__FusionData__Sequence * array)
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
      sensor_fusion__msg__FusionData__fini(&array->data[i]);
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

sensor_fusion__msg__FusionData__Sequence *
sensor_fusion__msg__FusionData__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sensor_fusion__msg__FusionData__Sequence * array = (sensor_fusion__msg__FusionData__Sequence *)allocator.allocate(sizeof(sensor_fusion__msg__FusionData__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sensor_fusion__msg__FusionData__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sensor_fusion__msg__FusionData__Sequence__destroy(sensor_fusion__msg__FusionData__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sensor_fusion__msg__FusionData__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sensor_fusion__msg__FusionData__Sequence__are_equal(const sensor_fusion__msg__FusionData__Sequence * lhs, const sensor_fusion__msg__FusionData__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sensor_fusion__msg__FusionData__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sensor_fusion__msg__FusionData__Sequence__copy(
  const sensor_fusion__msg__FusionData__Sequence * input,
  sensor_fusion__msg__FusionData__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sensor_fusion__msg__FusionData);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sensor_fusion__msg__FusionData * data =
      (sensor_fusion__msg__FusionData *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sensor_fusion__msg__FusionData__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sensor_fusion__msg__FusionData__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sensor_fusion__msg__FusionData__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
