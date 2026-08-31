#!/usr/bin/env python3
"""Shift all message timestamps of a rosbag2 (sqlite3) bag by a delta."""
import shutil
import sqlite3
import sys
from pathlib import Path

from ruamel.yaml import YAML

bagdir = Path(sys.argv[1])
delta = int(sys.argv[2])  # nanoseconds

db = next(bagdir.glob("*.db3"))
con = sqlite3.connect(db)
con.execute("update messages set timestamp = timestamp + ?", (delta,))
con.commit()
con.close()

yaml_path = bagdir / "metadata.yaml"
y = YAML()
with yaml_path.open() as f:
    data = y.load(f)
info = data["rosbag2_bagfile_information"]
info["starting_time"]["nanoseconds_since_epoch"] += delta
for f in info["files"]:
    f["starting_time"]["nanoseconds_since_epoch"] += delta
with yaml_path.open("w") as f:
    y.dump(data, f)
print(f"shifted by {delta} ns")
