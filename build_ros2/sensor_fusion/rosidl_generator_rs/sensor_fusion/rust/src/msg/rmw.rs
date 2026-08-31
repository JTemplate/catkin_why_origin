#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "sensor_fusion__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__sensor_fusion__msg__FusionData() -> *const std::ffi::c_void;
}

#[link(name = "sensor_fusion__rosidl_generator_c")]
extern "C" {
    fn sensor_fusion__msg__FusionData__init(msg: *mut FusionData) -> bool;
    fn sensor_fusion__msg__FusionData__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<FusionData>, size: usize) -> bool;
    fn sensor_fusion__msg__FusionData__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<FusionData>);
    fn sensor_fusion__msg__FusionData__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<FusionData>, out_seq: *mut rosidl_runtime_rs::Sequence<FusionData>) -> bool;
}

// Corresponds to sensor_fusion__msg__FusionData
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct FusionData {

    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub vx: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub vy: f64,

}



impl Default for FusionData {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !sensor_fusion__msg__FusionData__init(&mut msg as *mut _) {
        panic!("Call to sensor_fusion__msg__FusionData__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for FusionData {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__FusionData__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__FusionData__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__FusionData__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for FusionData {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for FusionData where Self: Sized {
  const TYPE_NAME: &'static str = "sensor_fusion/msg/FusionData";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__sensor_fusion__msg__FusionData() }
  }
}


#[link(name = "sensor_fusion__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__sensor_fusion__msg__SourceData() -> *const std::ffi::c_void;
}

#[link(name = "sensor_fusion__rosidl_generator_c")]
extern "C" {
    fn sensor_fusion__msg__SourceData__init(msg: *mut SourceData) -> bool;
    fn sensor_fusion__msg__SourceData__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SourceData>, size: usize) -> bool;
    fn sensor_fusion__msg__SourceData__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SourceData>);
    fn sensor_fusion__msg__SourceData__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SourceData>, out_seq: *mut rosidl_runtime_rs::Sequence<SourceData>) -> bool;
}

// Corresponds to sensor_fusion__msg__SourceData
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SourceData {

    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub vx: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub vy: f64,

}



impl Default for SourceData {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !sensor_fusion__msg__SourceData__init(&mut msg as *mut _) {
        panic!("Call to sensor_fusion__msg__SourceData__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SourceData {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__SourceData__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__SourceData__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { sensor_fusion__msg__SourceData__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SourceData {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SourceData where Self: Sized {
  const TYPE_NAME: &'static str = "sensor_fusion/msg/SourceData";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__sensor_fusion__msg__SourceData() }
  }
}


