#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include <iomanip>

#include<opencv2/opencv.hpp>
#include<Eigen/Eigen>

using namespace std;
using namespace cv;


void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps);
vector<double> checkFundamental(vector<DMatch> matches, vector<KeyPoint> keyPoints_1, vector<KeyPoint> keyPoints_2);
vector<Point2d> distort(vector<Point2d> Points);

int data_Associate(Mat& frame1, Mat& frame2){


    vector<KeyPoint> keypoints_1, keypoints_2;
    Ptr<FeatureDetector> detector = ORB::create(1000);
    detector->detect(frame1, keypoints_1);
    detector->detect(frame2, keypoints_2);


    Mat outimg1,outimg2;
    drawKeypoints(frame1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("ORB features1", outimg1);
    drawKeypoints(frame2, keypoints_2, outimg2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("ORB features2", outimg2);


    Mat descriptors_1, descriptors_2;
    Ptr<DescriptorExtractor> descriptorExtractor = ORB::create(1000);
    descriptorExtractor->compute(frame1, keypoints_1, descriptors_1);
    descriptorExtractor->compute(frame2, keypoints_2, descriptors_2);

    vector<DMatch> matches;
    int nmatches(0);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors_1, descriptors_2, matches);
    auto min_max = minmax_element(matches.begin(), matches.end(),
                                [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
    double min_dist = min_max.first->distance;
    double max_dist = min_max.second->distance;

    cout<<"max_dist = "<<max_dist<<" and min_dist = "<<min_dist<<endl;
    std::vector<DMatch> good_matches;
    for (int i = 0; i < descriptors_1.rows; i++) {
        /*if(matches[i].distance<50){
            if()
        }*/
        if (matches[i].distance <= 25 ) {
            good_matches.push_back(matches[i]);
        }
    }cout<<"good_matches.size():= "<<good_matches.size()<<endl;


    std::vector<DMatch> inlier_matches;

    vector<double> score = checkFundamental(good_matches,keypoints_1,keypoints_2);

    for(int i=0; i<score.size(); i++){
        cout<<score[i]<<endl;
        if(fabs(score[i])<3){
            inlier_matches.push_back(good_matches[i]);
        }
    }cout<<"inlier_matches.size():= "<<inlier_matches.size()<<endl;



    if(inlier_matches.size()==0){
        return 0;
    }
    Mat img_match;
    drawMatches(frame1, keypoints_1, frame2, keypoints_2, inlier_matches, img_match);

    imshow("all matches", img_match);

    /*for(int i=0; i<score.size(); i++){
        cout<<"score["<<i<<"]"<<std::fixed<<score[i]<<endl;
    }*/
    // for(int i=0; i<matches.size(); i++){
    //     //cout<<matches[i].distance<<endl;
    //     if(matches[i].distance)
    // }
    //waitKey(0);


    return inlier_matches.size();
}

int main(int argc, char** argv)
{
    fstream fs;
    fs.open("./data.txt",ios::out);
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    LoadImages(argv[1], vstrImageFilenames, vTimestamps);

    int nImages = vstrImageFilenames.size();
    Mat im1,im2;
    int inlier_matches(0);
    for(int ni=0; ni<nImages; ni++){
        if(ni==72){
            im1 = cv::imread(string(argv[2])+"/"+vstrImageFilenames[ni],cv::IMREAD_UNCHANGED);
            for(int nj=0; nj<nImages; nj++){
                im2 = cv::imread(string(argv[2])+"/"+vstrImageFilenames[nj],cv::IMREAD_UNCHANGED);

                inlier_matches=data_Associate(im1, im2);
                cout<<ni<<"vs"<<nj<<" inlier_matches:="<<inlier_matches<<endl;
                fs<<inlier_matches;
                if(nj<nImages-1){
                    fs<<" ";
                }
            }
            fs<<endl;
        }
    }
    fs.close();
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


vector<double> checkFundamental(vector<DMatch> matches, vector<KeyPoint> keyPoints_1, vector<KeyPoint> keyPoints_2){
    vector<Point2d> points1;
    vector<Point2d> points2;
    vector<double> score;

    for (int i = 0; i < (int) matches.size(); i++) {
        points1.push_back(keyPoints_1[matches[i].queryIdx].pt);
        points2.push_back(keyPoints_2[matches[i].trainIdx].pt);
    }

    points1=distort(points1);
    points2=distort(points2);


    cout<<"Points.size()= "<<points1.size()<<endl;
    if(points1.size()<=15 || points2.size()<=15){
        for (int i = 0; i < (int) matches.size(); i++) {
            score.push_back(1000);
        }
        return score;
    }



    Mat f_m=  findFundamentalMat(points1, points2, FM_RANSAC,3,0.99,50);

    Eigen::Matrix<double, 3, 3> F;

    F<< f_m.at<double>(0,0), f_m.at<double>(0,1), f_m.at<double>(0,2),
        f_m.at<double>(1,0), f_m.at<double>(1,1), f_m.at<double>(1,2),
        f_m.at<double>(2,0), f_m.at<double>(2,1), f_m.at<double>(2,2);


    Eigen::Vector3d P1;
    Eigen::Vector3d P2;
    for (int i = 0; i < (int) matches.size(); i++) {
        P1<<points1[matches[i].queryIdx].x, points1[matches[i].queryIdx].y, 1;
        P2<<points2[matches[i].trainIdx].x, points2[matches[i].trainIdx].y, 1;
        score.push_back( P2.transpose()*F*P1 );
    }

    return score;
}


vector<Point2d> distort(vector<Point2d> Points){
    vector<Point2d> local_Points;
    Point2d local_Point;

    Eigen::Matrix<double, 3, 3> K;
    K << 517.306408, 0, 318.643040,
         0, 516.469215, 255.313989,
         0, 0, 1;
    double k1(0.262383),k2(-0.953104),k3(1.163314),p1(-0.005358),p2(0.002628);
    Eigen::Vector3d Point;
    for(int i; i<Points.size();i++){
        Point<<Points[i].x,Points[i].y,1;
        Point = K.inverse()*Point;
        double x=Point[0];
        double y=Point[1];

        double r2=pow(x,2)+pow(y,2);
        double x_distort = x * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
        double y_distort = y * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
        local_Point.x = x_distort + (2 * p1 * x * y + p2 * (r2 + 2 * x * x));
        local_Point.y = y_distort + (p1 * (r2 + 2 * y * y) + 2 * p2 * x * y);
        Point<<local_Point.x,local_Point.y,1;

        Point=K*Point;
        local_Point.x=Point[0];
        local_Point.y=Point[1];
        local_Points.push_back(local_Point);
    }
    return local_Points;
}