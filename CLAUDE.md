# ROS 2 Perception Workspace Guide

This is a ROS 2 Humble workspace containing four `ament_cmake` packages:

- `lidar_perception`: point-cloud clustering, bounding boxes, and tracking.
- `ros_plc`: point-cloud filters plus LiDAR/radar object message adapters.
- `sensor_fusion`: synchronized LiDAR/radar extended Kalman filtering.
- `start_all`: launch and RViz2 configuration for the complete stack.

## Build

The workspace still contains historical catkin build output, so use dedicated
ROS 2 output directories unless those old generated directories are removed:

```bash
source /opt/ros/humble/setup.bash
colcon --log-base log_ros2 build \
  --build-base build_ros2 \
  --install-base install_ros2 \
  --symlink-install
source install_ros2/setup.bash
```

If Conda is active, pass the system Python executables to CMake:

```bash
colcon --log-base log_ros2 build \
  --build-base build_ros2 \
  --install-base install_ros2 \
  --symlink-install \
  --cmake-args \
    -DPython3_EXECUTABLE=/usr/bin/python3 \
    -DPYTHON_EXECUTABLE=/usr/bin/python3
```

## Run

```bash
source /opt/ros/humble/setup.bash
source install_ros2/setup.bash
ros2 launch start_all start.launch.xml
```

Individual launch files are installed by their packages:

- `ros2 launch ros_plc StartFilter.launch.xml`
- `ros2 launch lidar_perception lidar_perception.launch.xml`
- `ros2 launch sensor_fusion sensorfusion.launch.xml`

Use ROS 2 tools such as `ros2 node list`, `ros2 topic list`,
`ros2 topic echo`, and RViz2 for diagnosis.

## Development constraints

- Preserve the existing package and algorithm structure.
- Keep TF and sensor frame conventions stable unless a task explicitly changes them.
- Put C++ behavior in the existing sources and keep launch files in XML format.
- Build with C++17 and validate changes with `colcon build`.
