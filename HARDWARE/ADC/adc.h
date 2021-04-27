#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

void Adc_Init(void);//不同于函数ADC_Init()

//u16 Get_Adc_Data(u8 ch);
u16 Filter(u8 ch);
u16 Get_Adc(ADC_TypeDef* adc,u8 ch); 
u16 Get_Adc_Average(ADC_TypeDef* adc, u8 ch,u8 times, u8 delay_t);

void BubbleSort(uint8_t column);
#endif 
