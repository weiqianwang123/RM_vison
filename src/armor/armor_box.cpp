//
// Created by oem on 2022/9/9.
//

#include "armor_box.h"
using namespace cv;
using namespace std;
/**
 * @brief 构造函数
 * @param pos 拟合装甲板的矩形
 * @param blobs 装甲板的一对灯条
 * @param box_color 灯条颜色
 * @param box_id  装甲板id 
 * @param dis_size 相对相机的距离及装甲板大小
 */
ArmorBox::ArmorBox(const cv::RotatedRect &pos, const LightBlobs &blobs, int  box_color, int box_id, DistanceSize dis_size) :
        rect_(pos), light_Blobs_(blobs),id(box_id),distance_size_(dis_size){
    //
    };

ArmorBox::ArmorBox(const LightBlob &l_blob, const LightBlob &r_blob,bool Dist_height_rate,int pred_id){
    is_empty = false;
    set_points(l_blob.rect_,r_blob.rect_);
    RotatedRect rect = minAreaRect(armor_points_);
    this->rect_ = rect;
    this->light_Blobs_ = {l_blob,r_blob};
    if(Dist_height_rate)
    {
        boxSize_ = 1;
        armortype_ = SMALL;//缝合
    }
    else
    {
        boxSize_ = 0;
        armortype_ = BIG;//缝合
    }
    id_ = pred_id;
    };

/**
* @brief  对<进行重载，根据距离与屏幕中心的距离及id,对装甲板优先级比较
*/
bool ArmorBox::operator<(const ArmorBox &box)const{
    if(1){
        //加入id比较
    }
    auto d1 = (rect_.center.x - IMAGE_CENTER_X) * (rect_.center.x - IMAGE_CENTER_X)
                  + (rect_.center.y - IMAGE_CENTER_Y) * (rect_.center.y - IMAGE_CENTER_Y);
    auto d2 = (box.rect_.center.x - IMAGE_CENTER_X) * (box.rect_.center.x - IMAGE_CENTER_X)
                  + (box.rect_.center.y - IMAGE_CENTER_Y) * (box.rect_.center.y - IMAGE_CENTER_Y);
    return d1 < d2;
}

void ArmorBox::set_points(const cv::RotatedRect &l_light, const cv::RotatedRect &r_light){
    armor_points_.resize(4);
    Point2f l_rect[4],r_rect[4];
    l_light.points(l_rect);
    r_light.points(r_rect);

    if(l_light.size.width>l_light.size.height){
        armor_points_[0] = (l_rect[1] + l_rect[0])*0.5;
        armor_points_[1] = (l_rect[2] + l_rect[3])*0.5;
    }
    else{
        armor_points_[0] = (l_rect[0] + l_rect[3])*0.5;
        armor_points_[1] = (l_rect[1] + l_rect[2])*0.5;
    }
    if(r_light.size.width>r_light.size.height){
        armor_points_[3] = (r_rect[0]+r_rect[1])*0.5;
        armor_points_[2] = (r_rect[3] + r_rect[2])*0.5;
    }
    else{
        armor_points_[3] = (r_rect[3]+r_rect[0])*0.5;
        armor_points_[2] = (r_rect[2]+r_rect[1])*0.5;
    }
    // cout<<"set points:"<<armor_points<<endl;
}

/**
 * @brief 设置一些装甲板识别的参数
 * 
 * @param param 
 */
void loadParam(struct ArmorParam & param){
    //string  filename="../params/ArmorBox.xml";
    string  filename="./congigure/armorbox.xml";
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if(!fs.isOpened()){
        cout<<"please check your armor classifier_param file"<<endl;
        assert(fs.isOpened());
        return;
        }
    fs["lights_angle_max_dif"] >> param.lights_angle_max_dif;
    fs["lights_height_max_dif_4"] >> param.lights_height_max_dif_4;
    fs["lights_height_max_dif_10"] >> param.lights_height_max_dif_10;
    fs["lights_height_max_dif_15"] >> param.lights_height_max_dif_15;
    fs["lights_center_min_ratio"]>>param.lights_center_min_ratio;
    fs["lights_center_max_ratio"]>>param.lights_center_max_ratio;
    fs["lights_lenght_min_ratio"]>>param.lights_lenght_min_ratio;
    fs["lights_lenght_max_ratio"]>>param.lights_lenght_max_ratio;
    fs["light_min_wihe_ratio"]>>param.light_min_wihe_ratio;
    fs["light_max_wihe_ratio"]>>param.light_max_wihe_ratio;
    fs["light_min_area_ratio"]>>param.light_min_area_ratio;
    fs["light_max_area_ratio"]>>param.light_max_area_ratio;
    fs["light_bigger_area_ratio"]>>param.light_bigger_area_ratio;
    fs["blue_light_min_threshold"]>>param.blue_light_min_threshold;
    fs["red_light_min_threshold"]>>param.red_light_min_threshold;
    fs["gray_light_min_threshold"]>>param.gray_light_min_threshold;
    fs["color"]>>param.color;
    fs["IMAGE_COLS"]>>param.IMAGE_COLS;
    fs["IMAGE_ROWS"]>>param.IMAGE_ROWS;
    fs["kerSize"]>>param.kerSize;
    fs["minCounterSize"]>>param.minCounterSize;
    fs["num_max"]>>param.num_max;
    fs["cp_lights_angle_diff"]>>param.cp_lights_angle_diff;
    fs["lights_wihe_rate_boxSzie"]>>param.lights_wihe_rate_boxSzie;
    fs["lights_xBias_yBias_ratio_min"]>>param.lights_xBias_yBias_ratio_min;
    fs["toping_time_intervals"]>>param.toping_time_intervals;
    fs["angle_mean_max"]>>param.angle_mean_max;
    fs["num_noBox_max"]>>param.num_noBox_max;
    cout<<"have loaded the armor box params"<<endl;
}

/**
 * @brief 通过灯条的旋转矩形，获取灯条的角度
 * 
 * @return true 
 * @return false 
 */

bool LightBlob::setAngle()
{
    angle_ = rect_.size.height>rect_.size.width? -1*abs(rect_.angle):(rect_.angle+90);
    return true;
}