//
// Created by oem on 2022/9/9.
//

#ifndef ARMORBOX_H_
#define ARMORBOX_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include<vector>

const int IMAGE_COLS=1200;
const int IMAGE_ROWS=800;
const int IMAGE_CENTER_X=IMAGE_COLS/2.0;
const int IMAGE_CENTER_Y=IMAGE_ROWS/2.0;

const int ENEMY_RED=0;
const int ENEMY_BLUE=1;

using namespace cv;
using namespace std;

//! 装甲板限制参数
struct ArmorParam
{
    float lights_angle_max_dif;
    float lights_height_max_dif_4;
    float lights_height_max_dif_10;
    float lights_height_max_dif_15;
    float lights_center_min_ratio;
    float lights_center_max_ratio;
    float lights_lenght_min_ratio;
    float lights_lenght_max_ratio;
    float same_lights_max_dis;
    float light_min_wihe_ratio;
    float light_max_wihe_ratio;
    float light_min_area_ratio;
    float light_max_area_ratio;
    float light_bigger_area_ratio;
    float blue_light_min_threshold;
    float red_light_min_threshold;
    float gray_light_min_threshold;
    int color;
    int IMAGE_COLS;
    int IMAGE_ROWS;  
    int kerSize;
    int minCounterSize;
    int num_max;
    double cp_lights_angle_diff;
    float lights_wihe_rate_boxSzie;
    float lights_xBias_yBias_ratio_min;
    double toping_time_intervals;
    double angle_mean_max;
    int num_noBox_max;
};

void loadParam(struct ArmorParam & param);

//! 相对相机的距离及装甲板大小类型
enum DistanceSize
{
    BIG_NEAR = 0,   
    BIG_FAR = 1,
    SMALL_NEAR = 2,
    SMALL_FAR = 3,
};


/********************* 灯条类定义　************************/
class LightBlob
{
public:
    //! 包含灯条位置（旋转）
    cv::RotatedRect rect_;
    //! 轮廓与外包矩形面积比
    double area_ratio_;
    //! 长轴长度
    double length_;
    // 灯条角度
    double angle_;



public:
    /**
     * @brief 构造函数
     * @param r 旋转矩形
     * @param ratio  轮廓与外包矩形面积比
    */
    LightBlob(cv::RotatedRect &r, double ratio) : rect_(r), area_ratio_(ratio)
    {
        length_ = std::max(rect_.size.height, rect_.size.width);
        setAngle();
    }

    LightBlob() = default;

    /**
    * @brief  析构函数
    */
    ~LightBlob(){}
    /**
     * @brief Set the Angle of blob。如果灯条往左偏，则它是负的角；往右是正的角。
     * 
     * @return true 设置完成，返回真
     * @return false 
     */
    bool setAngle();
};
typedef std::vector<LightBlob> LightBlobs;

/********************* 装甲板类定义　************************/

    enum ArmorType{
        SMALL = 0,
        BIG = 1,
        RUNE_ARMOR = 2
    };//缝合

class ArmorBox
{
public:
    
    cv::RotatedRect rect_;//! 装甲板旋转矩形
    LightBlobs light_Blobs_;//! 装甲板的一对灯条
    int id_=-1;//！ 装甲板id;1号英雄，2号工程，3、4、5步兵，6无人机，7哨兵
    bool is_empty_=true; // 是否是空的装甲板
    int boxSize_=0;// 装甲板类型，0为小，1为大；

    ArmorType armortype_ = SMALL;
    // 灯条的四个顶点   
    std::vector<Point2f> armor_points_;// bl,tl,tr,br;
    DistanceSize distance_size_ ;
    // 装甲板得分。
    double score;
    /**
     * @brief 构造函数
     * @param pos 拟合装甲板的矩形
     * @param blobs 装甲板的一对灯条
     * @param box_color 灯条颜色
     * @param box_id  装甲板id 
     * @param dis_size 相对相机的距离及装甲板大小
     */
    explicit ArmorBox(const cv::RotatedRect &pos = cv::RotatedRect(), const LightBlobs &blobs = LightBlobs(), int box_color = 0, int box_id = 0, DistanceSize dis_size=SMALL_NEAR);
    ArmorBox(const LightBlob &l_blob, const LightBlob &r_blob,bool Dist_height_rate,int pred_id);
    
    /**
     * @brief  根据距离与屏幕中心的距离及id,对装甲板优先级比较
    */
    bool operator<(const ArmorBox &box) const;
    

    /**
     * @brief 设置装甲板的四个顶点
     * @param l_light the left light
     * @param r_light the right light
     */
    void set_points(const cv::RotatedRect &l_light, const cv::RotatedRect &r_light);
};
typedef std::vector<ArmorBox> ArmorBoxes;



#endif
