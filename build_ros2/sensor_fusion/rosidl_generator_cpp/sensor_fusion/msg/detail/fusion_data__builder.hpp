// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sensor_fusion:msg/FusionData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__BUILDER_HPP_
#define SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sensor_fusion/msg/detail/fusion_data__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sensor_fusion
{

namespace msg
{

namespace builder
{

class Init_FusionData_vy
{
public:
  explicit Init_FusionData_vy(::sensor_fusion::msg::FusionData & msg)
  : msg_(msg)
  {}
  ::sensor_fusion::msg::FusionData vy(::sensor_fusion::msg::FusionData::_vy_type arg)
  {
    msg_.vy = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sensor_fusion::msg::FusionData msg_;
};

class Init_FusionData_vx
{
public:
  explicit Init_FusionData_vx(::sensor_fusion::msg::FusionData & msg)
  : msg_(msg)
  {}
  Init_FusionData_vy vx(::sensor_fusion::msg::FusionData::_vx_type arg)
  {
    msg_.vx = std::move(arg);
    return Init_FusionData_vy(msg_);
  }

private:
  ::sensor_fusion::msg::FusionData msg_;
};

class Init_FusionData_y
{
public:
  explicit Init_FusionData_y(::sensor_fusion::msg::FusionData & msg)
  : msg_(msg)
  {}
  Init_FusionData_vx y(::sensor_fusion::msg::FusionData::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_FusionData_vx(msg_);
  }

private:
  ::sensor_fusion::msg::FusionData msg_;
};

class Init_FusionData_x
{
public:
  Init_FusionData_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FusionData_y x(::sensor_fusion::msg::FusionData::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_FusionData_y(msg_);
  }

private:
  ::sensor_fusion::msg::FusionData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sensor_fusion::msg::FusionData>()
{
  return sensor_fusion::msg::builder::Init_FusionData_x();
}

}  // namespace sensor_fusion

#endif  // SENSOR_FUSION__MSG__DETAIL__FUSION_DATA__BUILDER_HPP_
