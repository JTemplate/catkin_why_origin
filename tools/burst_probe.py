#!/usr/bin/env python3
"""Subscribe to the bag topics, then force the (paused) player to burst
N messages, and report how many arrive. Proves end-to-end data validity."""
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from rosbag2_interfaces.srv import Burst
from sensor_msgs.msg import PointCloud2
from tf2_msgs.msg import TFMessage


class BurstProbe(Node):
    def __init__(self):
        super().__init__("burst_probe")
        self.counts = {"/rslidar_points": 0, "/tf": 0, "/ti_mmwave/radar_scan_pcl_0": 0}
        qos = QoSProfile(depth=50, reliability=ReliabilityPolicy.BEST_EFFORT)
        self.create_subscription(PointCloud2, "/rslidar_points", self._cb("/rslidar_points"), qos)
        self.create_subscription(PointCloud2, "/ti_mmwave/radar_scan_pcl_0", self._cb("/ti_mmwave/radar_scan_pcl_0"), qos)
        self.create_subscription(TFMessage, "/tf", self._cb("/tf"), qos)

    def _cb(self, topic):
        def cb(msg):
            self.counts[topic] += 1
        return cb

    def burst(self, n, timeout=15.0):
        client = self.create_client(Burst, "/rosbag2_player/burst")
        if not client.wait_for_service(timeout_sec=5.0):
            print("burst service unavailable")
            return None
        req = Burst.Request()
        req.num_messages = n
        future = client.call_async(req)
        end = time.time() + timeout
        while time.time() < end and not future.done():
            rclpy.spin_once(self, timeout_sec=0.2)
        if future.done():
            return future.result().actually_burst
        return "timeout"


def main():
    rclpy.init()
    node = BurstProbe()
    time.sleep(2.0)  # let discovery settle
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    result = node.burst(n)
    print(f"burst(n={n}) -> published: {result}")
    end = time.time() + 4.0
    while time.time() < end:
        rclpy.spin_once(node, timeout_sec=0.2)
    print("received:", node.counts)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
