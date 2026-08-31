#!/usr/bin/env python3
"""Patch metadata.yaml of a rosbags-converted bag so `ros2 bag play` (Humble)
can deserialize the QoS profiles.

rosbags 0.11 dumps time fields as `nsec`; Humble's deserialize_qos expects
`nanosec` and fails with 'yaml-cpp: bad conversion'. We rewrite
offered_qos_profiles exactly like `ros2 bag record` writes them.
"""
import sys
from pathlib import Path

from ruamel.yaml import YAML

INF_SEC, INF_NSEC = 9223372036, 854775807   # INT64_MAX split, exactly like ros2 bag record

# topic -> (history, depth, reliability, durability)
QOS = {
    "/rslidar_points": (1, 5, 2, 2),            # SensorDataQoS: best-effort, volatile
    "/ti_mmwave/radar_scan_pcl_0": (1, 5, 2, 2),
    "/tf": (1, 10, 1, 2),                       # default: reliable, volatile
}


def qos_string(history, depth, reliability, durability):
    # byte-for-byte the same shape `ros2 bag record` (Humble) writes:
    #   deadline/lifespan/liveliness_lease_duration use keys `sec`+`nsec`,
    #   key order: ..., liveliness, liveliness_lease_duration, ...
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


def main():
    bagdir = Path(sys.argv[1])
    yaml_path = bagdir / "metadata.yaml"
    db_path = next(bagdir.glob("*.db3"))

    # 1) metadata.yaml
    yaml = YAML()
    with yaml_path.open() as f:
        data = yaml.load(f)

    topics = data["rosbag2_bagfile_information"]["topics_with_message_count"]
    for entry in topics:
        md = entry["topic_metadata"]
        name = md["name"]
        if name in QOS:
            md["offered_qos_profiles"] = qos_string(*QOS[name])
            print(f"patched yaml qos: {name}")

    with yaml_path.open("w") as f:
        yaml.dump(data, f)
    print(f"wrote {yaml_path}")

    # 2) sqlite3 topics table (what the player actually reads)
    import io
    import sqlite3

    con = sqlite3.connect(db_path)
    for row in con.execute("select id, name from topics"):
        tid, name = row
        if name in QOS:
            con.execute(
                "update topics set offered_qos_profiles=? where id=?",
                (qos_string(*QOS[name]), tid),
            )
            print(f"patched db qos: {name}")

    # 3) sqlite3 `metadata` table: native Humble bags leave this EMPTY and the
    #    player falls back to metadata.yaml / topics table. Mirror that.
    con.execute("delete from metadata")
    print("cleared db metadata table")

    con.commit()
    con.close()
    print(f"wrote {db_path}")


if __name__ == "__main__":
    main()
