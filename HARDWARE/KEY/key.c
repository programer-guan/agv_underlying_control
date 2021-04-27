#include "stm32f10x.h"                  // Device header 库函数头文件
#include "key.h"
#include "sys.h"               //定义了STM32的IO口输入读取宏定义和输出宏定义
#include "delay.h"
#include "usart.h"

//按键输入IO口初始化函数
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE,PORTC时钟

	//初始化 WK_UP-->GPIOA.0	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
	//KEY0-KEY2
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //低电平有效，设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 2,3,4
	
	//KEY3-KEY4
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;//PC6 PC7按键
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC6
}


//按键处理函数
//thresh 与定时器配合使用，采样滤波如果每次采样都为按下，则一定阈值时间后输出按下，实现按键滤波

u8 KEY_Scan(u8 thresh)  //#define u8 unsigned char，用u8代替unsigned char（无符号8位二进制数）
{
	static u8 key_filter[6] = {0,0,0,0,0,0};
	u8 key = 0;
	u8 i;
	
	if(WK_UP == 1)key_filter[0]++;
	else key_filter[0] = 0;
	
	if(KEY0 == 0)key_filter[1]++;
	else key_filter[1] = 0;
	
	if(KEY1 == 0)	key_filter[2]++;
	else key_filter[2] = 0;
	
	if(KEY2 == 0) key_filter[3]++;
	else key_filter[3] = 0;
	
	if(KEY3 == 0) key_filter[4]++;
	else key_filter[4] = 0;
	
	if(KEY4 == 0) key_filter[5]++;
	else key_filter[5] = 0;
	
	for(i = 0; i < 6; i++){
	if(key_filter[i] > thresh) {key |= 1<<i; key_filter[i] = thresh;}
	}
	
	return key;
}

