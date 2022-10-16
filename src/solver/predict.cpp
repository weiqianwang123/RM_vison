//
// Created by oem on 2022/10/6.
//
#include "predict.h"

void Ballistics::read_ballistics_param()
{
    string fileName = "../configure/predict.xml";
    cv::FileStorage fs(fileName, cv::FileStorage::READ);

    if(!fs.isOpened())
    {
        cout<<"please check your correct param file"<<endl;
        assert(fs.isOpened());
        return;
    }
    fs["bullte_speed"] >> ballisticsParam.bullte_speed;
    fs["communicate_delay"] >> ballisticsParam.communication_delay;
    fs["fire_delay"]>>ballisticsParam.fire_delay;
    fs["m"]>>ballisticsParam.m;//质量
    fs["k"]>>ballisticsParam.k; //空气阻力系数，我使用一介空气阻力模型；
    fs["speed_attenuation_coefficient"]>>ballisticsParam.speed_attenuation_coefficient;

}

Ballistics::Ballistics(double distance_camera, float pitch_camera, float yaw_camera, float *q, cv::Point3f point_camera,u_int32_t timestamp)
{
    read_ballistics_param();

    this->distance_camera = distance_camera;
    this->pitch_camera = pitch_camera;
    this->yaw_camera = yaw_camera;

    this->pose_now.q0 = q[0];
    this->pose_now.q1 = q[1];
    this->pose_now.q2 = q[2];
    this->pose_now.q3 = q[3];
    this->pose_now.timestamp = timestamp;
    Eigen::Vector3d temp(point_camera.x,point_camera.y,point_camera.z);
    this->point_camera = temp;

    distance_camera2realworld();
    point_camera2IMU();
    point_IMU2realworld();
    pitchyaw_camera2realworld();
    caculate_best_ballistics();
    pitchyaw_realworld2camera();


}

void Ballistics::distance_camera2realworld()
{
    this->distance_realworld = this->distance_camera;
}

void Ballistics::point_camera2IMU()
{
    point_IMU.x()=  point_camera.z();
    point_IMU.y() = -point_camera.y();
    point_IMU.z() = -point_camera.x();
}

void Ballistics::point_IMU2realworld()
{
    Eigen::Quaterniond q = Eigen::Quaterniond(pose_now.q0,pose_now.q1,pose_now.q2,pose_now.q3).conjugate();
    point_IMU = point_IMU*0.001;
    point_realworld = q*point_IMU;
}

void Ballistics::pitchyaw_camera2realworld()
{
    pitch_realworld = atan2(-point_realworld.z(), sqrt(point_realworld.x()*point_realworld.x()+point_realworld.y()*point_realworld.y()))/CV_PI*180;
    yaw_realworld = atan2(point_realworld.y(),point_realworld.x())/CV_PI*180;
}


void Ballistics::caculate_best_ballistics()
{
    double m_div_k  = ballisticsParam.m/ballisticsParam.k;
    double g=ballisticsParam.g;
    double x=point_realworld.x();
    double y=point_realworld.y();
    double z=point_realworld.z();
    double v_x=0;
    double v_y=0;
    double v_z=0;
    double t = sqrt(x*x+y*y)/ballisticsParam.bullte_speed;
    ceres::Problem problem;//有问题
    problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<CostFunction,1,1>(new CostFunction(v_x,v_y,v_z,ballisticsParam.bullte_speed,point_realworld.y(),g,m_div_k,point_realworld.x(),point_realworld.z())),
            nullptr,
            &t
    );
    ceres::Solver::Options options;
    ceres::Solver::Summary summary;

    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    options.logging_type = ceres::LoggingType::SILENT;
    ceres::Solve(options, &problem, &summary);  // 开始优化，求解 //有问题

    point_predict_realworld.x() =  (x)+(t)*(v_x);
    point_predict_realworld.y() =  y + t*v_y;
    point_predict_realworld.z() = z + t*v_z;
    pitch_predict_realworld = - asin( ((((z+v_z*t) + m_div_k*g*t)/(m_div_k)/(1.0-exp(-1.0/m_div_k*t)))- m_div_k*g)/(ballisticsParam.bullte_speed*ballisticsParam.speed_attenuation_coefficient))*180/CV_PI;
    yaw_predict_realworld = atan2(point_predict_realworld.y(),point_predict_realworld.x())/CV_PI*180;
    this->time_bullet_fly = (int)(t * 1000);

}


void Ballistics::pitchyaw_realworld2camera()
{
    Eigen::Quaterniond q = Eigen::Quaterniond(pose_now.q0,pose_now.q1,pose_now.q2,pose_now.q3);
    point_predict_IMU = q*point_predict_realworld;
    pitch_predict_camera = pitch_predict_realworld-pitch_realworld+pitch_camera;
    yaw_predict_camera = -(yaw_predict_realworld-yaw_realworld)+yaw_camera;


}