// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ros_plc:msg/RadarMsg.idl
// generated code does not contain a copyright notice

#ifndef ROS_PLC__MSG__DETAIL__RADAR_MSG__TRAITS_HPP_
#define ROS_PLC__MSG__DETAIL__RADAR_MSG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "ros_plc/msg/detail/radar_msg__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'timestamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace ros_plc
{

namespace msg
{

inline void to_flow_style_yaml(
  const RadarMsg & msg,
  std::ostream & out)
{
  out << "{";
  // member: current_id
  {
    out << "current_id: ";
    rosidl_generator_traits::value_to_yaml(msg.current_id, out);
    out << ", ";
  }

  // member: original_id
  {
    out << "original_id: ";
    rosidl_generator_traits::value_to_yaml(msg.original_id, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: phi
  {
    out << "phi: ";
    rosidl_generator_traits::value_to_yaml(msg.phi, out);
    out << ", ";
  }

  // member: speed
  {
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << ", ";
  }

  // member: timestamp
  {
    out << "timestamp: ";
    to_flow_style_yaml(msg.timestamp, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RadarMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: current_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_id: ";
    rosidl_generator_traits::value_to_yaml(msg.current_id, out);
    out << "\n";
  }

  // member: original_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "original_id: ";
    rosidl_generator_traits::value_to_yaml(msg.original_id, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: phi
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "phi: ";
    rosidl_generator_traits::value_to_yaml(msg.phi, out);
    out << "\n";
  }

  // member: speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << "\n";
  }

  // member: timestamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "timestamp:\n";
    to_block_style_yaml(msg.timestamp, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RadarMsg & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace ros_plc

namespace rosidl_generator_traits
{

[[deprecated("use ros_plc::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const ros_plc::msg::RadarMsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  ros_plc::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use ros_plc::msg::to_yaml() instead")]]
inline std::string to_yaml(const ros_plc::msg::RadarMsg & msg)
{
  return ros_plc::msg::to_yaml(msg);
}

template<>
inline const char * data_type<ros_plc::msg::RadarMsg>()
{
  return "ros_plc::msg::RadarMsg";
}

template<>
inline const char * name<ros_plc::msg::RadarMsg>()
{
  return "ros_plc/msg/RadarMsg";
}

template<>
struct has_fixed_size<ros_plc::msg::RadarMsg>
  : std::integral_constant<bool, has_fixed_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct has_bounded_size<ros_plc::msg::RadarMsg>
  : std::integral_constant<bool, has_bounded_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct is_message<ros_plc::msg::RadarMsg>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROS_PLC__MSG__DETAIL__RADAR_MSG__TRAITS_HPP_
