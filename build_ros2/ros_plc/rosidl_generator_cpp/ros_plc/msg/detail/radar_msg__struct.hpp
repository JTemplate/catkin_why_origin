// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ros_plc:msg/RadarMsg.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__RADAR_MSG__STRUCT_HPP_
#define ROS_PLC__MSG__DETAIL__RADAR_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'timestamp'
#include "builtin_interfaces/msg/detail/time__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ros_plc__msg__RadarMsg __attribute__((deprecated))
#else
# define DEPRECATED__ros_plc__msg__RadarMsg __declspec(deprecated)
#endif

namespace ros_plc
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct RadarMsg_
{
  using Type = RadarMsg_<ContainerAllocator>;

  explicit RadarMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : timestamp(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->current_id = 0l;
      this->original_id = 0l;
      this->x = 0.0f;
      this->y = 0.0f;
      this->phi = 0.0f;
      this->speed = 0.0f;
    }
  }

  explicit RadarMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : timestamp(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->current_id = 0l;
      this->original_id = 0l;
      this->x = 0.0f;
      this->y = 0.0f;
      this->phi = 0.0f;
      this->speed = 0.0f;
    }
  }

  // field types and members
  using _current_id_type =
    int32_t;
  _current_id_type current_id;
  using _original_id_type =
    int32_t;
  _original_id_type original_id;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _phi_type =
    float;
  _phi_type phi;
  using _speed_type =
    float;
  _speed_type speed;
  using _timestamp_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _timestamp_type timestamp;

  // setters for named parameter idiom
  Type & set__current_id(
    const int32_t & _arg)
  {
    this->current_id = _arg;
    return *this;
  }
  Type & set__original_id(
    const int32_t & _arg)
  {
    this->original_id = _arg;
    return *this;
  }
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__phi(
    const float & _arg)
  {
    this->phi = _arg;
    return *this;
  }
  Type & set__speed(
    const float & _arg)
  {
    this->speed = _arg;
    return *this;
  }
  Type & set__timestamp(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->timestamp = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros_plc::msg::RadarMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros_plc::msg::RadarMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros_plc::msg::RadarMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros_plc::msg::RadarMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros_plc__msg__RadarMsg
    std::shared_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros_plc__msg__RadarMsg
    std::shared_ptr<ros_plc::msg::RadarMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RadarMsg_ & other) const
  {
    if (this->current_id != other.current_id) {
      return false;
    }
    if (this->original_id != other.original_id) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->phi != other.phi) {
      return false;
    }
    if (this->speed != other.speed) {
      return false;
    }
    if (this->timestamp != other.timestamp) {
      return false;
    }
    return true;
  }
  bool operator!=(const RadarMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RadarMsg_

// alias to use template instance with default allocator
using RadarMsg =
  ros_plc::msg::RadarMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace ros_plc

#endif  // ROS_PLC__MSG__DETAIL__RADAR_MSG__STRUCT_HPP_
