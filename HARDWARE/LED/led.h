#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PEout(6)// PB5
#define LED1 PEout(5)// PE5
#define LED2 PFout(6)// PF6
#define LED3 PFout(7)// PF7
#define LED4 PFout(11)// PF11
#define LED5 PGout(9)// PF6


void LED_Init(void);//��ʼ��

		 				    
#endif
