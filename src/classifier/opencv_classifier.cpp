//
// Created by oem on 2022/9/11.
//
#include "opencv_classifier.h"
#include <iostream>
using namespace std;

Classifier_lenet5::Classifier_lenet5(/* args */)
{
    net = readNet("../configure/opencv_classifier_param/lenet5_small_best_SGD_100epochs.onnx");
    cout<<"opencv_classifier has imported"<<endl;
}

int Classifier_lenet5::classifier(cv::Mat &img)
{
    // double time1 = static_cast<double>( getTickCount()); //记录起始时间，其中getTickCount()函数返回CPU 自某个事件（如启动电脑）以来走过的时钟周期数
    Mat img_copy = img.clone();
    // imshow("classifier img before",img_copy);
    // waitKey(1);

    Mat img_copy1 = img_copy*1.5+10;
    // imshow("classifier img",img_copy1);
    // waitKey(1);
    cvtColor(img_copy1,img_copy1, COLOR_BGR2GRAY);
    Mat inputBolb = blobFromImage(img_copy,1.0,Size(32,32),Scalar(),false,true,CV_32F);
    inputBolb = inputBolb/255;
    net.setInput(inputBolb);
    Mat result_possibility = net.forward();
    Point max_loc,min_loc;
    double max_val,min_val;
    minMaxLoc(result_possibility,&min_val,&max_val,&min_loc,&max_loc);
    // double time2 = (static_cast<double>( getTickCount()) - time1)/getTickFrequency()*1000;
    //    cout<<"the classifier time:"<<time2<<endl;
    return max_loc.x;
}