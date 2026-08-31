#!/usr/bin/env python3
"""Inspect a ROS1 bag: list connections, types, message counts, time range."""
import sys
from pathlib import Path

from rosbags.rosbag1 import Reader

path = Path(sys.argv[1] if len(sys.argv) > 1 else '/home/jj/P_data/2026-03-05-16-54-49.bag')

with Reader(path) as reader:
    print(f"bag: {path}")
    print(f"duration: {reader.duration:.1f}s  start={reader.start_time}  end={reader.end_time}")
    print(f"message_count: {reader.message_count}")
    print(f"{'id':>3}  {'type':<40} topic")
    for c in sorted(reader.connections, key=lambda c: c.id):
        print(f"{c.id:>3}  {c.msgtype:<40} {c.topic}")
