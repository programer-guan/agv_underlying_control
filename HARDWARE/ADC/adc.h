#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

void Adc_Init(void);//��ͬ�ں���ADC_Init()

//u16 Get_Adc_Data(u8 ch);
u16 Filter(u8 ch);
u16 Get_Adc(ADC_TypeDef* adc,u8 ch); 
u16 Get_Adc_Average(ADC_TypeDef* adc, u8 ch,u8 times, u8 delay_t);

void BubbleSort(uint8_t column);
#endif 
