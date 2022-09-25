//
// Created by oem on 2022/9/9.
//

#ifndef RM_VISON_CANSERIAL_H
#define RM_VISON_CANSERIAL_H

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
#include "const.h"
#include <iostream>

#define CAN_NAME "can0"
#define STATE_SEND_ID 0x106
#define DATA_SEND_ID 0x108

#define IMU_TIME_RECEIVE_ID 0x100       // 时间ID
#define IMU_RECEIVE_ID 0x0FF            // 四元数ID
#define MODE_RECEIVE_ID 0x110           // 模式ID
#define BS_RECEIVE_ID 0x125             // 弹速等级ID
#define ROBOT_RECEIVE_ID 0x129          // 机器人状态信息ID
#define COMPETITION_RECEVIE_ID 0x12B    // 比赛信息ID

class CanSerial
{

private:
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq interface_request;

public:

    enum CanError {
        SUCCESS,
        DLC_ERROR,
        WRITE_ERROR,
        READ_ERROR
    };
}






#endif //RM_VISON_CANSERIAL_H
