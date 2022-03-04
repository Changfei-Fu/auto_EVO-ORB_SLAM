#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include <iomanip>

using namespace std;

int LoadImages(const string &strFile, vector<uint64_t> &vTimestamps);
void SearchImages(const string &strFile, vector<uint64_t> &vTimestamps, int &start, int &end);

int main(int argc, char** argv)
{
    vector<uint64_t> vTimestamps_KFT;
    vector<uint64_t> vTimestamps_rgb;
    int rgb_size(0);
    int KFT_size(0);

    rgb_size=LoadImages(argv[1],vTimestamps_rgb);
    KFT_size=LoadImages(argv[2],vTimestamps_KFT);
    /*else{
        rgb_size=LoadImages(string(argv[1])+"/data.txt",vTimestamps_rgb);
        KFT_size=LoadImages(string(argv[1])+"/KFT.txt",vTimestamps_KFT);
    }*/
    int start(0),end(0);
    SearchImages(argv[1], vTimestamps_KFT, start, end);//获得KFT.txt的起点和终点在rgb.txt中的对应位次
/*    for(int i = 0; i < rgb_size ;i++ ){
        if(vTimestamps_rgb[i]==vTimestamps_KFT[0]){
            start=i;
        }
        if(vTimestamps_rgb[i]==vTimestamps_KFT[KFT_size-1]){
            end=i;
        }
    }*/
    int data_num=rgb_size;
    int track_num= end-start+1;
    uint64_t data_time=vTimestamps_rgb[1]-vTimestamps_rgb[0];
    uint64_t track_time=vTimestamps_KFT[1]-vTimestamps_KFT[0];
    double num_rate=100*double(track_num)/double(data_num);
    double time_rate=100*double(track_time)/double(data_time);
    ofstream of;
    of.open(argv[3],ios::in|ios::app);
    of<<"starting time is： ["<<vTimestamps_KFT[0]<<" us],refer to ["<<std::fixed<<(start+1)<<"]th frame"<<endl;
    of<<"ending time is： ["<<vTimestamps_KFT[1]<<" us],refer to ["<<(end+1)<<"]th frame"<<endl;
    of<<"longest submap tracked ： ["<<std::fixed<<track_num<<"] frames , and there are totally ["<<rgb_size<<"] frames, account for ["<< num_rate<<"%] "<<endl;
    of<<"ongest submap tracked : ["<<track_time<<" us]，and the dataset time is : ["<<data_time <<" us], account for["<< time_rate<<"%]"<<endl;
    of<<endl;

    of.close();
    cout<<std::fixed<<time_rate<<endl;

    return 0;
}



int LoadImages(const string &strFile, vector<uint64_t> &vTimestamps)//此函数只取第一帧和最后一帧的时间戳
{
    bool start(true);
    int SIZE(0);
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
            int i=0;
            while(s[i]!=',' && s[i]!='\0'){
                i++;
            }
            stringstream ss;
            if(s[i]==','){//euroc的rgb.csv以逗号分隔，其余都是以空格分隔
                s[i]=' ';
                ss << s;
                uint64_t t;
                ss >> t;
                if(start){
                    start=false;
                    vTimestamps.push_back(t);
                }else{
                    if(vTimestamps.size()>=2){
                        vTimestamps.pop_back();
                    }
                    vTimestamps.push_back(t);
                }
            }else{//tum
                ss << s;
                long double t;
                uint64_t t1;
                ss >> t;
                t1=t*1e9;
                if(start){
                    start=false;
                    vTimestamps.push_back(t1);

                }else{
                    if(vTimestamps.size()==2){
                        vTimestamps.pop_back();
                    }
                    vTimestamps.push_back(t1);
                }
            }
            SIZE++;
            continue;
        }
    }
    return SIZE;
}

void SearchImages(const string &strFile, vector<uint64_t> &vTimestamps, int &start, int &end)
{
    start=-1;
    end=-1;
    int Index=0;
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
            int i(0);
            while(s[i]!=',' && s[i]!='\0'){
                i++;
            }
            if(s[i]==',')
                s[i]=' ';
            stringstream ss;
            ss << s;
            long double t;
            ss >> t;
            if( uint64_t(t*1e9) ==uint64_t(vTimestamps[0]) ){
                start=Index;
            }
            if( uint64_t(t*1e9) ==uint64_t(vTimestamps[1]) ){
                end=Index;
            }
            Index++;
            continue;
        }
    }
}
