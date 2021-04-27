#include "sys.h"
#include "usart.h"	 
#include "led.h"
#include "dma.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
/////////////////////////////////////USART1配置//////////////////////////////////////////////// 
u8 USART1_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节
u16 USART1_RX_STA=0;       //接收状态标记
u16 SYS_EVENT = 0;
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);												//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);												//使能AFIO时钟
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); 														//开启重映射
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);											//使能USART1
	
/*
	//USART1_TX   GPIOA.9 重映射 GPIOB.6	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10 重映射 GPIOB.7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10
*/
	//USART1_TX	  GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART1_RX	  GPIOB.7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1
	
	//USART1 DMA配置
	MYDMA_Config_Bite(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,119);//DMA1通道4,外设为串口1,存储器为SendBuff,长度88.
//USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送      
//DMA_Cmd(DMA1_Channel4, ENABLE);  //启动DMA通道
}

/////////////////////////////////////USART2配置////////////////////////////////////////////////  
void uart2_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  
	//USART2_TX   GPIOA.Z
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2 
}
/////////////////////////////////////USART3配置//////////////////////////////////////////////// 	
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口3接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3 
}
/////////////////////////////////////通用串口函数////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 



void USART_Send_Data(USART_TypeDef* USARTx,u8 *buf,u8 len)
{
	u8 t;
  for(t = 0;t < len; t++)		//循环发送数据
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


/////////////////////////////////////串口中断服务函数////////////////////////////////////////////////
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	static u8 reci_step = 0;										//数据接收状态机
	static u8 length = 0,i = 0;									//指令长度计数指针
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART1->DR;					//(USART1->DR);	//读取接收到的数据
		switch(reci_step)
		{
			case 0:									//接收判断帧头0xcd
				if(Res == 0xcd){
					reci_step = 1;
					USART1_RX_STA = 1;		//开始接收数据帧状态
				}
				else reci_step = 0;
				break;
			case 1:										//接收判断帧头0xeb
				if(Res == 0xeb) reci_step = 2;
				else reci_step = 0;
				break;
			case 2:										//接收判断帧头0xd7
				if(Res == 0xd7) reci_step = 3;
				else reci_step = 0;
				break;
			case 3:										//接收指令长度
				length = Res;
				i = 0;
				USART1_RX_BUF[i++] = Res;
				reci_step = 4;
				break;
			case 4:										//接收完整数据
				USART1_RX_BUF[i++] = Res;
				if(i > length){
					reci_step = 0;
					USART1_RX_STA = 2;		//数据帧接收完成状态
				}
				break;
			default:
				reci_step = 0;
				break;
		}
	}
}

void USART2_IRQHandler(void)                	 //串口2中断服务程序
{
//	u8 Res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		Res = USART2->DR;				//(USART2->DR);	//读取接收到的数据
//  }
} 

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	static u8 rece_step = 0;										//数据接收状态机
	static u8 len = 0,j = 0;										//指令长度计数指针
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收中断
	{
		Res = USART3->DR;				//(USART3->DR);		//读取接收到的数据
		switch(rece_step)
		{
			case 0:									//接收判断帧头0xcd
				if(Res == 0xcd){
					rece_step = 1;
					USART3_RX_STA = 1;	//开始接收数据帧状态
				}
				else rece_step = 0;
				break;
			case 1:									//接收判断帧头0xeb
				if(Res == 0xeb) rece_step = 2;
				else rece_step = 0;
				break;
			case 2:									//接收判断帧头0xd7
				if(Res == 0xd7) rece_step = 3;
				else rece_step = 0;
				break;
			case 3:									//接收指令长度
				len = Res;
				j = 0;
				USART3_RX_BUF[j++] = Res;
				rece_step = 4;
				break;
			case 4:									//接收完整数据
				USART3_RX_BUF[j++] = Res;
				if(j > len){
					rece_step = 0;
					USART3_RX_STA = 2; //数据帧接收完成状态
				}
				break;
			default:
				rece_step = 0;
				break;
		}
	}
}




