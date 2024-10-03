#!/usr/bin/env bash
source ~/changfei_ws/devel/setup.bash
cd ~/changfei_ws/src/ORB_SLAM3/
rosrun ORB_SLAM3 Mono ~/changfei_ws/src/ORB_SLAM3/Vocabulary/ORBvoc.txt ~/changfei_ws/src/ORB_SLAM3/Examples/ROS/ORB_SLAM3/TUM4.yaml

