#ifndef CAN_SERIAL_H
#define CAN_SERIAL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <chrono>
#include <iostream>

#define CAN_NAME "can0"
#define IMU_TIME_RECEIVE_ID 0x100
#define IMU_RECEIVE_ID_0 0x102
#define IMU_RECEIVE_ID_1 0x103
#define MODE_RECEIVE_ID 0x110
#define BS_RECEIVE_ID 0x127
#define STATE_SEND_ID 0x106
#define DATA_SEND_ID 0x108
#define ROBOT_INFO_ID 0x129
using namespace std;

class CanSerial
{

private:
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq interface_request;

public:

enum CanError{
    SUCCESS,
    DLC_ERROR,
    WRITE_ERROR,
    READ_ERROR
};

/**
 *  @brief  自瞄状态与目标发送接口API
 *  @param  state   识别状态
 *  @param  target_id   目标ID
 */
    int send_state(int state, const int target_id,int is_fire);


/**
 *  @brief  自瞄数据发送接口API
 *  @param  pitch_yaw   pitch: 0     yaw: 1
 *  @param  time    所使用的IMU数据的时间
 */
    int send_data(float pitch_predict_camera, float yaw_predict_camera,u_int32_t timestamp);

/**
 *  @brief  IMU数据接收API
 *  @param  imu_data    
 *              0: pitch
 *              1: pitch角速度
 *              2: yaw
 *              3: yaw角速度
 *              4: roll
 *              5: roll角速度
 *  @param  time    IMU时间，单位ms
 */
    int receive_IMU(float* imu_data, u_int32_t &time);

/**
 *  @brief  弹速上限接收API
 *  @param  bullet_speed    弹速上限
 */
    int receive_BS(float &bullet_speed);

/**
 *  @brief  机器人信息接收API
 *  @param  color   己方颜色
 */
    int receive_Robo(int &color);

/**
 *  @brief  自瞄模式切换命令接收API
 *  @param  mode    自瞄模式
 */
    int receive_mode(int &mode);

/**
 *  @brief  接收数据，根据id区分数据包，需要大于1000Hz频率接收
 *  @return error_code
 */
    int can_receive(uint &id, u_char *buf, u_char &dlc);
    
    void transformIMU(u_char *IMU_data, float *imu_data);

/**
 *  @brief  转化接收的IMU 时间数据
 */
    void transformIMU_Time(u_char *IMU_time, u_int32_t &time);
    CanSerial();
    ~CanSerial() = default;

private:
/**
 *  @brief  将角度数据转换为uint8以发送
 */
    void transformData(float* data);
public:
/**
 * @brief   发送数据
 * @param   id  数据对应的ID
 * @param   buf 数据，长度小于等于8
 * @param   dlc 数据长度
 * @return  error_code
*/
    int can_send(uint id, u_char *buf, u_char dlc);


};

#endif