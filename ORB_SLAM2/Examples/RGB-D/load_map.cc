/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/
#define load_the_map

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>
#include<unistd.h>
#include<System.h>

using namespace std;


int main(int argc, char **argv)
{
    if(argc != 4)
    {
        cerr << endl << "Usage: ./rgbd_tum path_to_vocabulary path_to_settings path_to_sequence path_to_association" << endl;
        return 1;
    }

//~/catkin_ws/src/ORB_SLAM2-master/Examples/RGB-D/load_map ~/catkin_ws/src/ORB_SLAM2/Vocabulary/ORBvoc.txt ~/catkin_ws/src/ORB_SLAM2-master/Examples/RGB-D/TUM1.yaml ~/catkin_ws/src/ORB_SLAM2-master/Examples/RGB-D/map.bin


    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::RGBD,true);
    string str_map_path_string = "/home/administrator/catkin_ws/src/ORB_SLAM2/Examples/Monocular/map.bin";
    SLAM.LoadMap(str_map_path_string);
    sleep(3600);
    // Vector for tracking time statistics




    return 0;
}


