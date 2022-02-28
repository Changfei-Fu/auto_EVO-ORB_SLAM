# auto_EVO-ORB_SLAM3
This project use evo to evaluate ORB_SLAM3 on several datasets such as EuRoc, TUM, ICL-NUIM and Kitti. Default settings allow this program to automatically run ORBSLAM3 on every sequence for 10 times and give out the results consisting of files named KeyFrameTrajectory.txt, figures of trajectories, mean time used to run on this sequence, mean tracking rate and mean ape.
# usage
modify the directory paths of ORB_SLAM3 and evo.

replace some files in ORB_SLAM3 with given files.

# Examples
python3 evo_ICL-NUIM.py ~/dataset/ICL-NUIM/folders
python3 evo_TUM.py ~/dataset/Tum/folders
python3 evo_EuRoc.py ~/dataset/EuRoc/folders
