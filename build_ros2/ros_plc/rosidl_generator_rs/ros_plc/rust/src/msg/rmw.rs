#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "ros_plc__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__LidarMsg() -> *const std::ffi::c_void;
}

#[link(name = "ros_plc__rosidl_generator_c")]
extern "C" {
    fn ros_plc__msg__LidarMsg__init(msg: *mut LidarMsg) -> bool;
    fn ros_plc__msg__LidarMsg__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<LidarMsg>, size: usize) -> bool;
    fn ros_plc__msg__LidarMsg__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<LidarMsg>);
    fn ros_plc__msg__LidarMsg__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<LidarMsg>, out_seq: *mut rosidl_runtime_rs::Sequence<LidarMsg>) -> bool;
}

// Corresponds to ros_plc__msg__LidarMsg
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct LidarMsg {

    // This member is not documented.
    #[allow(missing_docs)]
    pub current_id: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub original_id: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub z: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub speed: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub timestamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for LidarMsg {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ros_plc__msg__LidarMsg__init(&mut msg as *mut _) {
        panic!("Call to ros_plc__msg__LidarMsg__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for LidarMsg {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsg__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsg__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsg__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for LidarMsg {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for LidarMsg where Self: Sized {
  const TYPE_NAME: &'static str = "ros_plc/msg/LidarMsg";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__LidarMsg() }
  }
}


#[link(name = "ros_plc__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__LidarMsgArray() -> *const std::ffi::c_void;
}

#[link(name = "ros_plc__rosidl_generator_c")]
extern "C" {
    fn ros_plc__msg__LidarMsgArray__init(msg: *mut LidarMsgArray) -> bool;
    fn ros_plc__msg__LidarMsgArray__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<LidarMsgArray>, size: usize) -> bool;
    fn ros_plc__msg__LidarMsgArray__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<LidarMsgArray>);
    fn ros_plc__msg__LidarMsgArray__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<LidarMsgArray>, out_seq: *mut rosidl_runtime_rs::Sequence<LidarMsgArray>) -> bool;
}

// Corresponds to ros_plc__msg__LidarMsgArray
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct LidarMsgArray {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub objects: rosidl_runtime_rs::Sequence<super::super::msg::rmw::LidarMsg>,

}



impl Default for LidarMsgArray {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ros_plc__msg__LidarMsgArray__init(&mut msg as *mut _) {
        panic!("Call to ros_plc__msg__LidarMsgArray__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for LidarMsgArray {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsgArray__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsgArray__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__LidarMsgArray__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for LidarMsgArray {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for LidarMsgArray where Self: Sized {
  const TYPE_NAME: &'static str = "ros_plc/msg/LidarMsgArray";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__LidarMsgArray() }
  }
}


#[link(name = "ros_plc__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__RadarMsg() -> *const std::ffi::c_void;
}

#[link(name = "ros_plc__rosidl_generator_c")]
extern "C" {
    fn ros_plc__msg__RadarMsg__init(msg: *mut RadarMsg) -> bool;
    fn ros_plc__msg__RadarMsg__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<RadarMsg>, size: usize) -> bool;
    fn ros_plc__msg__RadarMsg__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<RadarMsg>);
    fn ros_plc__msg__RadarMsg__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<RadarMsg>, out_seq: *mut rosidl_runtime_rs::Sequence<RadarMsg>) -> bool;
}

// Corresponds to ros_plc__msg__RadarMsg
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct RadarMsg {

    // This member is not documented.
    #[allow(missing_docs)]
    pub current_id: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub original_id: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub phi: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub speed: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub timestamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for RadarMsg {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ros_plc__msg__RadarMsg__init(&mut msg as *mut _) {
        panic!("Call to ros_plc__msg__RadarMsg__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for RadarMsg {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsg__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsg__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsg__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for RadarMsg {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for RadarMsg where Self: Sized {
  const TYPE_NAME: &'static str = "ros_plc/msg/RadarMsg";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__RadarMsg() }
  }
}


#[link(name = "ros_plc__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__RadarMsgArray() -> *const std::ffi::c_void;
}

#[link(name = "ros_plc__rosidl_generator_c")]
extern "C" {
    fn ros_plc__msg__RadarMsgArray__init(msg: *mut RadarMsgArray) -> bool;
    fn ros_plc__msg__RadarMsgArray__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<RadarMsgArray>, size: usize) -> bool;
    fn ros_plc__msg__RadarMsgArray__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<RadarMsgArray>);
    fn ros_plc__msg__RadarMsgArray__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<RadarMsgArray>, out_seq: *mut rosidl_runtime_rs::Sequence<RadarMsgArray>) -> bool;
}

// Corresponds to ros_plc__msg__RadarMsgArray
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct RadarMsgArray {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub objects: rosidl_runtime_rs::Sequence<super::super::msg::rmw::RadarMsg>,

}



impl Default for RadarMsgArray {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ros_plc__msg__RadarMsgArray__init(&mut msg as *mut _) {
        panic!("Call to ros_plc__msg__RadarMsgArray__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for RadarMsgArray {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsgArray__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsgArray__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ros_plc__msg__RadarMsgArray__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for RadarMsgArray {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for RadarMsgArray where Self: Sized {
  const TYPE_NAME: &'static str = "ros_plc/msg/RadarMsgArray";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ros_plc__msg__RadarMsgArray() }
  }
}


