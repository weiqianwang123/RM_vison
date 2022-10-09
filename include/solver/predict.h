//
// Created by oem on 2022/10/6.
//

#ifndef RM_VISON_PREDICT_H
#define RM_VISON_PREDICT_H


#include<opencv2/opencv.hpp>
#include<ceres/ceres.h>
#include<Eigen/Core>
#include<Eigen/Geometry>
#include <iomanip>

using namespace ceres;
using namespace std;
using namespace cv;

struct Pose
{
    float q0 = 0;
    float q1 = 0;
    float q2 = 0;
    float q3 = 0;
    u_int32_t timestamp = 0;
};


struct ballistics_param
{
    double bullte_speed;//子弹速度
    double communication_delay;//通信延迟
    double fire_delay;//发射延迟
    double speed_attenuation_coefficient;

    double m;//质量
    double k;//f=kV,近似阻力系数
    double g;//重力系数

};

struct predict_param
{


    Mat A;//运动方程前一运动状态前的系数
    Mat P;//状态值协方差矩阵
    Mat Q;//状态矩阵的噪声噪声
    Mat R;//测量噪声协方差矩阵
    Mat H;//状态转移矩阵
};


/*弹道类用于
 * 1，解算弹道；
 * 2，相机坐标系，枪管坐标系，陀螺仪坐标系，世界坐标系之间的相互转化；
 * 3，打静止物体;
*/
class  Ballistics
{
public:
    ballistics_param ballisticsParam{}; //初始化弹道参数

    int time_bullet_fly = 0;

    double distance_camera;  //由pnp解算得出的在相机坐标系下距离
    double distance_realworld;  //世界坐标系下的距离

    float pitch_camera,yaw_camera;  //由pnp解算得出的在相机坐标系下的pitch和yaw
    float pitch_realworld,yaw_realworld;  //世界坐标系下的pitch和yaw
    float pitch_predict_realworld,yaw_predict_realworld;
    float pitch_predict_camera,yaw_predict_camera;

    Eigen::Vector3d point_camera;
    Eigen::Vector3d point_IMU;
    Eigen::Vector3d point_realworld;
    Eigen::Vector3d point_predict_realworld;
    Eigen::Vector3d point_predict_IMU;

    Pose pose_now;//由电控发过来的现在时刻云台姿态

public:
    void read_ballistics_param();

    Ballistics(double distance_camera,float pitch_camera,float yaw_camera,float* q,cv::Point3f point_camera,u_int32_t timestamp);
    ~Ballistics() = default;



    void distance_camera2realworld();
    void point_camera2IMU();
    void point_IMU2realworld();
    void pitchyaw_camera2realworld();
    void caculate_best_ballistics();
    void pitchyaw_realworld2camera();






/**
 * @brief 残差计算结构体
 *
 */
    struct CostFunction { //ceres 求解模板
        CostFunction(double v_x, double v_y,double v_z, double speed, double y, double g,double m_div_k,double x,double z):
                v_x(v_x), v_y(v_y),v_z(v_z),speed(speed),y(y),g(g),m_div_k(m_div_k),x(x),z(z){};

        template <typename T>
        bool operator()(const T* const t, T* residual) const {
            residual[0] =  (sqrt((x+(t[0])*v_x)*(x+(t[0])*v_x) + (y+(t[0])*v_y)*(y+(t[0])*v_y))
                            - m_div_k * speed * (1.0-exp(-1.0/m_div_k*t[0]))
                              * cos( asin( ((((z+v_z*t[0]) + m_div_k*g*t[0])/(m_div_k)/(1.0-exp(-1.0/m_div_k*t[0])))- m_div_k*g)/speed)));
            // * cos( asin( ((((z) + m_div_k*g*t[0])/(m_div_k)/(1.0-exp(-1.0/m_div_k*t[0])))- m_div_k*g)/speed)));
            return true;
        }
        double v_x, v_y,v_z,speed,y,g,m_div_k,x,z;
    };
};

#endif //RM_VISON_PREDICT_H
