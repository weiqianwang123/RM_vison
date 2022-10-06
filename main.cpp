#include<iostream>
#include"armor_box.h"
#include"armor_detector.h"
#include"mercure_driver.h"
//#include"debug.h"
#include"opencv_classifier.h"
#include"solver.h"
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace camera;

int main(){

    // create a image as source image
    Mat src_img;
    //src_img.create(800,1200,CV_8UC3);
    src_img.create(720,1280,CV_8UC3);
    cout<<"2"<<endl;

    //initial the camera
    VideoCapture capture(2);
    //camera::MercureDriver capture;
    cout<<"2"<<endl;
    
    // initial the armordetector
    ArmorDetector armor_detector = ArmorDetector();
    bool is_findArmors,is_setroi,is_setSrc,is_settarget;
    ArmorBoxes boxes;

    cout<<"2"<<endl;
    while(1){
        
        
        double time1 = static_cast<double>( getTickCount()); //记录起始时间，其中getTickCount()函数返回CPU 自某个事件（如启动电脑）以来走过的时钟周期数
        capture >> src_img;
        cout<<" "<<src_img.size().height<<endl;
        cout<<" "<<src_img.size().width<<endl;

        is_setSrc = armor_detector.setSrc(src_img);

        is_setroi = armor_detector.setRoi();//未设置roi
        
        is_findArmors = armor_detector.findArmorBoxes();

        is_settarget = armor_detector.setTargetBox();


        cout<<"target_box.is_empty:"<<armor_detector.target_box_.is_empty_<<endl;

        
        double time2 = (static_cast<double>( getTickCount()) - time1)/getTickFrequency()*1000;

        cout<<"the time costed in detecting is "<<time2<<endl;
        //show_armorBoxes(armor_detector,true);

        armor_detector.solvePnp();



        imshow("src_img",src_img);
        waitKey(1);

        
    }

    return 0;
}//#include"debug.h"