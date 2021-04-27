#include "sys.h"
#include "usart.h"	 
#include "led.h"
#include "dma.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
/////////////////////////////////////USART1����//////////////////////////////////////////////// 
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�
u16 USART1_RX_STA=0;       //����״̬���
u16 SYS_EVENT = 0;
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);												//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);												//ʹ��AFIOʱ��
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); 														//������ӳ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);											//ʹ��USART1
	
/*
	//USART1_TX   GPIOA.9 ��ӳ�� GPIOB.6	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10 ��ӳ�� GPIOB.7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10
*/
	//USART1_TX	  GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART1_RX	  GPIOB.7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1
	
	//USART1 DMA����
	MYDMA_Config_Bite(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,119);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����88.
//USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA����      
//DMA_Cmd(DMA1_Channel4, ENABLE);  //����DMAͨ��
}

/////////////////////////////////////USART2����////////////////////////////////////////////////  
void uart2_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
  
	//USART2_TX   GPIOA.Z
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
  //USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3  

  //Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 
}
/////////////////////////////////////USART3����//////////////////////////////////////////////// 	
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA=0;       //����״̬���	  
  
void uart3_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
  //USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������3�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
}
/////////////////////////////////////ͨ�ô��ں���////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 



void USART_Send_Data(USART_TypeDef* USARTx,u8 *buf,u8 len)
{
	u8 t;
  for(t = 0;t < len; t++)		//ѭ����������
	{
		while((USARTx->SR&0X40)==0);
		USARTx->DR = buf[t];
	}
}

void USART_PUT_CHAR(USART_TypeDef* USARTx, u8 ch)
{
	while((USARTx->SR&0X40)==0);
	USARTx->DR = ch;
}


/////////////////////////////////////�����жϷ�����////////////////////////////////////////////////
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	static u8 reci_step = 0;										//���ݽ���״̬��
	static u8 length = 0,i = 0;									//ָ��ȼ���ָ��
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res = USART1->DR;					//(USART1->DR);	//��ȡ���յ�������
		switch(reci_step)
		{
			case 0:									//�����ж�֡ͷ0xcd
				if(Res == 0xcd){
					reci_step = 1;
					USART1_RX_STA = 1;		//��ʼ��������֡״̬
				}
				else reci_step = 0;
				break;
			case 1:										//�����ж�֡ͷ0xeb
				if(Res == 0xeb) reci_step = 2;
				else reci_step = 0;
				break;
			case 2:										//�����ж�֡ͷ0xd7
				if(Res == 0xd7) reci_step = 3;
				else reci_step = 0;
				break;
			case 3:										//����ָ���
				length = Res;
				i = 0;
				USART1_RX_BUF[i++] = Res;
				reci_step = 4;
				break;
			case 4:										//������������
				USART1_RX_BUF[i++] = Res;
				if(i > length){
					reci_step = 0;
					USART1_RX_STA = 2;		//����֡�������״̬
				}
				break;
			default:
				reci_step = 0;
				break;
		}
	}
}

void USART2_IRQHandler(void)                	 //����2�жϷ������
{
//	u8 Res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		Res = USART2->DR;				//(USART2->DR);	//��ȡ���յ�������
//  }
} 

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	static u8 rece_step = 0;										//���ݽ���״̬��
	static u8 len = 0,j = 0;										//ָ��ȼ���ָ��
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//�����ж�
	{
		Res = USART3->DR;				//(USART3->DR);		//��ȡ���յ�������
		switch(rece_step)
		{
			case 0:									//�����ж�֡ͷ0xcd
				if(Res == 0xcd){
					rece_step = 1;
					USART3_RX_STA = 1;	//��ʼ��������֡״̬
				}
				else rece_step = 0;
				break;
			case 1:									//�����ж�֡ͷ0xeb
				if(Res == 0xeb) rece_step = 2;
				else rece_step = 0;
				break;
			case 2:									//�����ж�֡ͷ0xd7
				if(Res == 0xd7) rece_step = 3;
				else rece_step = 0;
				break;
			case 3:									//����ָ���
				len = Res;
				j = 0;
				USART3_RX_BUF[j++] = Res;
				rece_step = 4;
				break;
			case 4:									//������������
				USART3_RX_BUF[j++] = Res;
				if(j > len){
					rece_step = 0;
					USART3_RX_STA = 2; //����֡�������״̬
				}
				break;
			default:
				rece_step = 0;
				break;
		}
	}
}




