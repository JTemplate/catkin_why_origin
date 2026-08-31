// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sensor_fusion:msg/SourceData.idl
// generated code does not contain a copyright notice

#ifndef SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__TRAITS_HPP_
#define SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sensor_fusion/msg/detail/source_data__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace sensor_fusion
{

namespace msg
{

inline void to_flow_style_yaml(
  const SourceData & msg,
  std::ostream & out)
{
  out << "{";
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

  // member: vx
  {
    out << "vx: ";
    rosidl_generator_traits::value_to_yaml(msg.vx, out);
    out << ", ";
  }

  // member: vy
  {
    out << "vy: ";
    rosidl_generator_traits::value_to_yaml(msg.vy, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SourceData & msg,
  std::ostream & out, size_t indentation = 0)
{
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

  // member: vx
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "vx: ";
    rosidl_generator_traits::value_to_yaml(msg.vx, out);
    out << "\n";
  }

  // member: vy
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "vy: ";
    rosidl_generator_traits::value_to_yaml(msg.vy, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SourceData & msg, bool use_flow_style = false)
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

}  // namespace sensor_fusion

namespace rosidl_generator_traits
{

[[deprecated("use sensor_fusion::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const sensor_fusion::msg::SourceData & msg,
  std::ostream & out, size_t indentation = 0)
{
  sensor_fusion::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sensor_fusion::msg::to_yaml() instead")]]
inline std::string to_yaml(const sensor_fusion::msg::SourceData & msg)
{
  return sensor_fusion::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sensor_fusion::msg::SourceData>()
{
  return "sensor_fusion::msg::SourceData";
}

template<>
inline const char * name<sensor_fusion::msg::SourceData>()
{
  return "sensor_fusion/msg/SourceData";
}

template<>
struct has_fixed_size<sensor_fusion::msg::SourceData>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<sensor_fusion::msg::SourceData>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<sensor_fusion::msg::SourceData>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SENSOR_FUSION__MSG__DETAIL__SOURCE_DATA__TRAITS_HPP_
