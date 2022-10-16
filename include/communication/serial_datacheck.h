// //
// // Created by oem on 2022/10/6.
// //

// #ifndef RM_VISON_SERIAL_DATACHECK_H
// #define RM_VISON_SERIAL_DATACHECK_H
// #include <iostream>
// #include <fstream>
// #include <stdio.h>
// #include <stdint.h>
// #include <chrono>
// #include <thread>
// #include <fcntl.h>
// #include <unistd.h>
// #include <termios.h>
// #include <errno.h>
// #include <string>
// #include <stdexcept>
// #include <exception>
// #include <memory.h>
// #include <base.h>
// #include "serial_communication.h"

// #define PORT "/dev/ttyUSB0"
// #define FD -1
// #define SPEED 460800
// #define DATABITS 8
// #define STOPBITS 1
// #define PARITY 'N'
// #define COMPORT
// #define READ_BYTE 23
// #define SEND_BYTE 8

// const uint8_t  SERIAL_SOF = (uint8_t)0xEF;
// const uint8_t  SERIAL_EOF = (uint8_t)0xFF;


// struct SendData
// {
//     uint8_t SOF;
//     uint8_t Data[5];
//     uint8_t END;
//     // 是否找到装甲板
//     // pitch 高位
//     // pitch 低位
//     // yaw  高位
//     // yaw  低位
// };


// #pragma pack(1)
// typedef struct RevData
// {
//     uint8_t SOF;
//     uint8_t flag;
//     float q[4];
//     uint32_t time_stamp;
//     uint8_t END;
//     //起始字节
//     //标志信息
//     //q0,q1,q2,q3
//     //32位时间戳
//     //结束字节
// } ReceiveData;
// #pragma pack(pop)


// class Datacheck
// {
// public:
//     enum ErrorCode
//     {
//         OK = 0,                         // 一切正常
//         READ_LOSS = 2,           // 读取信息长度不正确
//         READ_ERROR = 3,          //读取错误
//         WRITE_ERROR = 4          //写入错误
//     };
//     ReceiveData receive_data;
//     SendData send_data;
//     transporter::UartTransporter serial;
//     ErrorCode  error_code;


// public:
//     int  receive();
//     int send(float* data,uint8_t flag);
// };
// #endif //RM_VISON_SERIAL_DATACHECK_H
