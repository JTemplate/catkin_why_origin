#!/usr/bin/env python3
"""Count messages on topics, subscribing with BOTH reliable and best-effort QoS."""
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import PointCloud2
from tf2_msgs.msg import TFMessage
from visualization_msgs.msg import MarkerArray

from ros_plc.msg import LidarMsgArray, RadarMsgArray

TOPIC_TYPES = {
    "/rslidar_points": PointCloud2,
    "/ti_mmwave/radar_scan_pcl_0": PointCloud2,
    "/tf": TFMessage,
    "/ground_cut_output_cloud_topic": PointCloud2,
    "/x_cut_output_cloud_topic": PointCloud2,
    "/y_cut_output_cloud_topic": PointCloud2,
    "/output_cloud": PointCloud2,
    "/lidar_perception/cluster_points": PointCloud2,
    "/radar_perception/cluster_points": PointCloud2,
    "/mmwaveFiltered": PointCloud2,
    "/lidar_perception/marker_track": MarkerArray,
    "/lidar_perception/marker_info": MarkerArray,
    "/radar_perception/marker_info": MarkerArray,
    "/lidar_objects": LidarMsgArray,
    "/radar_objects": RadarMsgArray,
    "/fusion_markers": MarkerArray,
    "/fusion_markers_text": MarkerArray,
}


class FlowCounter(Node):
    def __init__(self, topics, duration):
        super().__init__("flow_counter")
        self.duration = duration
        self.counts = {(t, q): 0 for t in topics for q in ("be", "rel")}
        for topic in topics:
            msg_type = TOPIC_TYPES.get(topic, PointCloud2)
            for qos_name, reliability in (("be", ReliabilityPolicy.BEST_EFFORT),
                                          ("rel", ReliabilityPolicy.RELIABLE)):
                qos = QoSProfile(depth=20, reliability=reliability)
                self.create_subscription(msg_type, topic, self._make_cb(topic, qos_name), qos)

    def _make_cb(self, topic, qos_name):
        def cb(msg):
            self.counts[(topic, qos_name)] += 1
        return cb

    def run(self):
        end = time.time() + self.duration
        while time.time() < end:
            rclpy.spin_once(self, timeout_sec=0.5)
        for (topic, qos_name), n in sorted(self.counts.items()):
            if n:
                print(f"{n:>6} msgs  {topic}  [{qos_name}]")


def main():
    rclpy.init()
    topics = sys.argv[1].split(",")
    duration = float(sys.argv[2]) if len(sys.argv) > 2 else 10.0
    node = FlowCounter(topics, duration)
    node.run()
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
