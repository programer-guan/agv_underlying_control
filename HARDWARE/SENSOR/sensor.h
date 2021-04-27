#ifndef __SENSOR_H
#define __SENSOR_H

#include "sys.h" 
#include "adc.h"
#include "encode.h"

#define SENSOR_DLEN 23

typedef struct {
    int status;       //С��״̬
    float power;      //��Դ��ѹ
    float theta;      //��λ��
    int encoder_ppr;  //��תһȦ������
    int encoder_delta_r;//���ֱ�����
    int encoder_delta_l;
    int encoder_delta_car;//����������λ��
    float omga_r;       //����ת��
    float omga_l;
    float distance1;  //��һ������������ֵ
    float distance2;
    float distance3;
    float distance4;
    float IMU[9];    //9������
    unsigned int time_stamp;//ʱ���
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

