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
#include "hmi.h"  	 //菜单模式
#include "can.h"
#include "pid.h"

void EVENT_PROCESS(void);

int main(void)
{ 
	delay_init();	     		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	uart_init(115200);	 //串口1初始化为115200
	uart2_init(115200);	 //串口2初始化为115200
	uart3_init(115200);	 //串口3初始化为115200
	TIM3_Int_Init(719,499);	//定时5ms 200Hz(7999,1999);			//分频

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0);//CAN普通模式初始化, 波特率500Kbps.CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
	LCD1602_Init();
 	LED_Init();		  	//初始化与LED连接的硬件接口
  KEY_Init();       //初始化与按键连接的硬件接口
  EXTIX_Init();		  //外部中断初始化  
	Adc_Init();		    //ADC初始化(含DMA配置)	
	MOTOR_INIT(); 

	HMI_INIT();
//PID_INIT(&PID_steer,3,0,3);
	while(1)
	{ 	
		EVENT_PROCESS();
	}
}

s32 delta[100];
//事件处理函数
//用于处理串口发送的非实时数据请求
void EVENT_PROCESS(void)
{
	static u8 key_code = 0;
	u8 i;
	u8 INS[USART_REC_LEN];
	u8 frame_length;
	u8 instruction;
	//串口中断事件处理
	if(USART1_RX_STA == 2)			//已经接受完一帧数据帧
	{
		for(i = 0; i < USART_REC_LEN; i++)//指令帧深拷贝
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
					if(count ==	0)			 //如果count=0，则为自动模式；反之则为手动模式
					{										
					MOTOR_STRAIGHT(INS[6]-100);
					delay_ms(5); 				//缺少延时导致下一条代码不生效
					SET_MOTOR_DIR(INS[7]-90);
					}
				}
				break;
			default:break;
		}
	}
	
	if(SYS_EVENT)
	{
		if(SYS_EVENT & 0x0001)				//5ms事件
		{
			GET_DIRECT_POS();
			key_code |= KEY_Scan(5);   //5是滤波频率
			if(count == 0) MOTOR_DIRECT(0,0);//PID控制转向
			SYS_EVENT ^= 0x0001;
		}
		
		if(SYS_EVENT & 0x0002)				//20ms事件
		{
			GET_SENSOR_DATA();          //将数据采集并发送给USART_DMA目标地址	
			SYS_EVENT ^= 0x0002;
		}
		
		if(SYS_EVENT & 0x0004)				//25ms事件
		{
  		HMI_KEY_EVENT(key_code);
			key_code = 0;
			SYS_EVENT ^= 0x0004;
		}
		
		if(SYS_EVENT &0x0008)					//500ms事件
		{
			Get_power();  
		//USART_PUT_CHAR(USART2,count);
			HMI_REFRESH();
			SYS_EVENT ^= 0x0008;
		}
	}
}
