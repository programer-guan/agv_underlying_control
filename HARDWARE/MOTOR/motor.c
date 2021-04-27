#include "motor.h"
#include "usart.h"
#include "adc.h"
#include "led.h"
#include "can.h"
#include "pid.h"
#include "delay.h"
#include "usart.h"
#include "sensor.h"
#include "stdlib.h"

u16 adc_r = 3550;
u16 adc_m = 1800;
u16 adc_l = 50;

u8 canbuf_turn[]  	 = {0x01,0x01};											  								//ת������������ģʽ��ʼ
u8 canbuf_straight[] = {0x01,0x0B};											  								//ֱ������������ģʽ��ʼ
u8 canbuf_revise[] 	 = {0x2F,0x60,0x60,0x00,0x03,0x00,0x00,0x00}; 		    //�޸�������ģʽ
u8 canbuf_abile[]    = {0x2B,0x40,0x60,0x00,0x0F,0x00,0x00,0x00};         //ʹ��������
u8 canbuf_disabile[] = {0x2B,0x40,0x60,0x00,0x07,0x00,0x00,0x00};       	//������ʹ�ܶϿ�

float medium(float a,float b,float c)
{
	if((a > b && b >= c) || (c > b && b >= a))return b;
	if((a > c && c >= b) || (b > c && c >= a))return c;
	if((c > a && a >  b) || (b > a && a >= c))return a;
	
	return 0;
}

void MOTOR_GPIO_INIT(void)
{
	//��ʼ����λ����
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOC, ENABLE);	
	//������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//���λ�õ�λ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 //PC0---���λ�õ�λ��  PC1---��Դ��ѹ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//ǰ���������
//���룺ins ����ָ��   var ������
//���������ʼ������
void MOTOR_INIT(void)
{ 
	LED1=1; 
	MOTOR_GPIO_INIT();
  Encoder_Init();
	delay_ms(100);
	//ֱ�е����ʼ��
	Can_Send_Msg(0X00000000,canbuf_straight,2);
	Can_Send_Msg(0X0000060B,canbuf_revise,8);
//Can_Send_Msg(0X0000060B,canbuf_abile,8);
	delay_ms(50);
	//ת������ʼ��
	Can_Send_Msg(0X00000000,canbuf_turn,2);	  //��ʼ
	Can_Send_Msg(0X00000601,canbuf_revise,8);	//�޸�������ģʽ
//Can_Send_Msg(0X00000601,canbuf_abile,8);	//ʹ��������
	
	LED1=0;
	delay_ms(10);
//MOTOR_CALIBRATE();
}
/************************************************************************
*****AGVС�������Զ�ģʽ
***��Ϊֱ�е�����ƺ���	+	ת�������ƺ���
************************************************************************/	 
//ֱ�е�����ƺ���
void MOTOR_STRAIGHT(int var)
{
	u8 canbuf_speed1[8]; 												//�������ٶ�	��32λΪ�ٶ��趨ֵ����λ�ں�1000r/min
	Can_Send_Msg(0X0000060B,canbuf_abile,8);	  //ʹ��������	
	delay_us(500);
	int speed = (float)1000*10/60*3840*var/100;	//�ٶȿ���
	if(var== 0)Can_Send_Msg(0X00060B,canbuf_disabile,8);
	
	canbuf_speed1[0] = 0x23;
	canbuf_speed1[1] = 0xFF;
	canbuf_speed1[2] = 0x60;
	canbuf_speed1[3] = 0x00;

	canbuf_speed1[4] = *((u8 *)(&speed)+0);
	canbuf_speed1[5] = *((u8 *)(&speed)+1);
	canbuf_speed1[6] = *((u8 *)(&speed)+2);
	canbuf_speed1[7] = *((u8 *)(&speed)+3);

  Can_Send_Msg(0X00060B,canbuf_speed1,8);//�������ٶ�	
	delay_us(500);
	
#ifdef DEBUG_MODE0
	if(speed > 0)USART_Send_Data(USART2,"forward ",8);//STEP FORWARD %d, var     i r8 0x8000,��������0��ǰ��
	if(speed < 0)USART_Send_Data(USART2,"backward ",9);
	if(speed ==0)USART_Send_Data(USART2,"stop ",5);
#endif
}	

//ת�������ƺ���
//���룺ins ����ָ��:FΪ�ң�BΪ��   var ������
void MOTOR_DIRECT(int ins, int var)
{
	int angle;
	int v;
	u8 canbuf_speed2[8];
	int velocity;
	Can_Send_Msg(0X00000601,canbuf_abile,8);	
	delay_us(500);
	if(ins == 0)
	{
		angle = PID_Increment_CALC(&PID_steer);		//����PID
		
  //printf("%d ",angle);											//��ӡ�����ֵ
		
		if(abs(angle) <= 3) v = 0;								//ǿ�ƹ���
		if(angle > 3) v = medium(-6,angle,-11);		//˫��
		if(angle <-3) v = medium(6,angle,11);
		}
	else	
		v = var;

	if((SENSOR_DATA.theta	>100) && (v	<0)) v = 0;	 //С��ת��������������������������ת��С��������ת��
	if((SENSOR_DATA.theta	<-100)&& (v	>0)) v = 0;
	
	if(v == 0) Can_Send_Msg(0X000601,canbuf_disabile,8);

	velocity = (float)1000*10/60*6144*v/100*10;	//���ٶȿ���
	canbuf_speed2[0] = 0x23;
	canbuf_speed2[1] = 0xFF;
	canbuf_speed2[2] = 0x60;
	canbuf_speed2[3] = 0x00;
	
	canbuf_speed2[4] = *((u8 *)(&velocity)+0);
	canbuf_speed2[5] = *((u8 *)(&velocity)+1);
	canbuf_speed2[6] = *((u8 *)(&velocity)+2);
	canbuf_speed2[7] = *((u8 *)(&velocity)+3);
	
	Can_Send_Msg(0X000601,canbuf_speed2,8);
	delay_us(800);

		
#ifdef DEBUG_MODE0
	if(velocity > 0)USART_Send_Data(USART2,"left\n",5);//STEP FORWARD %d, var     i r8 0x8000,��������0��ǰ��
	if(velocity < 0)USART_Send_Data(USART2,"right\n",6);
	if(velocity ==0)USART_Send_Data(USART2,"turnstop\n",9);
#endif
}

//ת�����Ƕ�����������
void MOTOR_CALIBRATE(void)
{
	
}

//��ȡ��ǰ���ת��ĽǶ�
//����ֵ����ǰ���ת��
//��Χ������135��
u16 GET_DIRECT_POS(void)
{
	float ang;
	u16 ang_adc;
	ang_adc = Filter(0);//���λ��ADֵ
	ang = ang_adc-adc_m;//��ȥ����λadcֵ
	ang /= (adc_r-adc_l)*0.5;	//��һ��
	ang *= 135;																//ת���ɽǶ�
	
	if(ang > 135) ang = 135;
	else if(ang < -135) ang = -135;
	PID_steer.cur = ang;
	
	SENSOR_DATA.theta = ang;
	return ang;
}

//���ò�ת��MOTOR�ķ���
//���룺Ŀ����ת��
//����ֵ��
u8 SET_MOTOR_DIR(float dst_ang)
{
	if(dst_ang > 90) dst_ang = 90;
	if(dst_ang < -90) dst_ang = -90;
	
	PID_steer.dst = dst_ang;
	
	return 0;
}


