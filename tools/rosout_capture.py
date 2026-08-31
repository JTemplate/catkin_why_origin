#!/usr/bin/env python3
"""Echo /rosout for a few seconds to capture the player's own logging."""
import sys
import time

import rclpy
from rclpy.node import Node
from rcl_interfaces.msg import Log


class RosoutCapture(Node):
    def __init__(self, duration):
        super().__init__("rosout_capture")
        self.duration = duration
        self.create_subscription(Log, "/rosout", self.cb, 100)

    def cb(self, msg):
        print(f"[{msg.name}] [{msg.level}] {msg.msg}")

    def run(self):
        end = time.time() + self.duration
        while time.time() < end:
            rclpy.spin_once(self, timeout_sec=0.5)


def main():
    rclpy.init()
    node = RosoutCapture(float(sys.argv[1]) if len(sys.argv) > 1 else 10.0)
    node.run()
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
