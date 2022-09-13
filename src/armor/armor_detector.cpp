//
// Created by oem on 2022/9/9.
//


#include "armor_detector.h"

/**
 * @brief 设置ROI区域
 * @param   
 */
bool ArmorDetector::setRoi(){ 
    roi_=src.clone();
    return true;
    // if(num>armor_param.num_max||is_findBox==false){
    //     roi=src.clone();
    //     offset = Point2f(0,0);
    //     num = 1;
    //     return true;
    // }
    // else if(target_box.is_empty == false){
    //     // 设置装甲板的roi，宽是3倍的装甲板，高是3倍。宽时长边，高是短边
    //     num++;
    //     int tl_x,tl_y;
    //     double width=target_box.rect.size.width>target_box.rect.size.height?target_box.rect.size.width:target_box.rect.size.height;
    //     double hight=target_box.rect.size.width<target_box.rect.size.height?target_box.rect.size.width:target_box.rect.size.height;
    //     tl_x = target_box.armor_points[1].x - int(width);
    //     tl_y = target_box.armor_points[1].y - int(hight);
    //     tl_x = tl_x>0?tl_x:0;
    //     tl_y = tl_y>0?tl_y:0;
    //     int cols = (tl_x+int(3*width))<armor_param.IMAGE_ROWS?int(3*width):(armor_param.IMAGE_ROWS-tl_x);
    //     int rows = (tl_y +int(3*hight))<armor_param.IMAGE_COLS?int(3*hight):(armor_param_.IMAGE_COLS-tl_y);
    //     roi = src(Rect(tl_x,tl_y,cols,rows));
    //     offset = Point(tl_x,tl_y);
    // }
    //return true;
};

bool ArmorDetector::setRoi(int roi_mul){ 
    if(num_>armor_param_.num_max||is_findBox_==false){
        roi_=src_.clone();
        offset_ = Point2f(0,0);
        num_ = 1;
        return true;
    }
    else if(target_box_.is_empty_ == false){
        // 设置装甲板的roi，宽是3倍的装甲板，高是3倍。宽时长边，高是短边
        num_++;
        int tl_x,tl_y;
        double width=target_box_.rect_.size.width>target_box_.rect_.size.height?target_box_.rect_.size.width:target_box_.rect_.size.height;
        double hight=target_box_.rect_.size.width<target_box_.rect_.size.height?target_box_.rect_.size.width:target_box_.rect_.size.height;
        tl_x = target_box_.armor_points_[1].x - int(width);
        tl_y = target_box_.armor_points_[1].y - int(hight);
        tl_x = tl_x>0?tl_x:0;
        tl_y = tl_y>0?tl_y:0;
        int cols = (tl_x+int(roi_mul*width))<armor_param_.IMAGE_ROWS?int(roi_mul*width):(armor_param_.IMAGE_ROWS-tl_x);
        int rows = (tl_y +int(roi_mul*hight))<armor_param_.IMAGE_COLS?int(roi_mul*hight):(armor_param_.IMAGE_COLS-tl_y);
        roi_ = src_(Rect(tl_x,tl_y,cols,rows));
        offset_ = Point(tl_x,tl_y);
    }
    return true;
};

bool ArmorDetector::setSrc(const Mat &img){
    if(img.empty()) {return false;}
    src_ = img.clone();
    return true;
}

/**
 * @brief   构造检测器
 * @param  param_filename  分类器参数地址
 */
    ArmorDetector::ArmorDetector()
    {
        loadParam(armor_param_);
        enemy_color_ = armor_param_.color;
        int num_ = 1;
        is_findBox_=false;
    };

/**
 * @brief   筛选灯条
 * @param   contours 灯条轮廓
 * @param   rect 灯条的旋转矩形
 */
bool ArmorDetector::filterLightBlob(const cv::RotatedRect &rect){
    double lw_rate= rect_.size.height>rect_.size.width?
                    rect_.size.height/rect_.size.width:
                    rect_.size.width/rect_.size.height;
    if( lw_rate <  armor_param_.light_min_wihe_ratio || lw_rate > armor_param_.light_max_wihe_ratio){
        // cout<<"lw_rate:"<<lw_rate<<endl;
        return false;
    }

    double area_rate = rect_.size.height*rect_.size.width/(armor_param_.IMAGE_COLS*armor_param_.IMAGE_ROWS);
    if(area_rate<armor_param_.light_min_area_ratio||area_rate>armor_param_.light_max_area_ratio){
        // cout<<"false2 light:"<<area_rate<<endl;
        return false;
    };

    
    double angle_ = rect_.size.height>rect_.size.width? abs(rect_.angle):(rect_.angle+90);
    if(angle_>armor_param_.lights_angle_max_dif){
        // cout<<"false3"<<endl;
        return false;
    }
    return true;
}

/**
 * @brief   在图像上寻找灯条
 * @param   light_blobs 得到的灯条集
 */
bool ArmorDetector::findLightBlobs(LightBlobs &light_blobs){
    light_blobs.clear();
    if(roi_.empty())
        return false;

    //灰度化
    cv::Mat gray;
	cv::cvtColor(roi_, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, gray,armor_param_.gray_light_min_threshold, 255, cv::THRESH_BINARY);
    // cv::imshow("grey",gray);waitKey(1);
    // cv::threshold(gray,gray,253,255,cv::THRESH_TOZERO_INV);

    //分离RGB通道
	std::vector<cv::Mat> bgr_channel;
	cv::split(roi_, bgr_channel);
    cv::Mat binary_color_img_;
    if(0==armor_param_.color){
        //红通道减蓝通道相减并二值化
        cv::subtract(bgr_channel[2], bgr_channel[0], binary_color_img_);
        cv::threshold(binary_color_img_, binary_color_img_, armor_param_.red_light_min_threshold, 255, cv::THRESH_BINARY);
        // imshow("e_red_sub",binary_color_img_);waitKey(1);
        //得到通道相减后的二值图与灰度二值图取且,得到具有高亮度的红灯条区域
        binary_Mat_=binary_color_img_ & gray;
        // imshow("e_red_bin",binary_Mat);waitKey(1);
    }
    else{
        //蓝通道减红通道相减并二值化
        cv::subtract(bgr_channel[0], bgr_channel[2], binary_color_img_);
        cv::threshold(binary_color_img_, binary_color_img_,armor_param_.blue_light_min_threshold, 255, cv::THRESH_BINARY);
        //将通道相减后的二值化图与膨胀后的灰度二值图取且，得到具有高亮度的红灯条区域
        // imshow("e_bule_sub",binary_color_img_);waitKey(1);
        //得到通道相减后的二值图与灰度二值图取且,得到具有高亮度的红灯条区域
        binary_Mat_=binary_color_img_ & gray;
        // imshow("e_bule_bin",binary_Mat);waitKey(1);
        // binary_Mat = binary_color_img_& gray; //;
    }
    cv::Mat elem_COLSE = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    // cv::Mat elem_OPEN=cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
	//进行形态学闭运算操作  
	morphologyEx(binary_Mat_, binary_Mat_, MORPH_CLOSE, elem_COLSE);
    // morphologyEx(binary_Mat, binary_Mat, MORPH_OPEN, elem_OPEN);
    if(binary_Mat_.empty()){
         return false;
     }
    //在通道相减后的二值图寻找轮廓

    std::vector<std::vector<cv::Point>> contours_light;
	cv::findContours(binary_Mat_, contours_light, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE,offset_);
    // cout<<"contours_light size:"<<contours_light.size()<<endl;
    //拟合灯条，并筛除不合格灯条
    for(auto contour:contours_light){
        if(contour.size()<armor_param_.minCounterSize ){
            continue;
        }
        cv::RotatedRect light=cv::minAreaRect(contour);
        if(filterLightBlob(light))
        {
            double area_ratio=cv::contourArea(contour)/light.size.area();
            light_blobs.emplace_back(
                light, area_ratio
            );
        }
        
        // 筛除灯条和倒影。需要拟合两个灯条

    }//for contours
    // cout<<"light bolbs :"<<light_blobs.size()<<endl;

    // 对灯条排序
    sort(light_blobs.begin(),light_blobs.end(),[](const LightBlob &a,const LightBlob &b){
       return a.rect_.center.x<b.rect_.center.x;
        
    }); 
    
    // 筛除灯条和倒影。需要拟合两个灯条
    if(light_blobs.size()<2){
        return false;
    }
    // else{
    //     return true;
    // }
    for(int i=0;i<light_blobs.size()-1;i++){
        bool result1,result2;
        // cout<< "centers:"<<i<<","<<light_blobs[i].rect.center<<","<<light_blobs[i+1].rect.center<<","<<light_blobs.size()<<endl;
        filterLightBlob_withReflection(light_blobs[i],light_blobs[i+1],result1,result2);
        // cout<<"result:"<<result1<<","<<result2<<endl;
        if(result2==false){
            light_blobs.erase(light_blobs.begin()+i+1);
            i--;
        }
        else if(result1==false){
            light_blobs.erase(light_blobs.begin()+i);
        }
 
    }

    return light_blobs.size()>=2;
}

void ArmorDetector::filterLightBlob_withReflection(const LightBlob &Lb1,const LightBlob &Lb2,
                                                    bool &result1,bool &result2){
    result2 = true;
    result1 = true; //假设两个均不是倒影
    // if(Lb1.angle/abs(Lb1.angle)!=Lb2.angle/abs(Lb2.angle)){
    //     return;
    // }
    if((abs(Lb1.rect_.center.y - Lb2.rect_.center.y)/abs(Lb1.rect_.center.x - Lb2.rect_.center.x)>armor_param_.lights_xBias_yBias_ratio_min &&
    abs(Lb1.rect_.center.y - Lb2.rect_.center.y)/abs(Lb1.length_)<10)){
        if(Lb1.rect_.center.y <Lb2.rect_.center.y){
            result2 = false;
        }
        else{
            result1 = false;
        }
    } 
    return;
}

/**
 * @brief  检验两灯条是否满足构造成装甲板的要求
 * @param  light_blob_i  需要匹配的灯条i
 * @param  light_blob_j  需要匹配的灯条j
 */
bool  ArmorDetector::isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j, double & Dist_height_rate,int &id)
{
    //判断两个灯条长度比
    if ((light_blob_i.length_ / light_blob_j.length_ > armor_param_.lights_lenght_max_ratio ||
         light_blob_i.length_ / light_blob_j.length_ < armor_param_.lights_lenght_min_ratio))
    {
        // cout<<"lihght length rate:"<<armor_param_.lights_lenght_max_ratio<<","<<light_blob_i.length / light_blob_j.length<<endl;
        return false;
    }

    //判断两个灯条的中心距及高度差
    double side_length, max_length;
    cv::Point2f sub_centers = light_blob_j.rect_.center - light_blob_i.rect_.center;
    // cout<<light_blob_j.rect.center<<"--"<<light_blob_i.rect.center<<endl;
    side_length = sqrt(sub_centers.ddot(sub_centers));  //中心距

    max_length = fmax(light_blob_i.length_, light_blob_j.length_); //最大长度
    double min_length = fmin(light_blob_i.length_, light_blob_j.length_);
    Dist_height_rate = side_length / max_length;
    if (Dist_height_rate > armor_param_.lights_center_max_ratio ||
        Dist_height_rate < armor_param_.lights_center_min_ratio) 
        //中心的高度差和长度的比值
    {
        // cout<< "centers:"<<light_blob_j.rect.center<<" - "<<light_blob_i.rect.center<<endl;;
        // cout<<"false1 data:"<<Dist_height_rate<<endl;
        // cout<<abs(sub_centers.x)<<"--"<<abs(sub_centers.y)<<"--"<<max_length<<endl;
        // cout<<1<<endl;
        
        return false;
    }
    // cout<<"11:"<<abs(sub_centers.y)/min_length<<endl;
    // 灯条的角度必须往一边偏，否则就不是同一组灯条。
    double angle = abs(light_blob_i.angle - light_blob_j.angle);
    if (angle>armor_param_.cp_lights_angle_diff)
    {
        //cout<<"armor_param.cp_lights_angle_diff:"<<armor_param.cp_lights_angle_diff<<endl;
        return false;
    }
    
    double angle_mean = abs(light_blob_i.angle + light_blob_j.angle)/2;
    // cout<<"angle mean111:"<<angle_mean<<endl;
    double delt_y = (cos(angle_mean/180.0*3.1415926)*min_length);
    double suby_len_rate = abs(sub_centers.y)/delt_y;
    // cout<<"delt_y:"<<delt_y<<","<<abs(sub_centers.y)<<endl;
    
    // cout<<"sub len rate:"<<suby_len_rate<<endl;
    if(angle_mean<6){
        if(armor_param_.lights_height_max_dif_4 < suby_len_rate ){
            // cout<<"suby6"<<suby_len_rate<<endl;
            return false;
        }
    }

    else if(angle_mean<12){
        // cout<<armor_param.lights_height_max_dif_10<<endl;
        if(armor_param_.lights_height_max_dif_10 < suby_len_rate ){
            // cout<<"suby12"<<suby_len_rate<<endl;
            
            return false;
        }
    }
    else{
        if(armor_param_.lights_height_max_dif_15 < suby_len_rate ){
            // cout<<"subyelse"<<suby_len_rate<<endl;
            
            return false;
        }
    }

    // 利用分类器，进行装甲板判断
    cv::Point2f centers = (light_blob_j.rect_.center + light_blob_i.rect_.center)/2.0;
    int x_roiClassify = (centers.x-1*side_length)>0?int(centers.x-1*side_length):0;
    int y_roiClassify = (centers.y-1.75*max_length)>0?int(centers.y-1.75*max_length):0;
    int width_roiClassify = x_roiClassify+2*side_length>armor_param_.IMAGE_ROWS?int(armor_param_.IMAGE_ROWS-x_roiClassify):int(2*side_length);
    int heigth_roiClassify = y_roiClassify+3.5*max_length>armor_param_.IMAGE_COLS?int(armor_param_.IMAGE_COLS-y_roiClassify):int(3.5*max_length);
    Mat roi_classifier = src_(Rect(x_roiClassify,y_roiClassify,width_roiClassify,heigth_roiClassify));
    // imshow("the classifier pic",roi_classifier);waitKey(1);
    
    //!!!!!
    //id_ = classifier.inference(roi_classifier);
    // if(id==8||id==9||id==10){
    //     return false;

    // }
    // else{
    //     return true;
    // }
    return true;
}

void ArmorDetector::calculateScore(ArmorBox & armor){
    Point2f img_center = Point2f(armor_param_.IMAGE_ROWS/2.0,armor_param_.IMAGE_COLS/2.0);
    Point2f sub_center = armor.rect_.center - img_center;
    double area_weigth=0.5,center_weigth=0.2,id_weigth=0;
    // cout<<"distance of centers:"<<sqrt(sub_center.ddot(sub_center))<<endl;
    double lights_height_diff = abs(abs(armor.armor_points_[0].y-armor.armor_points_[2].y) - abs(armor.armor_points_[1].y-armor.armor_points_[3].y)) / abs(armor.armor_points_[1].y-armor.armor_points_[3].y);
    double area_rate = sqrt( armor.rect_.size.width*armor.rect_.size.height);
    double center_distance = sqrt(sub_center.ddot(sub_center));
    double id_s = 1;
    double lights_len_sum = armor.light_Blobs_[0].length_ + armor.light_Blobs_[1].length_;
    // if(armor.id!=8 && armor.id!=9&& armor.id!=10){
    //     id_s = 0;
    // }
    armor.score = area_rate * 10 + lights_len_sum
                - id_s*10
                - center_distance*5
                - lights_height_diff * 2 ;
}

                // 
/**
 * @brief  匹配所有灯条，得出伪装甲板集
 * @param  light_blobs 筛选后得到的灯条集
 * @param  armor_boxes 得到的装甲板集
 */
bool ArmorDetector::matchArmorBoxes(LightBlobs &light_blobs, ArmorBoxes &armor_boxes)
{
    armor_boxes.clear();
    if(light_blobs.empty()) return false;
    
    double Dist_height_rate;
    if(light_blobs.size()==2){
        //判断灯条是否匹配
            int id;
            if (isCoupleLight(light_blobs.at(0), light_blobs.at(1),Dist_height_rate,id)){
            armor_boxes.emplace_back(light_blobs[0],light_blobs[1],(Dist_height_rate>armor_param_.lights_wihe_rate_boxSzie),id);
            }
            
    }


    else{
        for(int i=0; i < light_blobs.size() - 1; i++){
            for(int j = i+1; j < i+2; j++){
                // cout<<"i j:"<<i<<","<<j<<endl;
                int id;
            if (!isCoupleLight(light_blobs.at(i), light_blobs.at(j),Dist_height_rate,id)){
                continue;
            }
            int left = min(i,j), right = max(i,j);
            armor_boxes.emplace_back(light_blobs[left],light_blobs[right],(Dist_height_rate>armor_param_.lights_wihe_rate_boxSzie),id);
            }
        }
    }
    return !armor_boxes.empty();
}


/**
 * @brief  得出目标装甲板集
 * @param  boxes 得到的目标装甲板集
 */
bool ArmorDetector::findArmorBoxes(){
    this->armor_boxes_.clear();
    this->light_blobs_.clear();
    if(!findLightBlobs(this->light_blobs_)){
        is_findBox_ = false;
        return false;
    }  
    if(!matchArmorBoxes(this->light_blobs_,this->armor_boxes_)){
        is_findBox_ = false;
        return false;
        }
    is_findBox_ = true;
    return true;   
}

bool ArmorDetector::setTargetBox(){
    if(is_findBox_){
        num_noBox_ =0;
        target_box_.is_empty_=false;
        target_box_ = armor_boxes_[0];
        calculateScore(target_box_);
        // double score=0,score_temp=calculateScore(_armor_boxes[0]);
        if(armor_boxes_.size()>1){
            for(int i=1;i<armor_boxes_.size();i++){
            calculateScore(armor_boxes_[i]);
            if(armor_boxes_[i].score>target_box_.score){
                target_box_ = armor_boxes_[i];
            }
        }
        }   
    }
    else{
        num_noBox_++;
        if(num_noBox_>armor_param_.num_noBox_max){
            target_box_.is_empty_=true;
        }
    }
    return true;
}
