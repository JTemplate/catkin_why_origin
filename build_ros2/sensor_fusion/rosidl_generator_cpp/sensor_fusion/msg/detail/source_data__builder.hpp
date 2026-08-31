// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sensor_fusion:msg/SourceData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__BUILDER_HPP_
#define SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sensor_fusion/msg/detail/source_data__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sensor_fusion
{

namespace msg
{

namespace builder
{

class Init_SourceData_vy
{
public:
  explicit Init_SourceData_vy(::sensor_fusion::msg::SourceData & msg)
  : msg_(msg)
  {}
  ::sensor_fusion::msg::SourceData vy(::sensor_fusion::msg::SourceData::_vy_type arg)
  {
    msg_.vy = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sensor_fusion::msg::SourceData msg_;
};

class Init_SourceData_vx
{
public:
  explicit Init_SourceData_vx(::sensor_fusion::msg::SourceData & msg)
  : msg_(msg)
  {}
  Init_SourceData_vy vx(::sensor_fusion::msg::SourceData::_vx_type arg)
  {
    msg_.vx = std::move(arg);
    return Init_SourceData_vy(msg_);
  }

private:
  ::sensor_fusion::msg::SourceData msg_;
};

class Init_SourceData_y
{
public:
  explicit Init_SourceData_y(::sensor_fusion::msg::SourceData & msg)
  : msg_(msg)
  {}
  Init_SourceData_vx y(::sensor_fusion::msg::SourceData::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_SourceData_vx(msg_);
  }

private:
  ::sensor_fusion::msg::SourceData msg_;
};

class Init_SourceData_x
{
public:
  Init_SourceData_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SourceData_y x(::sensor_fusion::msg::SourceData::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_SourceData_y(msg_);
  }

private:
  ::sensor_fusion::msg::SourceData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sensor_fusion::msg::SourceData>()
{
  return sensor_fusion::msg::builder::Init_SourceData_x();
}

}  // namespace sensor_fusion

#endif  // SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__BUILDER_HPP_
