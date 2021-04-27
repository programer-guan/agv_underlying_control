#include "pid.h"

PID_Type PID_steer;

void PID_INIT(PID_Type* p,float Kp,float Ki,float Kd)
{
	p->Kp = Kp;
	p->Ki = Ki;
	p->Kd = Kd;
	
	p->cur = 0;
	p->dst = 0;
	p->error_k = 0;
	p->error_k1 = 0;
	p->error_k2 = 0;
}
/**********************************************************/
/********************λ��ʽPID����*************************/
/**********************************************************/
float PID_Position_CALC(PID_Type* p)
{
	float pid_real;
	p->error_k = p->dst - p->cur;											//calculate the error of dest and current
	p->error_i += p->error_k;
	pid_real = p->Kp*p->error_k + p->Ki*p->error_i + p->Kd*(p->error_k-p->error_k1);
	p->error_k1 = p->error_k;
	return pid_real;
}
/**********************************************************/
/********************����ʽPID����*************************/
/**********************************************************/
float PID_Increment_CALC(PID_Type* p)
{
	float pid_real;
	p->error_k  = p->dst - p->cur;																								//��ǰ���
	pid_real	= p->error_k ;
//pid_real = p->Kp*(p->error_k-p->error_k1) + p->Ki*p->error_k + p->Kd*(p->error_k-2*p->error_k1+p->error_k2);
	p->error_k2 = p->error_k1;																										//ǰǰ�ο������(ֻ��������ʽPID)
	p->error_k1 = p->error_k;
	return pid_real;
}  

