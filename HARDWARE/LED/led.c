#include "led.h"
    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|
													RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				 //LED0 1-->PE 6 5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;				 //LED2 3 4-->PF 6 7 11 �˿�����
 GPIO_Init(GPIOF, &GPIO_InitStructure);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED2 3 4-->PF 6 7 11 �˿�����
 GPIO_Init(GPIOG, &GPIO_InitStructure);
}
 
