#!/usr/bin/env bash
cd ~/catkin_ws/src/ORB_SLAM3
./Examples/Monocular/mono_euroc_copy ./Vocabulary/ORBvoc.txt ~/dataset/EuRoC/MH_01_easy/mav0/cam0/sensor.yaml ~/dataset/EuRoC/MH_01_easy ~/dataset/EuRoC/MH_01_easy/mav0/cam0/data.csv
