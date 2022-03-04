/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2020 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/



#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings path_to_sequence" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    string strFile = string(argv[3])+"/rgb.txt";
    LoadImages(strFile, vstrImageFilenames, vTimestamps);
    // 取出rgb.txt中的文本，把图片名和时间戳分别存在一个容器中。

    int nImages = vstrImageFilenames.size();

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::MONOCULAR,true);
    //第一个参数是词典，第二个参数是settingfile，第三个参数表示单目。
    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    // Main loop
    cv::Mat im;
    for(int ni=0; ni<nImages; ni++)//每张图片
    {
        // Read image from file

        im = cv::imread(string(argv[3])+"/"+vstrImageFilenames[ni],cv::IMREAD_UNCHANGED);
        double tframe = vTimestamps[ni];

        if(im.empty())//判断图像加载成功
        {
            cerr << endl << "Failed to load image at: "
                 << string(argv[3]) << "/" << vstrImageFilenames[ni] << endl;
            return 1;
        }

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the image to the SLAM system
        SLAM.TrackMonocular(im,tframe);//tframe是当前帧时间戳

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
        //ttrack是SLAM.TrackMonocular(im,tframe)的时间
        vTimesTrack[ni]=ttrack;//vTimesTrack中保存每一帧的跟踪时间

        // Wait to load the next frame， 根据时间戳，等待一定时间后再接收下一帧
        double T=0;
        if(ni<nImages-1)//没到最后一帧，就用这一帧到下一帧的时间
            T = vTimestamps[ni+1]-tframe;
        else if(ni>0)//到最后一帧，用前一帧到这一帧的时间
            T = tframe-vTimestamps[ni-1];

        if(ttrack<T)//还没到下一帧的时间，先睡一会儿
            usleep((T-ttrack)*1e6);
    }

    // Stop all threads
    SLAM.Shutdown();



    // Tracking time statistics，获得实际跟踪每一帧消耗时间的数据，包括中间帧时间，平均时间，总时间
    sort(vTimesTrack.begin(),vTimesTrack.end());//vTimesTrack中保存每一帧的跟踪时间
    float totaltime = 0;
    for(int ni=0; ni<nImages; ni++)
    {
        totaltime+=vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;//中间那一帧跟踪时间
    cout << "mean tracking time: " << totaltime/nImages << endl;//平均跟踪时间
    // Tracking time statistics，获得实际跟踪每一帧消耗时间的数据，包括中间帧时间，平均时间，总时间



    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM(string(argv[3])+"/KeyFrameTrajectory.txt");
    cout<<"nImages = "<< nImages <<endl;
    return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());
    while(!f.eof())
    {
        string s;
        getline(f,s);//此函数每次读一行
        if(s[0]=='#') continue;
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}
