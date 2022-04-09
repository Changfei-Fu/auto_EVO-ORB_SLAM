# auto_EVO-ORB_SLAM3
This project use evo to evaluate ORB_SLAM3 on several datasets such as EuRoc, TUM, ICL-NUIM and Kitti. Default settings allow this program to automatically run ORBSLAM3 on every sequence for 10 times and give out the results consisting of files named KeyFrameTrajectory.txt, figures of trajectories, mean time used to run on this sequence, mean tracking rate and mean ape. It should be pointed out that the metric named Tracking Rate is used to describe the percentage that the longest trajectory can cover the sequence. 
# usage
## 1. At first you need to compile the provided ORB_SLAM3 library.

cd ORB_SLAM3

sh ./build.sh

## 2. At the second step you do nothing apart from running the python Scripts.

Note that there should be folders of sequences in the folder named Tum/folders.

## Examples
#### 1). ICL-NUIM dataset
cd ./auto_EVO-ORB_SLAM3/ICL-NUIM/

python3 evo_ICL-NUIM.py ~/dataset/ICL-NUIM/folders/

#### 2). Tum dataset
cd ./auto_EVO-ORB_SLAM3/Tum/

python3 evo_Tum.py ~/dataset/Tum/folders/

#### 3). EuRoc dataset
cd ./auto_EVO-ORB_SLAM3/EuRoc/

python3 evo_EuRoc.py ~/dataset/EuRoc/folders/

# Specialties
## 1. Resume after interrupted
  If this project is stoped before it finishes 10 times evoluation, the process can be continued next time you want to run it.
## 2. Automatically evoluate new sequences
  This project automatically run ORB_SLAM3 on all sequneces in the folder "~/dataset/<dataset_name>/folders", and after the process is interrupted you can resume it without rerun the finished sequences and finished times in 10 times. In addition, if you add new sequence into the folder, it will automatically proceed the new sequnces without waste time on the finished sequences.
