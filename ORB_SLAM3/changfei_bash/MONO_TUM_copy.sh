#!/usr/bin/env bash
cd ~/catkin_ws/src/ORB_SLAM3
./Examples/Monocular/mono_tum_copy ./Vocabulary/ORBvoc.txt ./Examples/RGB-D/TUM3.yaml ~/dataset/weinan_dataset/rgbd_dataset_freiburg2_pioneer_360
