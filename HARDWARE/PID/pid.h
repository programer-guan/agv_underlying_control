#ifndef __PID_H
#define __PID_H	
#include "sys.h"

typedef struct
{
	float Kp;					//����ϵ��
	float Ki;					//���ֳ���
	float Kd;					//΢�ֳ���
	float dst;				//Ŀ�����λ��
	float cur;				//��ǰ����λ��
	float error_k;		//��ǰ���
	float error_k1;		//ǰһ�ο������
	float error_i;		//������
	float error_k2;		//ǰǰ�ο������(ֻ��������ʽPID)
}PID_Type;

extern PID_Type PID_steer;

void PID_INIT(PID_Type* p,float Kp,float Ki,float Kd);
float PID_Position_CALC(PID_Type* p);
float PID_Increment_CALC(PID_Type* p);

#endif 
