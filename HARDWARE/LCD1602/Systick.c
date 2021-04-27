#include "stm32f10x.h"
#include "Systick.h"
#include "core_cm3.h"  //Systick是内核片上外设
/*
	启动滴答定时器
*/
 
 
// 1微秒延时
void SysTick_Delay_Us( unsigned int us) 
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	
	for(i=0;i<us;i++)
	{
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
 
//1毫秒延时
void SysTick_Delay_Ms( unsigned int  ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}
