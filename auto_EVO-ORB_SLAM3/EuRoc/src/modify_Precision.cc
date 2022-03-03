#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;
int change_datacsv(const string &srcFile, vector<string> &lines, int precision){
    ifstream f_i;
    ofstream f_o;
    f_i.open(srcFile.c_str());
    int lines_num(0);
    while(!f_i.eof())
    {
        string s,s1;
        stringstream ss;
        getline(f_i,s);//此函数每次读一行,存在S中
        if(!s.empty())
        {
            if(s[0]=='#')
                continue;
            lines_num++;
            int i(0);

            while(s[i]!=' '){
                i++;
            }
            s1=s.substr(0,i);

            long double t;
            ss<<s1.substr(6);//单一参数表示取后续全部
            ss>>t;
            t*=1e-9;//加小数点


            ss.clear();
            ss.str("");
            ss<<s1.substr(0,6)<<std::fixed<<setprecision(precision)<<t;
            s1="";
            ss>>s1;
            s.replace(0,i,s1);

            ss.clear();
            ss.str("");
            ss << s << endl;
            lines.push_back(ss.str());
        }
    }
    return lines_num;
}


int main(int argc, char** argv)
{
    vector<string> lines;
    int lines_num= change_datacsv("KeyFrameTrajectory.txt", lines, 9);
    fstream f_o;
    f_o.open("KeyFrameTrajectory.txt",ios::out);
    for(int i=0 ;i <lines_num;i++){
        f_o << lines[i];
    }
    f_o.close();
    
    return 0;
}
