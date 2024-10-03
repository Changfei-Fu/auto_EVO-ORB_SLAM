#!/usr/bin/env bash
cd ~/changfei_ws/src/ORB_SLAM3
./Examples/Monocular/mono_tum ./Vocabulary/ORBvoc.txt ./Examples/Monocular/TUM.yaml ~/Downloads/dataset/rgbd_dataset_freiburg1_xyz
