# CMake generated Testfile for 
# Source directory: /home/jj/catkin_why_origin/src/Sensor_Fusion-master
# Build directory: /home/jj/catkin_why_origin/build_ros2/sensor_fusion
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sensor_fusion_timestamp_test "/usr/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/jj/catkin_why_origin/build_ros2/sensor_fusion/test_results/sensor_fusion/sensor_fusion_timestamp_test.gtest.xml" "--package-name" "sensor_fusion" "--output-file" "/home/jj/catkin_why_origin/build_ros2/sensor_fusion/ament_cmake_gtest/sensor_fusion_timestamp_test.txt" "--command" "/home/jj/catkin_why_origin/build_ros2/sensor_fusion/sensor_fusion_timestamp_test" "--gtest_output=xml:/home/jj/catkin_why_origin/build_ros2/sensor_fusion/test_results/sensor_fusion/sensor_fusion_timestamp_test.gtest.xml")
set_tests_properties(sensor_fusion_timestamp_test PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/jj/catkin_why_origin/build_ros2/sensor_fusion/sensor_fusion_timestamp_test" TIMEOUT "60" WORKING_DIRECTORY "/home/jj/catkin_why_origin/build_ros2/sensor_fusion" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;86;ament_add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/jj/catkin_why_origin/src/Sensor_Fusion-master/CMakeLists.txt;38;ament_add_gtest;/home/jj/catkin_why_origin/src/Sensor_Fusion-master/CMakeLists.txt;0;")
subdirs("sensor_fusion__py")
subdirs("sensor_fusion__rs")
subdirs("gtest")
