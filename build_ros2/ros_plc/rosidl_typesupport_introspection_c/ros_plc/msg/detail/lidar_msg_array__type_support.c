// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros_plc:msg/LidarMsgArray.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros_plc/msg/detail/lidar_msg_array__rosidl_typesupport_introspection_c.h"
#include "ros_plc/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros_plc/msg/detail/lidar_msg_array__functions.h"
#include "ros_plc/msg/detail/lidar_msg_array__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `objects`
#include "ros_plc/msg/lidar_msg.h"
// Member `objects`
#include "ros_plc/msg/detail/lidar_msg__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros_plc__msg__LidarMsgArray__init(message_memory);
}

void ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_fini_function(void * message_memory)
{
  ros_plc__msg__LidarMsgArray__fini(message_memory);
}

size_t ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__size_function__LidarMsgArray__objects(
  const void * untyped_member)
{
  const ros_plc__msg__LidarMsg__Sequence * member =
    (const ros_plc__msg__LidarMsg__Sequence *)(untyped_member);
  return member->size;
}

const void * ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_const_function__LidarMsgArray__objects(
  const void * untyped_member, size_t index)
{
  const ros_plc__msg__LidarMsg__Sequence * member =
    (const ros_plc__msg__LidarMsg__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_function__LidarMsgArray__objects(
  void * untyped_member, size_t index)
{
  ros_plc__msg__LidarMsg__Sequence * member =
    (ros_plc__msg__LidarMsg__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__fetch_function__LidarMsgArray__objects(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros_plc__msg__LidarMsg * item =
    ((const ros_plc__msg__LidarMsg *)
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_const_function__LidarMsgArray__objects(untyped_member, index));
  ros_plc__msg__LidarMsg * value =
    (ros_plc__msg__LidarMsg *)(untyped_value);
  *value = *item;
}

void ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__assign_function__LidarMsgArray__objects(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros_plc__msg__LidarMsg * item =
    ((ros_plc__msg__LidarMsg *)
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_function__LidarMsgArray__objects(untyped_member, index));
  const ros_plc__msg__LidarMsg * value =
    (const ros_plc__msg__LidarMsg *)(untyped_value);
  *item = *value;
}

bool ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__resize_function__LidarMsgArray__objects(
  void * untyped_member, size_t size)
{
  ros_plc__msg__LidarMsg__Sequence * member =
    (ros_plc__msg__LidarMsg__Sequence *)(untyped_member);
  ros_plc__msg__LidarMsg__Sequence__fini(member);
  return ros_plc__msg__LidarMsg__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_plc__msg__LidarMsgArray, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "objects",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_plc__msg__LidarMsgArray, objects),  // bytes offset in struct
    NULL,  // default value
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__size_function__LidarMsgArray__objects,  // size() function pointer
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_const_function__LidarMsgArray__objects,  // get_const(index) function pointer
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__get_function__LidarMsgArray__objects,  // get(index) function pointer
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__fetch_function__LidarMsgArray__objects,  // fetch(index, &value) function pointer
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__assign_function__LidarMsgArray__objects,  // assign(index, value) function pointer
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__resize_function__LidarMsgArray__objects  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_members = {
  "ros_plc__msg",  // message namespace
  "LidarMsgArray",  // message name
  2,  // number of fields
  sizeof(ros_plc__msg__LidarMsgArray),
  ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_member_array,  // message members
  ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_init_function,  // function to initialize message memory (memory has to be allocated)
  ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_type_support_handle = {
  0,
  &ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros_plc
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_plc, msg, LidarMsgArray)() {
  ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_plc, msg, LidarMsg)();
  if (!ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_type_support_handle.typesupport_identifier) {
    ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros_plc__msg__LidarMsgArray__rosidl_typesupport_introspection_c__LidarMsgArray_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
