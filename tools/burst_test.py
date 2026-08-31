#!/usr/bin/env python3
"""Call the player's pause + burst services; report actually_published."""
import sys

import rclpy
from rclpy.node import Node
from rosbag2_interfaces.srv import Burst, Pause


def call_sync(node, client, req, timeout=10.0):
    if not client.wait_for_service(timeout_sec=5.0):
        print(f"service {client.srv_name} not available")
        return None
    future = client.call_async(req)
    rclpy.spin_until_future_complete(node, future, timeout_sec=timeout)
    return future.result()


def main():
    rclpy.init()
    node = Node("burst_client")
    pause = node.create_client(Pause, "/rosbag2_player/pause")
    burst = node.create_client(Burst, "/rosbag2_player/burst")

    r1 = call_sync(node, pause, Pause.Request())
    print("pause response:", r1)

    n = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    req = Burst.Request()
    req.num_messages = n
    r2 = call_sync(node, burst, req)
    print("burst response actually_published:", None if r2 is None else r2.actually_burst)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
