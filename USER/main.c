#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "dma.h"
#include "usart.h"
#include "exti.h"
#include "motor.h"
#include "timer.h"
#include "sensor.h"
#include "lcd1602.h"
#include "hmi.h"  	 //�˵�ģʽ
#include "can.h"
#include "pid.h"

void EVENT_PROCESS(void);

int main(void)
{ 
	delay_init();	     		//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	uart_init(115200);	 //����1��ʼ��Ϊ115200
	uart2_init(115200);	 //����2��ʼ��Ϊ115200
	uart3_init(115200);	 //����3��ʼ��Ϊ115200
	TIM3_Int_Init(719,499);	//��ʱ5ms 200Hz(7999,1999);			//��Ƶ

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0);//CAN��ͨģʽ��ʼ��, ������500Kbps.CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
	LCD1602_Init();
 	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
  KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�
  EXTIX_Init();		  //�ⲿ�жϳ�ʼ��  
	Adc_Init();		    //ADC��ʼ��(��DMA����)	
	MOTOR_INIT(); 

	HMI_INIT();
//PID_INIT(&PID_steer,3,0,3);
	while(1)
	{ 	
		EVENT_PROCESS();
	}
}

s32 delta[100];
//�¼�������
//���ڴ����ڷ��͵ķ�ʵʱ��������
void EVENT_PROCESS(void)
{
	static u8 key_code = 0;
	u8 i;
	u8 INS[USART_REC_LEN];
	u8 frame_length;
	u8 instruction;
	//�����ж��¼�����
	if(USART1_RX_STA == 2)			//�Ѿ�������һ֡����֡
	{
		for(i = 0; i < USART_REC_LEN; i++)//ָ��֡���
		{
			INS[i] = USART1_RX_BUF[i];
		}
		USART1_RX_STA = 0;
		frame_length = INS[0];
		instruction = INS[1];
		switch(instruction)
		{
			case 0x74:
				if(frame_length == 9){
					if(count ==	0)			 //���count=0����Ϊ�Զ�ģʽ����֮��Ϊ�ֶ�ģʽ
					{										
					MOTOR_STRAIGHT(INS[6]-100);
					delay_ms(5); 				//ȱ����ʱ������һ�����벻��Ч
					SET_MOTOR_DIR(INS[7]-90);
					}
				}
				break;
			default:break;
		}
	}
	
	if(SYS_EVENT)
	{
		if(SYS_EVENT & 0x0001)				//5ms�¼�
		{
			GET_DIRECT_POS();
			key_code |= KEY_Scan(5);   //5���˲�Ƶ��
			if(count == 0) MOTOR_DIRECT(0,0);//PID����ת��
			SYS_EVENT ^= 0x0001;
		}
		
		if(SYS_EVENT & 0x0002)				//20ms�¼�
		{
			GET_SENSOR_DATA();          //�����ݲɼ������͸�USART_DMAĿ���ַ	
			SYS_EVENT ^= 0x0002;
		}
		
		if(SYS_EVENT & 0x0004)				//25ms�¼�
		{
  		HMI_KEY_EVENT(key_code);
			key_code = 0;
			SYS_EVENT ^= 0x0004;
		}
		
		if(SYS_EVENT &0x0008)					//500ms�¼�
		{
			Get_power();  
		//USART_PUT_CHAR(USART2,count);
			HMI_REFRESH();
			SYS_EVENT ^= 0x0008;
		}
	}
}
