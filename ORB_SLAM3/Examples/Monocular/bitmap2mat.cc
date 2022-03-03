
#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<iomanip>
#include <unistd.h>

#include<opencv2/core/core.hpp>

#include"System.h"
#include "Converter.h"

#include <FreeImage.h>

#include <algorithm>
#include <cmath>
#include <ios>
#include <limits>
#include <memory>
#include <string>
#include <vector>
using namespace std;
using namespace cv;
typedef std::unique_ptr<FIBITMAP, void (*)(FIBITMAP *)> FIBitmapPtr;

void FI2MAT(FIBITMAP* fi_bitmap, Mat& mat);
void MAT2FI(FIBITMAP* fi_bitmap, Mat& mat);

int main()
{
    string path="/home/changfei_fu/dataset/rgbd_dataset_freiburg1_xyz/rgb/1305031102.175304.png";
    const FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
    FIBITMAP* fi_bitmap = FreeImage_Load(format, path.c_str(), JPEG_EXIFROTATE);
    Mat mat;
    FI2MAT(fi_bitmap, mat);

    imshow("",mat);
    waitKey(0);

    Mat mat1=imread("/home/changfei_fu/dataset/rgbd_dataset_freiburg1_xyz/rgb/1305031105.143106.png");
    MAT2FI(fi_bitmap, mat1);
    Mat mat2;
    FI2MAT(fi_bitmap, mat2);
    imshow("wwwww",mat2);
    waitKey(0);
    return 0;
}

void FI2MAT(FIBITMAP* fi_bitmap, Mat& mat)
{
    int width = FreeImage_GetWidth(fi_bitmap);
    int height = FreeImage_GetHeight(fi_bitmap);

    mat = Mat(height, width, CV_8UC3, FreeImage_GetBits(fi_bitmap));
}

void MAT2FI(FIBITMAP* fi_bitmap, Mat& mat)
{
    Mat wrraper=Mat(mat.rows, mat.cols, CV_8UC3, FreeImage_GetBits(fi_bitmap));
    mat.copyTo(wrraper);
}
