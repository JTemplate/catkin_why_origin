// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros_plc:msg/RadarMsg.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__RADAR_MSG__BUILDER_HPP_
#define ROS_PLC__MSG__DETAIL__RADAR_MSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros_plc/msg/detail/radar_msg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros_plc
{

namespace msg
{

namespace builder
{

class Init_RadarMsg_timestamp
{
public:
  explicit Init_RadarMsg_timestamp(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  ::ros_plc::msg::RadarMsg timestamp(::ros_plc::msg::RadarMsg::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_speed
{
public:
  explicit Init_RadarMsg_speed(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  Init_RadarMsg_timestamp speed(::ros_plc::msg::RadarMsg::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_RadarMsg_timestamp(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_phi
{
public:
  explicit Init_RadarMsg_phi(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  Init_RadarMsg_speed phi(::ros_plc::msg::RadarMsg::_phi_type arg)
  {
    msg_.phi = std::move(arg);
    return Init_RadarMsg_speed(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_y
{
public:
  explicit Init_RadarMsg_y(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  Init_RadarMsg_phi y(::ros_plc::msg::RadarMsg::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_RadarMsg_phi(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_x
{
public:
  explicit Init_RadarMsg_x(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  Init_RadarMsg_y x(::ros_plc::msg::RadarMsg::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_RadarMsg_y(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_original_id
{
public:
  explicit Init_RadarMsg_original_id(::ros_plc::msg::RadarMsg & msg)
  : msg_(msg)
  {}
  Init_RadarMsg_x original_id(::ros_plc::msg::RadarMsg::_original_id_type arg)
  {
    msg_.original_id = std::move(arg);
    return Init_RadarMsg_x(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

class Init_RadarMsg_current_id
{
public:
  Init_RadarMsg_current_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RadarMsg_original_id current_id(::ros_plc::msg::RadarMsg::_current_id_type arg)
  {
    msg_.current_id = std::move(arg);
    return Init_RadarMsg_original_id(msg_);
  }

private:
  ::ros_plc::msg::RadarMsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_plc::msg::RadarMsg>()
{
  return ros_plc::msg::builder::Init_RadarMsg_current_id();
}

}  // namespace ros_plc

#endif  // ROS_PLC__MSG__DETAIL__RADAR_MSG__BUILDER_HPP_
