// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros_plc:msg/LidarMsg.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__LIDAR_MSG__BUILDER_HPP_
#define ROS_PLC__MSG__DETAIL__LIDAR_MSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros_plc/msg/detail/lidar_msg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros_plc
{

namespace msg
{

namespace builder
{

class Init_LidarMsg_timestamp
{
public:
  explicit Init_LidarMsg_timestamp(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  ::ros_plc::msg::LidarMsg timestamp(::ros_plc::msg::LidarMsg::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_speed
{
public:
  explicit Init_LidarMsg_speed(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  Init_LidarMsg_timestamp speed(::ros_plc::msg::LidarMsg::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_LidarMsg_timestamp(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_z
{
public:
  explicit Init_LidarMsg_z(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  Init_LidarMsg_speed z(::ros_plc::msg::LidarMsg::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_LidarMsg_speed(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_y
{
public:
  explicit Init_LidarMsg_y(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  Init_LidarMsg_z y(::ros_plc::msg::LidarMsg::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_LidarMsg_z(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_x
{
public:
  explicit Init_LidarMsg_x(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  Init_LidarMsg_y x(::ros_plc::msg::LidarMsg::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_LidarMsg_y(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_original_id
{
public:
  explicit Init_LidarMsg_original_id(::ros_plc::msg::LidarMsg & msg)
  : msg_(msg)
  {}
  Init_LidarMsg_x original_id(::ros_plc::msg::LidarMsg::_original_id_type arg)
  {
    msg_.original_id = std::move(arg);
    return Init_LidarMsg_x(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

class Init_LidarMsg_current_id
{
public:
  Init_LidarMsg_current_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LidarMsg_original_id current_id(::ros_plc::msg::LidarMsg::_current_id_type arg)
  {
    msg_.current_id = std::move(arg);
    return Init_LidarMsg_original_id(msg_);
  }

private:
  ::ros_plc::msg::LidarMsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_plc::msg::LidarMsg>()
{
  return ros_plc::msg::builder::Init_LidarMsg_current_id();
}

}  // namespace ros_plc

#endif  // ROS_PLC__MSG__DETAIL__LIDAR_MSG__BUILDER_HPP_
