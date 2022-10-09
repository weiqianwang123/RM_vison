//
// Created by oem on 2022/10/6.
//
#include "serial_datacheck.h"
int Datacheck::send(float* data, uint8_t flag)
{
//    tcflush(communication.get_fd(), TCOFLUSH);

    // 将数据转换为 u_char
    send_data.Data[0] = flag;
    int16_t pitch, yaw;
    u_char pitch_, _pitch, yaw_, _yaw;

    pitch = (int16_t) (data[0] * 32768.0 / 180.0);
    pitch_ = pitch >> 8;
    _pitch = pitch & 0xFF;

    yaw = (int16_t) (-data[1] * 32768.0 / 180.0);     // 取反
    yaw_ = yaw >> 8;
    _yaw = yaw & 0xFF;


    send_data.Data[1] = (u_char) pitch_;
    send_data.Data[2] = (u_char) _pitch;
    send_data.Data[3] = (u_char) yaw_;
    send_data.Data[4] = (u_char) _yaw;

    send_data.SOF = SERIAL_SOF;
    send_data.END = SERIAL_EOF;


    if(serial.write(&send_data, sizeof(send_data)) == false)  return error_code = WRITE_ERROR;

    return error_code =  OK;
}

int Datacheck::receive()
{

    uint8_t commands[READ_BYTE] = {0};
    uint8_t buffer[4*READ_BYTE] = {0};
    memset(&receive_data, 0, sizeof(receive_data));
    int readCount = -1;
    readCount = serial.read(buffer,sizeof(receive_data));

    if (readCount != sizeof(receive_data))
    {
        return error_code = READ_LOSS;
    }

    memcpy(&receive_data, buffer, sizeof(receive_data));

    if ((receive_data.SOF != SERIAL_SOF)  ||  (receive_data.END != SERIAL_EOF))
    {
        return error_code = READ_ERROR;
    }


    return error_code = OK;


}
