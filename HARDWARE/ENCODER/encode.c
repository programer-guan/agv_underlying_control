#include "encode.h"
#include "timer.h"

static void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE );//��ӳ��
/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// ������������������ PB->6   PB->7 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           
*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	// ��������������ӳ������ PD->12   PD->13 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);   
	
	/*- TIM4������ģʽ���� -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 60000;									//�趨��������װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;								//50Hz��������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//�����˲���
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM4->CNT = 0;
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_Cmd(TIM4, ENABLE);   //����TIM4��ʱ��
}

void Encoder_Init(void)
{
  TIM4_Mode_Config();
}

/*------------End line - Lx ------------*/
