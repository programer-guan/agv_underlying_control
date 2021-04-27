#include "stm32f10x.h"                  // Device header �⺯��ͷ�ļ�
#include "key.h"
#include "sys.h"               //������STM32��IO�������ȡ�궨�������궨��
#include "delay.h"
#include "usart.h"

//��������IO�ڳ�ʼ������
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTE,PORTCʱ��

	//��ʼ�� WK_UP-->GPIOA.0	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
	//KEY0-KEY2
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�͵�ƽ��Ч�����ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE 2,3,4
	
	//KEY3-KEY4
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;//PC6 PC7����
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC6
}


//����������
//thresh �붨ʱ�����ʹ�ã������˲����ÿ�β�����Ϊ���£���һ����ֵʱ���������£�ʵ�ְ����˲�

u8 KEY_Scan(u8 thresh)  //#define u8 unsigned char����u8����unsigned char���޷���8λ����������
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

