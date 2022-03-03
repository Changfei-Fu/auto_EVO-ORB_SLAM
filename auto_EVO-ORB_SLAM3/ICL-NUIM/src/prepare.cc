#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
using namespace std;
void change_rgbtxt(const string &srcFile, const string &dstFile,int num){
    ifstream f_i;
    ofstream f_o;
    f_i.open(srcFile.c_str());
    f_o.open(dstFile.c_str());
    int i(0);
    while(!f_i.eof())
    {
        if(i==num){
            break;
        }
        string s;
        getline(f_i,s);//此函数每次读一行,存在S中
        if(!s.empty())
        {
            if(s[0]=='#')
                continue;
            else{
                f_o << s <<endl;
                i++;
            }
        }
    }
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
            //cout<<std::fixed<<t<<endl;
            vTimestamps.push_back(t);
            continue;
        }
    }

}
void WriteImages(const string &strFile, vector<double> &vTimestamps)
{


    ifstream f_i;
	ofstream f_o;
    f_i.open(strFile.c_str());
	f_o.open("./groundtruth.txt");
	int lines(0);
    while(!f_i.eof())
    {
        vector<string> String(8);
		string s;
        getline(f_i,s);//此函数每次读一行
        if(!s.empty())
        {
            if(s[0]=='#')
                continue;
			stringstream ss;
            ss << s;
			for(int i=0;i<8;i++){
				ss >> String[i];	
			}
            stringstream sss;
            sss<<std::fixed<<vTimestamps[lines++];
			sss>>String[0];
			//cout<<String[0]<<endl;
			for(int i=0;i<7;i++){
				f_o<<String[i]<<" ";
                //cout<<String[i]<<"";
			}f_o<<String[7]<<endl;
        }
    }

}

int main(int argc, char** argv)
{
    int Image_num(0),num(0);
    //cv::Mat img0=cv::imread("../rgb/0.png",cv::IMREAD_UNCHANGED);
    // while(1){
    //     num++;
    //     cv::Mat img=cv::imread("./rgb/"+to_string(num)+".png",cv::IMREAD_UNCHANGED);
    //     if(img.empty()){
    //         num--;
    //         break;
    //     }
    //     //cout<<num<<endl;
    // }
    stringstream ss;
    ss<<argv[1];
    ss>>Image_num;
//    if(img0.empty()) {//从1开始
//        Image_num=num;
//    }else{
//        Image_num=num+1;
//    }

    rename("./rgb.txt", "./RGB.txt");
    change_rgbtxt("./RGB.txt","./rgb.txt",Image_num);//删除TUM中rgb.txt的多余部分
    remove("./RGB.txt");
    vector<double> vTimestamps_rgb;
    LoadImages("./rgb.txt",vTimestamps_rgb);//取出rgb.txt时间戳

//    for(auto it: vTimestamps_rgb){
//        cout<<std::fixed<<it<<endl;
//    }
    rename("./groundtruth.txt", "./ground_truth.txt");
    WriteImages("./ground_truth.txt",vTimestamps_rgb);//修改GT时间戳
    remove("./ground_truth.txt");
    return 0;
}
