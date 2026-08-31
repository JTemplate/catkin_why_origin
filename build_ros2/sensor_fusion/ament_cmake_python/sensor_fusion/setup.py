from setuptools import find_packages
from setuptools import setup

setup(
    name='sensor_fusion',
    version='0.1.0',
    packages=find_packages(
        include=('sensor_fusion', 'sensor_fusion.*')),
)
