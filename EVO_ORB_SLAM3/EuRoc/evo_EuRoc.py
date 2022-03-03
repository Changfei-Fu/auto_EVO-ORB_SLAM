from ctypes import sizeof
from email.policy import default
from pickle import NONE
import pprint
from unittest import result
import numpy as np
import os

#第一步先通过sys这个模块来添加evo的地址，再import evo
import sys
evo_path=os.path.abspath("../../evo")
sys.path.append(evo_path)
import evo
#第一步先通过sys这个模块来添加evo的地址，再import evo

from evo.tools import file_interface
#traj_ref = file_interface.read_euroc_csv_trajectory(ref_file)
#traj_est = file_interface.read_tum_trajectory_file(est_file)
from evo.core import metrics
import time

def kft_Gen(data_Path,Index):
    data_Name = os.path.basename(data_Path)
    results_Path="./Results/"+data_Name     
    time3=0
    if not os.path.exists(results_Path+"/KeyFrameTrajectory"+str(Index)+".txt"):
        orbslam_Path="../../ORB_SLAM3"
        orbslam=orbslam_Path + "/Examples/Monocular/mono_euroc_evo"
        orbslam_Voc=orbslam_Path+"/Vocabulary/ORBvoc.txt"
        Intrinsics="./EuRoC.yaml"

        cmd = orbslam+" "+orbslam_Voc+" "+Intrinsics+" "+ data_Path
        time1=time.time()
        os.system(cmd)
        time2=time.time()
        time3=time2-time1
        cmd="mv "+data_Path+"/KeyFrameTrajectory.txt ./KeyFrameTrajectory.txt"
        os.system(cmd)   
        os.system("./modify_Precision")
        cmd="mv ./KeyFrameTrajectory.txt "+results_Path+"/KeyFrameTrajectory"+str(Index)+".txt"
        os.system(cmd)   
    return time3

def prepare(results_Path,Index):
    #指定estimated和refered文件
    ref_file = results_Path+'/groundtruth.csv'
    est_file = results_Path+'/KeyFrameTrajectory'+str(Index)+'.txt'
    print(est_file)
    traj_ref = file_interface.read_euroc_csv_trajectory(ref_file)
    traj_est = file_interface.read_tum_trajectory_file(est_file)

    #匹配时间戳-t
    from evo.core import sync
    max_diff = 0.01
    traj_ref, traj_est = sync.associate_trajectories(traj_ref, traj_est, max_diff)
    #对齐轨迹-a -s
    import copy
    traj_est_aligned = copy.deepcopy(traj_est)
    traj_est_aligned.align(traj_ref, correct_scale=True, correct_only_scale=False)
    traj_est=traj_est_aligned
    trajectorys=[traj_ref,traj_est]
    return trajectorys

def ape(Trajectorys) :
    (traj_ref,traj_est)=Trajectorys
    #计算
    pose_relation = metrics.PoseRelation.translation_part
    data = (traj_ref, traj_est)
    ape_metric = metrics.APE(pose_relation)
    ape_metric.process_data(data)
    title = str(ape_metric)
    title += "\n(with Sim(3) Umeyama alignment)"
    ape_result = ape_metric.get_result()
    ape_result.info["title"] = title
    ape_result.add_trajectory("reference",traj_ref)
    ape_result.add_trajectory("estimate",traj_est)
    ape_rmse=ape_metric.get_statistic(metrics.StatisticsType.rmse)
    
    return ape_result,ape_rmse

def ape_plot(data_Path,Index,ape_result,Trajectorys):
    (traj_ref,traj_est)=Trajectorys
    #画图
    from evo.tools import plot
    from evo.tools.settings import SETTINGS
    import matplotlib.pyplot as plt
    import numpy as np

    fig2 = plt.figure(figsize=SETTINGS.plot_figsize)
    plot_mode=plot.PlotMode.xyz
    ax = plot.prepare_axis(fig2, plot_mode)
    plot.traj(ax, plot_mode, traj_ref,
              style=SETTINGS.plot_reference_linestyle,
              color=SETTINGS.plot_reference_color, label='reference',
              alpha=SETTINGS.plot_reference_alpha)
    plot.draw_coordinate_axes(ax, traj_ref, plot_mode,
                                SETTINGS.plot_axis_marker_scale)
    plot.traj_colormap(ax, traj_est, ape_result.np_arrays["error_array"],
                       plot_mode, min_map=ape_result.stats["min"],
                       max_map=ape_result.stats["max"],
                       title="Error mapped onto trajectory")
    plot.draw_coordinate_axes(ax, traj_est, plot_mode,
                              SETTINGS.plot_axis_marker_scale)
    if SETTINGS.plot_pose_correspondences:
        plot.draw_correspondence_edges(
            ax, traj_est, traj_ref, plot_mode,
            style=SETTINGS.plot_pose_correspondences_linestyle,
            color=SETTINGS.plot_reference_color,
            alpha=SETTINGS.plot_reference_alpha)
    fig2.axes.append(ax)

    plot_collection = plot.PlotCollection(ape_result.info["title"])
 #   plot_collection.add_figure("raw", fig2)
    plot_collection.add_figure("map", fig2)

#    plot_collection.show()
    data_Name = os.path.basename(data_Path)
    plot_collection.export('./Results/'+data_Name+'/plot'+str(Index),confirm_overwrite=True)
    plt.close()
    return

def evo_data(data_Path,times):#输入某个数据集地址
    data_Name = os.path.basename(data_Path)
    results_Path="./Results/"+data_Name
    if not os.path.exists("./tracking_Rate") or \
       not os.path.exists("./modify_Precision")  :
        if not os.path.exists("./build"):
            os.system("mkdir build")
        os.system('sh ./build.sh')


    if not os.path.exists('./Results'):
        os.system("mkdir ./Results")
    if not os.path.exists(results_Path):
        os.system("mkdir "+results_Path)


    if not os.path.exists(results_Path+"/rgb.csv"):
        os.system("cp "+data_Path+"/mav0/cam0/data.csv "+results_Path+"/rgb.csv")
    if not os.path.exists(results_Path+"/groundtruth.csv"):
        os.system("cp "+data_Path+"/mav0/state_groundtruth_estimate0/data.csv "+results_Path+"/groundtruth.csv")
    
    if not os.path.exists(results_Path+"/Results.txt"):
        os.system("touch "+results_Path+"/Results.txt")
    else:
        data_results=open(results_Path+"/Results.txt","w")
        print("",end="",file=data_results)
    if not os.path.exists(results_Path+"/time.txt"):   
        os.system("touch "+results_Path+"/time.txt")
        data_time=open(results_Path+"/time.txt","w")
        for j in range(0,times):
            print("0",file=data_time)
        data_time.close()
    
    tracking_Rate=np.zeros(times)
    rmse=np.zeros(times)
    time=np.zeros(times)
    ape_Result={}       
    import subprocess
    for i in range(0,times):
        time[i]=kft_Gen(data_Path,i+1)
        Trajectorys=prepare(results_Path,i+1)
        ape_Result[i],rmse[i]=ape(Trajectorys)
        ape_plot(data_Path,i+1,ape_Result[i],Trajectorys)
        data=open(results_Path+"/Results.txt","a")
        print(str(i+1)+"th ape is: "+str(rmse[i]),file=data)
        data.close()


        lines=[]
        data_time=open(results_Path+"/time.txt","r")
        lines=data_time.readlines()
        if not time[i]==0:
            lines[i]=str(time[i])+'\n'
        data_time.close()
        data_time=open(results_Path+"/time.txt","w")
        for j in range(0,times):
            time[j]=float(lines[j])
            print(time[j],file=data_time)
        data_time.close()                    

        cmd="./tracking_Rate "+results_Path+"/rgb.csv "+results_Path+"/KeyFrameTrajectory"+str(i+1)+".txt "\
             +results_Path+"/Results.txt"
        tracking_Rate[i] = subprocess.getoutput(cmd)
    sum_ape=sum(rmse)
    sum_tracking_Rate=sum(tracking_Rate)
    sum_time=sum(time)

    data=open(results_Path+"/Results.txt","a")
    print("mean ape is: "+str(sum_ape/times),file=data)
    print("mean tracking_Rate is: "+str(sum_tracking_Rate/times)+"%",file=data)
    print("mean costed time is: "+str(sum_time/times),file=data)
    data.close()


dataset_Path=sys.argv[1]
data_Name=os.listdir(dataset_Path)
data_Name.sort()
data_path={}
for i in range(0,len(data_Name)):
    print(data_Name[i]+":")
    data_path[i]=dataset_Path+"/"+data_Name[i]
    evo_data(data_path[i],10)
#python evo_EuRoc.py ~/dataset/EuRoc/folders
#argv[1]输入dataset的目录，该程序会读取文件夹内全部sequence文件夹的地址，对每个sequence执行evo_data
#在evo_data函数中，先检测并创建：
# "mkdir ./Results"
# "touch ./Results/dataname: rgb.txt groundtruth.txt time.txt Results.txt"
# "进入kft_Gen函数获得KeyFrameTrajectory.txt，之后画图计算ape和时间"
# "如果执行到中途进程中断，重新执行可以继续，但是不能删除KeyFrameTrajectory[i].txt和time.txt"
# 其余都可以删除。