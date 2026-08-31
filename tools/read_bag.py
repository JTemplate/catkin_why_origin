#!/usr/bin/env python3
"""Read a rosbag2 bag via rosbag2_py.SequentialReader and count messages."""
import sys

from rosbag2_py import ConverterOptions, SequentialReader, StorageOptions

uri = sys.argv[1]
reader = SequentialReader()
reader.open(
    StorageOptions(uri=uri, storage_id="sqlite3"),
    ConverterOptions(input_serialization_format="cdr", output_serialization_format="cdr"),
)
topic_types = reader.get_all_topics_and_types()
print("topics:", [(t.name, t.type) for t in topic_types])
n = 0
while reader.has_next():
    item = reader.read_next()
    n += 1
    if n <= 3:
        if isinstance(item, tuple):
            meta, msg = item
        else:
            msg = item
        print(f"  msg {n}: topic={msg.topic_name} ts={msg.timestamp} size={len(msg.serialized_data)}")
print("total messages readable:", n)
