#ifndef __MOTO_H
#define __MOTO_H

#include "sys.h" 
#include "hmi.h"

#define DIR_POS_ADC_CH ADC_Channel_10
#define DIR_POW_ADC_CH ADC_Channel_11

#define POS_LR PGin(13)
#define POS_M PGin(11)

extern int angle;

void MOTOR_GPIO_INIT(void);
void MOTOR_INIT(void);
void MOTOR_STRAIGHT(int var);
void MOTOR_DIRECT(int ins, int var);

float medium(float a,float b,float c);
void MOTOR_CALIBRATE(void);
u16 GET_DIRECT_POS(void);

u8 SET_MOTOR_DIR(float dst_ang);

char *inttohex(int aa);			//将十进制转换为十六进制以字符的形式存放到数组buffer中
void dec16hex(int num,char *hex);
#endif
