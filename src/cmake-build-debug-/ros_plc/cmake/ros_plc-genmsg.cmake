# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "ros_plc: 4 messages, 0 services")

set(MSG_I_FLAGS "-Iros_plc:/home/jzh/catkin_why/src/ros_plc/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(ros_plc_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_custom_target(_ros_plc_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ros_plc" "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" ""
)

get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_custom_target(_ros_plc_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ros_plc" "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" ""
)

get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_custom_target(_ros_plc_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ros_plc" "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" "ros_plc/radarMsg:std_msgs/Header"
)

get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_custom_target(_ros_plc_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ros_plc" "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" "ros_plc/lidarMsg:std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
)
_generate_msg_cpp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
)
_generate_msg_cpp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
)
_generate_msg_cpp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
)

### Generating Services

### Generating Module File
_generate_module_cpp(ros_plc
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(ros_plc_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(ros_plc_generate_messages ros_plc_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_cpp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_cpp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_cpp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_cpp _ros_plc_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ros_plc_gencpp)
add_dependencies(ros_plc_gencpp ros_plc_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ros_plc_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
)
_generate_msg_eus(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
)
_generate_msg_eus(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
)
_generate_msg_eus(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
)

### Generating Services

### Generating Module File
_generate_module_eus(ros_plc
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(ros_plc_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(ros_plc_generate_messages ros_plc_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_eus _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_eus _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_eus _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_eus _ros_plc_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ros_plc_geneus)
add_dependencies(ros_plc_geneus ros_plc_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ros_plc_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
)
_generate_msg_lisp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
)
_generate_msg_lisp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
)
_generate_msg_lisp(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
)

### Generating Services

### Generating Module File
_generate_module_lisp(ros_plc
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(ros_plc_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(ros_plc_generate_messages ros_plc_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_lisp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_lisp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_lisp _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_lisp _ros_plc_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ros_plc_genlisp)
add_dependencies(ros_plc_genlisp ros_plc_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ros_plc_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
)
_generate_msg_nodejs(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
)
_generate_msg_nodejs(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
)
_generate_msg_nodejs(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
)

### Generating Services

### Generating Module File
_generate_module_nodejs(ros_plc
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(ros_plc_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(ros_plc_generate_messages ros_plc_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_nodejs _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_nodejs _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_nodejs _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_nodejs _ros_plc_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ros_plc_gennodejs)
add_dependencies(ros_plc_gennodejs ros_plc_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ros_plc_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
)
_generate_msg_py(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
)
_generate_msg_py(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
)
_generate_msg_py(ros_plc
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg"
  "${MSG_I_FLAGS}"
  "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
)

### Generating Services

### Generating Module File
_generate_module_py(ros_plc
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(ros_plc_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(ros_plc_generate_messages ros_plc_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_py _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsg.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_py _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/radarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_py _ros_plc_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jzh/catkin_why/src/ros_plc/msg/lidarMsgArray.msg" NAME_WE)
add_dependencies(ros_plc_generate_messages_py _ros_plc_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ros_plc_genpy)
add_dependencies(ros_plc_genpy ros_plc_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ros_plc_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ros_plc
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(ros_plc_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/ros_plc
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(ros_plc_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/ros_plc
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(ros_plc_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/ros_plc
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(ros_plc_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ros_plc
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(ros_plc_generate_messages_py std_msgs_generate_messages_py)
endif()
