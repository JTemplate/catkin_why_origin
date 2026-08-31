from setuptools import find_packages
from setuptools import setup

setup(
    name='ros_plc',
    version='0.1.0',
    packages=find_packages(
        include=('ros_plc', 'ros_plc.*')),
)
