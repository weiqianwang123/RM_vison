// //
// // Created by oem on 2022/10/6.
// //

// #ifndef RM_VISON_SERIAL_COMMUNICATION_H
// #define RM_VISON_SERIAL_COMMUNICATION_H
// #pragma once

// #include "base.h"
// #include <opencv4/opencv2/opencv.hpp>

// #define PORT "/dev/ttyUSB0"
// #define FD -1
// #define SPEED 460800
// #define DATABITS 8
// #define STOPBITS 1
// #define PARITY "NONE"
// #define FLOW_CTRL 0
// #define COMPORT 1

// namespace transporter {

//     class UartTransporter : public Base
//     {
// //构造
//     public:
//         // 构造方式一: 直接传入参数
//         UartTransporter(std::string port = PORT, int baud = SPEED, int flow_ctrl = FLOW_CTRL, int databits=DATABITS, int stopbits=STOPBITS, std::string parity = PARITY):
//                 port_(port),
//                 baud_(baud),
//                 flow_ctrl_(flow_ctrl),
//                 databits_(databits),
//                 stopbits_(stopbits),
//                 parity_(parity)
//         {}
//         // 构造方式二: 使用参数文件
//         UartTransporter(std::string config_flie_path);
//         ~UartTransporter() {close();}

// // 继承功能
//     public:
//         bool open() override;
//         void close() override;
//         int read(void * buffer, size_t len) override;
//         int write(const void * buffer, size_t len) override;
//         bool is_open() override {return is_open_;}
//         std::string error_message() override {return error_message_;}

// // 设备文件描述符
//     private:
//         int fd_{-1};

// // 设备状态
//     private:
//         bool is_open_{false};
//         std::string error_message_;

// // 参数
//     private:
//         std::string port_;
//         int baud_;
//         int flow_ctrl_;
//         int databits_;
//         int stopbits_;
//         std::string parity_;
//     };


// }

// #endif //RM_VISON_SERIAL_COMMUNICATION_H
