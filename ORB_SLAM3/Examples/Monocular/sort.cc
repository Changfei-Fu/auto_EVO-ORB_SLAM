#include<iostream>
#include<fstream>
#include<opencv2/core/core.hpp>


using namespace std;

void LoadImages(const string &strFile, vector<double> &vTimestamps);

int main(int argc, char** argv)
{
	vector<double> vTimestamps_KFT;
	vector<double> vTimestamps_rgb;
	if(argc!=2){
        LoadImages("./rgb.txt",vTimestamps_rgb);
        LoadImages("./KeyFrameTrajectory.txt",vTimestamps_KFT);
	}else{
        LoadImages(string(argv[1])+"/rgb.txt",vTimestamps_rgb);
        LoadImages(string(argv[1])+"/KeyFrameTrajectory.txt",vTimestamps_KFT);
	}
    int rgb_size=vTimestamps_rgb.size();
    int KFT_size=vTimestamps_KFT.size();
    int start(0),end(0);
    for(int i = 0; i < rgb_size ;i++ ){
        if(vTimestamps_rgb[i]==vTimestamps_KFT[0]){
            start=i;
        }
        if(vTimestamps_rgb[i]==vTimestamps_KFT[KFT_size-1]){
            end=i;
        }
    }
    int data_num=rgb_size;
    int track_num= end-start+1;
    double data_time=vTimestamps_rgb[rgb_size-1]-vTimestamps_rgb[0];
    double track_time=vTimestamps_KFT[KFT_size-1]-vTimestamps_KFT[0];
    double num_rate=100*double(track_num)/double(data_num);
    double time_rate=100*track_time/data_time;
    cout<<"轨迹起始时间： ["<<std::fixed<<vTimestamps_KFT[0]<<"],对应数据集第["<<std::fixed<<(start+1)<<"]帧时间: ["<<std::fixed<<vTimestamps_rgb[start]<<"]"<<endl;
    cout<<"轨迹结束时间： ["<<std::fixed<<vTimestamps_KFT[KFT_size-1]<<"],对应数据集第["<<(end+1)<<"]帧时间： ["<<std::fixed<<vTimestamps_rgb[end]<<"]"<<endl;
    cout<<"最长子图跟踪： ["<<std::fixed<<track_num<<"]帧图像, 数据集共有["<<rgb_size<<"]帧图像, 占比["<< num_rate<<"%]"<<endl;
    cout<<"最长子图跟踪时长: ["<<std::fixed<<track_time<<"s]，数据集时长: ["<< std::fixed<<data_time <<"s], 占比["<< time_rate<<"%]"<<endl;


    return 0;
}



void LoadImages(const string &strFile, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());
    while(!f.eof())
    {
        string s;
        getline(f,s);//此函数每次读一行
        if(!s.empty())
        {
            if(s[0]=='#')
                continue;
            stringstream ss;
            ss << s;
            double t;
            ss >> t;
            vTimestamps.push_back(t);
            continue;
        }
    }

}
