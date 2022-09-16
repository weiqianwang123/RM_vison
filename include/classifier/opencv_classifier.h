//
// Created by oem on 2022/9/11.

#ifndef RM_VISON_OPENCV_CLASSIFIER_H
#define RM_VISON_OPENCV_CLASSIFIER_H

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;

class Classifier_lenet5
{
private:
    Net net;

public:
    Classifier_lenet5(/* args */);

    /**
     * @brief 模型推理，预测数字
     *
     * @param img 由装甲板框选出的图像区域，用于识别装甲板上数字。
     */
    int classifier(Mat &img);
};




#endif //RM_VISON_OPENCV_CLASSIFIER_H
