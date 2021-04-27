#ifndef __PID_H
#define __PID_H	
#include "sys.h"

typedef struct
{
	float Kp;					//比例系数
	float Ki;					//积分常数
	float Kd;					//微分常数
	float dst;				//目标控制位置
	float cur;				//当前控制位置
	float error_k;		//当前误差
	float error_k1;		//前一次控制误差
	float error_i;		//误差积分
	float error_k2;		//前前次控制误差(只用于增量式PID)
}PID_Type;

extern PID_Type PID_steer;

void PID_INIT(PID_Type* p,float Kp,float Ki,float Kd);
float PID_Position_CALC(PID_Type* p);
float PID_Increment_CALC(PID_Type* p);

#endif 
