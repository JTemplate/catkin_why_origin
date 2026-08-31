#!/usr/bin/env python3
"""Convert selected topics of a ROS1 bag to a ROS2 (rosbag2 sqlite3) bag.

Messages are DESERIALIZED with the ROS1 type system and RE-SERIALIZED with
the ROS2 type system — ROS1 and ROS2 serializations differ for strings
(ROS1 has no trailing NUL; CDR includes it), so raw pass-through would
produce payloads that subscribers cannot deserialize.

After conversion: patch QoS profiles into the metadata (golden Humble
format), clear the sqlite `metadata` table, and set the schema version to
Humble's so `ros2 bag play` behaves exactly like a native bag.
"""
import shutil
import sqlite3
import sys
from pathlib import Path

from rosbags.interfaces import (
    Qos,
    QosDurability,
    QosHistory,
    QosLiveliness,
    QosReliability,
    QosTime,
)
from rosbags.rosbag1 import Reader as Reader1
from rosbags.rosbag2 import Writer as Writer2
from rosbags.serde import deserialize_ros1, serialize_ros2
from rosbags.typesys import Stores, get_typestore
from ruamel.yaml import YAML

src = Path(sys.argv[1])
dst = Path(sys.argv[2])

ALLOWED = {
    "/rslidar_points": "sensor_msgs/msg/PointCloud2",
    "/ti_mmwave/radar_scan_pcl_0": "sensor_msgs/msg/PointCloud2",
    "/tf": "tf2_msgs/msg/TFMessage",
}

STORE1 = get_typestore(Stores.ROS1_MELODIC)
STORE2 = get_typestore(Stores.ROS2_HUMBLE)

INF_SEC, INF_NSEC = 9223372036, 854775807   # INT64_MAX split, like ros2 bag record


def qos_string(history, depth, reliability, durability):
    lines = [
        f"- history: {history}",
        f"  depth: {depth}",
        f"  reliability: {reliability}",
        f"  durability: {durability}",
    ]
    for t in ("deadline", "lifespan"):
        lines.append(f"  {t}:")
        lines.append(f"    sec: {INF_SEC}")
        lines.append(f"    nsec: {INF_NSEC}")
    lines.append("  liveliness: 1")
    lines.append("  liveliness_lease_duration:")
    lines.append(f"    sec: {INF_SEC}")
    lines.append(f"    nsec: {INF_NSEC}")
    lines.append("  avoid_ros_namespace_conventions: false")
    return "\n".join(lines)


SENSOR_QOS = qos_string(1, 5, 2, 2)   # SensorDataQoS: best-effort, volatile
DEFAULT_QOS = qos_string(1, 10, 1, 2)  # default rclcpp: reliable, volatile


def qos_obj(reliability, depth):
    return Qos(
        history=QosHistory.KEEP_LAST,
        depth=depth,
        reliability=reliability,
        durability=QosDurability.VOLATILE,
        deadline=QosTime(sec=-1, nsec=0),
        lifespan=QosTime(sec=-1, nsec=0),
        liveliness=QosLiveliness.AUTOMATIC,
        liveliness_lease_duration=QosTime(sec=-1, nsec=0),
        avoid_ros_namespace_conventions=False,
    )


SENSOR_QOS_OBJ = qos_obj(QosReliability.BEST_EFFORT, 5)
DEFAULT_QOS_OBJ = qos_obj(QosReliability.RELIABLE, 10)

if dst.exists():
    shutil.rmtree(dst)

counts = {}
with Reader1(src) as r1, Writer2(dst, version=8) as w2:
    seen = set()
    conns = []
    conn_map = {}
    for c in r1.connections:
        if c.topic in ALLOWED and c.topic not in seen:
            seen.add(c.topic)
            conns.append(c)
            ros2type = ALLOWED[c.topic]
            qos = SENSOR_QOS_OBJ if c.topic in ("/rslidar_points", "/ti_mmwave/radar_scan_pcl_0") else DEFAULT_QOS_OBJ
            wconn = w2.add_connection(
                c.topic, ros2type,
                msgdef=c.msgdef.data, rihs01=c.digest,
                offered_qos_profiles=[qos],
            )
            conn_map[c.topic] = (wconn, ros2type)
    for conn, timestamp, raw in r1.messages(connections=conns):
        wconn, ros2type = conn_map[conn.topic]
        msg = deserialize_ros1(raw, ros2type, STORE1)
        data = serialize_ros2(msg, ros2type, STORE2)
        w2.write(wconn, timestamp, data)
        counts[conn.topic] = counts.get(conn.topic, 0) + 1

print(f"converted {sum(counts.values())} messages to {dst}")
for topic, n in sorted(counts.items()):
    print(f"  {n:>8}  {topic}")

# ---- post-process to make the bag indistinguishable from a native Humble bag ----
db = next(dst.glob("*.db3"))
con = sqlite3.connect(db)
# golden Humble qos strings in the topics table (what the player reads)
for row in con.execute("select id, name from topics"):
    tid, name = row
    q = SENSOR_QOS if name in ("/rslidar_points", "/ti_mmwave/radar_scan_pcl_0") else DEFAULT_QOS
    con.execute("update topics set offered_qos_profiles=? where id=?", (q, tid))
# native Humble bags leave the metadata table empty
con.execute("delete from metadata")
# Humble schema version marker
con.execute("delete from schema")
con.execute("insert into schema values (3, 'humble')")
con.commit()
con.close()

yaml_path = dst / "metadata.yaml"
y = YAML()
with yaml_path.open() as f:
    data = y.load(f)
info = data["rosbag2_bagfile_information"]
info["version"] = 5
info["ros_distro"] = "humble"
for t in info["topics_with_message_count"]:
    name = t["topic_metadata"]["name"]
    t["topic_metadata"]["offered_qos_profiles"] = SENSOR_QOS if name in ("/rslidar_points", "/ti_mmwave/radar_scan_pcl_0") else DEFAULT_QOS
with yaml_path.open("w") as f:
    y.dump(data, f)
print("patched qos/schema/metadata for Humble compatibility")
