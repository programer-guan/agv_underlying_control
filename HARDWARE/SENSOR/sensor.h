#ifndef __SENSOR_H
#define __SENSOR_H

#include "sys.h" 
#include "adc.h"
#include "encode.h"

#define SENSOR_DLEN 23

typedef struct {
    int status;       //小车状态
    float power;      //电源电压
    float theta;      //方位角
    int encoder_ppr;  //旋转一圈脉冲数
    int encoder_delta_r;//右轮编码器
    int encoder_delta_l;
    int encoder_delta_car;//两车轮中心位移
    float omga_r;       //右轮转速
    float omga_l;
    float distance1;  //第一个超声波距离值
    float distance2;
    float distance3;
    float distance4;
    float IMU[9];    //9轴数据
    unsigned int time_stamp;//时间戳
}UPLOAD_STATUS;

extern UPLOAD_STATUS SENSOR_DATA;
extern s32 delta[100]; 

extern u8 send_data_2[5]; 
extern u8 send_data_4[7];

void SEND_SENSOR_DATA(void);
void GET_SENSOR_DATA(void);
void Get_power(void);
void Get_mpu9250(void);

void Milestones(void);
void send_frame_10byte(int *);
#endif

