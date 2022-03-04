# auto_EVO-ORB_SLAM3
This project use evo to evaluate ORB_SLAM3 on several datasets such as EuRoc, TUM, ICL-NUIM and Kitti. Default settings allow this program to automatically run ORBSLAM3 on every sequence for 10 times and give out the results consisting of files named KeyFrameTrajectory.txt, figures of trajectories, mean time used to run on this sequence, mean tracking rate and mean ape.
# usage
## 1. At first you need to compile the provided ORB_SLAM3 library.

cd ORB_SLAM3

sh ./build.sh

## 2. At the second step you do nothing apart from running the python programs.

### Examples
python3 ./auto_EVO-ORB_SLAM3/evo_ICL-NUIM.py ~/dataset/ICL-NUIM/folders

python3 ./auto_EVO-ORB_SLAM3/evo_TUM.py ~/dataset/Tum/folders

python3 ./auto_EVO-ORB_SLAM3/evo_EuRoc.py ~/dataset/EuRoc/folders

# Specialty
