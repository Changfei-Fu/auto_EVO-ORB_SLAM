#!/usr/bin/env bash
cd ~/catkin_ws/src/ORB_SLAM2
./Examples/RGB-D/rgbd_tum ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/TUM4.yaml ~/dataset/rgbd_dataset_freiburg1_xyz ~/dataset/rgbd_dataset_freiburg1_xyz/associate.txt
