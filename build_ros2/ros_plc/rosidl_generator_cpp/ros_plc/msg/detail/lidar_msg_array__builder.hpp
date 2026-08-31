// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros_plc:msg/LidarMsgArray.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__BUILDER_HPP_
#define ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros_plc/msg/detail/lidar_msg_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros_plc
{

namespace msg
{

namespace builder
{

class Init_LidarMsgArray_objects
{
public:
  explicit Init_LidarMsgArray_objects(::ros_plc::msg::LidarMsgArray & msg)
  : msg_(msg)
  {}
  ::ros_plc::msg::LidarMsgArray objects(::ros_plc::msg::LidarMsgArray::_objects_type arg)
  {
    msg_.objects = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_plc::msg::LidarMsgArray msg_;
};

class Init_LidarMsgArray_header
{
public:
  Init_LidarMsgArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LidarMsgArray_objects header(::ros_plc::msg::LidarMsgArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_LidarMsgArray_objects(msg_);
  }

private:
  ::ros_plc::msg::LidarMsgArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_plc::msg::LidarMsgArray>()
{
  return ros_plc::msg::builder::Init_LidarMsgArray_header();
}

}  // namespace ros_plc

#endif  // ROS_PLC__MSG__DETAIL__LIDAR_MSG_ARRAY__BUILDER_HPP_
