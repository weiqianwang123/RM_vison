#include "can_serial.h"


int CanSerial::send_state(int state, const int target_id,int is_fire)
{
    u_char buf[2];
    buf[0] = is_fire;
    buf[0] = buf[0]<<1 | state;//是否可以打击右移一位，最后一位state为是否识别到
    // cout<<"state:"<<state<<endl;
    int id = target_id; //暂时忽略了
    return can_send(STATE_SEND_ID, buf, 2);
    //buf[1]忽略不使用
}

int CanSerial::send_data(float pitch_predict_camera, float yaw_predict_camera, u_int32_t timestamp)
{
    u_char data[8];
    int16_t pitch = (int16_t)(pitch_predict_camera * 32768.0 / 180.0);
    int16_t yaw = (int16_t)(yaw_predict_camera * 32768.0 / 180.0);
    memcpy(&data[0], &pitch, sizeof(pitch));
    memcpy(&data[2], &yaw, sizeof(yaw));
    memcpy(&data[4], &timestamp, sizeof(timestamp));
    // cout<<"the datas:"<<hex<<(int)data[0]<<","<<(int)data[1]<<","<<(int)data[2]<<","<<(int)data[3]<<endl;
    return can_send(DATA_SEND_ID, data, 8);
}



int CanSerial::can_send(uint id, u_char *buf, u_char dlc)
{
    if (dlc > 8)    return DLC_ERROR;

    struct can_frame send_frame;

    send_frame.can_id = id;
    send_frame.can_dlc = dlc;

    for (int i = 0; i < (int)dlc; i++)
        send_frame.data[i] = buf[i];
    
    int t = write(socket_fd, &send_frame, sizeof(send_frame));
    if (t > 0)  return SUCCESS;
    return WRITE_ERROR;
}

int CanSerial::can_receive(uint &id, u_char *buf, u_char &dlc)
{
    struct can_frame frame;
    int t = read(socket_fd, &frame, sizeof(frame));
    if (t <= 0) return READ_ERROR;

    id = frame.can_id;
    dlc = frame.can_dlc;

    memcpy(buf, frame.data, dlc);
    return SUCCESS;
}


CanSerial::CanSerial()
{
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // 配置 Socket CAN 为非阻塞IO
    int flags=fcntl(socket_fd, F_GETFL, 0); 
    flags |= O_NONBLOCK;
    fcntl(socket_fd, F_SETFL, flags);

    // 指定can设备
    strcpy(interface_request.ifr_name, CAN_NAME);
    ioctl(socket_fd, SIOCGIFINDEX, &interface_request);
    addr.can_family = AF_CAN;
    addr.can_ifindex = interface_request.ifr_ifindex;
    
    // 将套接字与can设备绑定
    bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    
    // 过滤规则
    /**再学一下**/
    
}


void CanSerial::transformIMU(u_char *IMU_data, float *imu_data){
    memcpy(imu_data, IMU_data, sizeof(u_char) * 16);
}


void CanSerial::transformIMU_Time(u_char *IMU_time, uint32_t &time)
{
    time = 0;
    for (int i = 0; i < 4; i++)
    {
        time |=  ((u_int32_t)IMU_time[i] << (i * 8));
    }
}
