// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from ros_plc:msg/RadarMsgArray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "ros_plc/msg/detail/radar_msg_array__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace ros_plc
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void RadarMsgArray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) ros_plc::msg::RadarMsgArray(_init);
}

void RadarMsgArray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<ros_plc::msg::RadarMsgArray *>(message_memory);
  typed_message->~RadarMsgArray();
}

size_t size_function__RadarMsgArray__objects(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<ros_plc::msg::RadarMsg> *>(untyped_member);
  return member->size();
}

const void * get_const_function__RadarMsgArray__objects(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<ros_plc::msg::RadarMsg> *>(untyped_member);
  return &member[index];
}

void * get_function__RadarMsgArray__objects(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<ros_plc::msg::RadarMsg> *>(untyped_member);
  return &member[index];
}

void fetch_function__RadarMsgArray__objects(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const ros_plc::msg::RadarMsg *>(
    get_const_function__RadarMsgArray__objects(untyped_member, index));
  auto & value = *reinterpret_cast<ros_plc::msg::RadarMsg *>(untyped_value);
  value = item;
}

void assign_function__RadarMsgArray__objects(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<ros_plc::msg::RadarMsg *>(
    get_function__RadarMsgArray__objects(untyped_member, index));
  const auto & value = *reinterpret_cast<const ros_plc::msg::RadarMsg *>(untyped_value);
  item = value;
}

void resize_function__RadarMsgArray__objects(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<ros_plc::msg::RadarMsg> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember RadarMsgArray_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_plc::msg::RadarMsgArray, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "objects",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_plc::msg::RadarMsg>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_plc::msg::RadarMsgArray, objects),  // bytes offset in struct
    nullptr,  // default value
    size_function__RadarMsgArray__objects,  // size() function pointer
    get_const_function__RadarMsgArray__objects,  // get_const(index) function pointer
    get_function__RadarMsgArray__objects,  // get(index) function pointer
    fetch_function__RadarMsgArray__objects,  // fetch(index, &value) function pointer
    assign_function__RadarMsgArray__objects,  // assign(index, value) function pointer
    resize_function__RadarMsgArray__objects  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers RadarMsgArray_message_members = {
  "ros_plc::msg",  // message namespace
  "RadarMsgArray",  // message name
  2,  // number of fields
  sizeof(ros_plc::msg::RadarMsgArray),
  RadarMsgArray_message_member_array,  // message members
  RadarMsgArray_init_function,  // function to initialize message memory (memory has to be allocated)
  RadarMsgArray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t RadarMsgArray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &RadarMsgArray_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace ros_plc


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_plc::msg::RadarMsgArray>()
{
  return &::ros_plc::msg::rosidl_typesupport_introspection_cpp::RadarMsgArray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_plc, msg, RadarMsgArray)() {
  return &::ros_plc::msg::rosidl_typesupport_introspection_cpp::RadarMsgArray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
