// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sensor_fusion:msg/FusionData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_HPP_
#define SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__sensor_fusion__msg__FusionData __attribute__((deprecated))
#else
# define DEPRECATED__sensor_fusion__msg__FusionData __declspec(deprecated)
#endif

namespace sensor_fusion
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct FusionData_
{
  using Type = FusionData_<ContainerAllocator>;

  explicit FusionData_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0;
      this->y = 0.0;
      this->vx = 0.0;
      this->vy = 0.0;
    }
  }

  explicit FusionData_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0;
      this->y = 0.0;
      this->vx = 0.0;
      this->vy = 0.0;
    }
  }

  // field types and members
  using _x_type =
    double;
  _x_type x;
  using _y_type =
    double;
  _y_type y;
  using _vx_type =
    double;
  _vx_type vx;
  using _vy_type =
    double;
  _vy_type vy;

  // setters for named parameter idiom
  Type & set__x(
    const double & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const double & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__vx(
    const double & _arg)
  {
    this->vx = _arg;
    return *this;
  }
  Type & set__vy(
    const double & _arg)
  {
    this->vy = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sensor_fusion::msg::FusionData_<ContainerAllocator> *;
  using ConstRawPtr =
    const sensor_fusion::msg::FusionData_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sensor_fusion::msg::FusionData_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sensor_fusion::msg::FusionData_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sensor_fusion__msg__FusionData
    std::shared_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sensor_fusion__msg__FusionData
    std::shared_ptr<sensor_fusion::msg::FusionData_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FusionData_ & other) const
  {
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->vx != other.vx) {
      return false;
    }
    if (this->vy != other.vy) {
      return false;
    }
    return true;
  }
  bool operator!=(const FusionData_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FusionData_

// alias to use template instance with default allocator
using FusionData =
  sensor_fusion::msg::FusionData_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sensor_fusion

#endif  // SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__STRUCT_HPP_
