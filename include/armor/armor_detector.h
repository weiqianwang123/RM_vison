//
// Created by oem on 2022/9/9.
//

#ifndef ARMORDETECTOR_H_
#define ARMORDETECTOR_H_

#include <string>
#include "armor_box.h"
#include "opencv_classifier.h"
using namespace cv;

class ArmorDetector
{
private:
    //!  分类器
    Classifier_lenet5 classifier;

    // roi区域追踪帧数；
    int num_=1;
    int num_noBox_ = 0; //连续多少帧找不到目标装甲板
    
    
public:
    //! 敌方颜色,0为红色，1为蓝色
    int enemy_color_;
    // 是否找到装甲板
    bool is_findBox_;
    //!  装甲板限制参数
    ArmorParam armor_param_;
    //! 目标装甲板
    ArmorBox target_box_;
    //! 目标装甲板集
    ArmorBoxes armor_boxes_;
    //! 目标灯条集
    LightBlobs light_blobs_;
    //! 图像ROI区域
    cv::Mat  roi_;
    Mat binary_Mat_; // 处理后的图
    //！ ROI区域的偏移量
    cv::Point2f offset_;
    // 原图像
    Mat src_;

    u_char shoot_code_=0x0; //0是没找到 1是找到了
public:

/**
 * @brief 设置ROI区域
 * @param  src 相机得到的图片
 * @param  tracking_rect 需要取的roi区域，默认为空时取原始图像区域
 */
    bool setRoi();
    bool setRoi(int roi_mul);
    bool setSrc(const Mat &img);
private:

/**
 * @brief   在图像上寻找灯条
 * @param   light_blobs 得到的灯条集
 */
    bool findLightBlobs(LightBlobs &light_blobs);

/**
 * @brief 筛选灯条，有倒影版本
 * 
 * @param rect1 前一个灯条
 * @param rect2 后一个灯条，两个相邻，用的是x坐标
 * @param result1 第一根灯条是否删去。
 * @return true 
 * @return false 
 */
void filterLightBlob_withReflection(const LightBlob &Lb1,const LightBlob &Lb2,
                                                    bool &result1,bool &result2);
public:

/**
 * @brief  匹配所有灯条，得出伪装甲板集
 * @param  light_blobs 筛选后得到的灯条集
 * @param  armor_boxes 得到的装甲板集
 */
    bool matchArmorBoxes(LightBlobs &light_blobs, ArmorBoxes &armor_boxes);

/**
 * @brief  检验两灯条是否满足构造成装甲板的要求
 * @param  light_blob_i  需要匹配的灯条i
 * @param  light_blob_j  需要匹配的灯条j
 */
    bool isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j, double &centerDist_height_rate,int &id);

/**
 * @brief 设置目标装甲板
 * 
 * @return true 
 * @return false 
 */

    bool setTargetBox();

/**
 * @brief 计算装甲板得分
 * 
 * @param armor 
 * @return true 
 * @return false 
 */
    void calculateScore(ArmorBox & armor);
public:

/**
 * @brief  得出目标装甲板集
 * @param  boxes 得到的目标装甲板集
 */
    bool findArmorBoxes(); 

bool filterLightBlob(const cv::RotatedRect &rect);

/**
 * @brief   构造检测器
 * @param  color 敌方颜色
 * @param  param_filename  分类器参数地址
 * @param  armor_param   装甲板的限制参数
 * @return NONE
 */
    ArmorDetector();
    // ArmorDetector(const LightBlob & l_blob,const LightBlob & r_blob);
    ~ArmorDetector() = default;
};
#endif
