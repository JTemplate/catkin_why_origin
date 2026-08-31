#!/usr/bin/env python3
"""Print header stamps of the latest messages on the given topics."""
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from ros_plc.msg import LidarMsgArray, RadarMsgArray


class StampProbe(Node):
    def __init__(self):
        super().__init__("stamp_probe")
        qos = QoSProfile(depth=10, reliability=ReliabilityPolicy.BEST_EFFORT)
        self.create_subscription(RadarMsgArray, "/radar_objects", self.cb_radar, qos)
        self.create_subscription(LidarMsgArray, "/lidar_objects", self.cb_lidar, qos)

    def cb_radar(self, msg):
        t = msg.header.stamp
        print(f"radar_objects: stamp={t.sec}.{t.nanosec:09d} objects={len(msg.objects)}")

    def cb_lidar(self, msg):
        t = msg.header.stamp
        print(f"lidar_objects: stamp={t.sec}.{t.nanosec:09d} objects={len(msg.objects)}")


def main():
    rclpy.init()
    node = StampProbe()
    end = time.time() + float(sys.argv[1]) if len(sys.argv) > 1 else 6.0
    while time.time() < end:
        rclpy.spin_once(node, timeout_sec=0.5)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
