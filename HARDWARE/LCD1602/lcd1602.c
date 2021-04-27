/*
        1602.c�ļ�
	1602�������ߴ���
*/
#include "lcd1602.h"
#include "delay.h" 
#include "sensor.h"
#include "usart.h"

void LCD1602_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);
       //dataIO GPIOF4~7��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);				
        // RS RW E IO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);				
}
 
/* ���Һ��æ��æ*/
void LCD1602_Wait_Ready(void)
{
	int8_t sta;
	
	DATAOUT(0xff);    //PA�˿�ȫ����1
	LCD_RS_Clr();     //RS 0
	LCD_RW_Set();     //RW 1
	do
	{
		LCD_EN_Set();   //EN 1
		delay_ms(1);  	//��ʱ5ms  �����õδ�ʱ�������ʱ���������µ�ʱ�̳���Ῠ��������ԭ���֪!
 		sta = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);//��ȡ״̬��
		LCD_EN_Clr();  //EN  0
	}while(sta & 0x80);//bit7=1��ʾæ���ظ���⵽��Ϊ0ֹͣ
}
 
/*��1602д��һ�ֽ�����,cmd-��д������ */
//д��ָ��PA4-7 RS-0  RW-0
void LCD1602_Write_Cmd(u8 cmd)
{
	LCD1602_Wait_Ready();  //�ж�æ��æ
	LCD_RS_Clr();   
	LCD_RW_Clr();
	DATAOUT(cmd);  //�ȴ�����λ
	LCD_EN_Set();
	LCD_EN_Clr();
	
	DATAOUT(cmd<<4); //����λ���Ƶ�����λ
	LCD_EN_Set();
	LCD_EN_Clr();
}
 
/* ��1602д��һ�ֽ�����*/
void LCD1602_Write_Dat(u8 dat)
{
	LCD1602_Wait_Ready(); //�ж�æ��æ
	LCD_RS_Set();   //1
	LCD_RW_Clr();   //0
	
	DATAOUT(dat);   //�ȴ�����λ
	LCD_EN_Set();  
	LCD_EN_Clr();
	
	DATAOUT(dat<<4); //����λ���Ƶ�����λ
	LCD_EN_Set(); 
	LCD_EN_Clr();
	
}
 
/*����*/
void LCD1602_ClearScreen(void)
{
	LCD1602_Write_Cmd(0x01);
	
}
 
/*����RAM��ʼ��ַ,�����λ��,(X,Y)��Ӧ��Ļ�ϵ��ַ����� */
void LCD1602_Set_Cursor(u8 x, u8 y)
{
	u8 addr;
	
	if (y == 0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LCD1602_Write_Cmd(addr | 0x80);
}

void LCD1602_Show_char(u8 x, u8 y, u8 c)
{
	LCD1602_Set_Cursor(x, y);
	LCD1602_Write_Dat(c);
}

/*��Һ������ʾ�ַ���,(X,Y)-��Ӧ��Ļ�ϵ���ʼ����,str-?�ַ���ָ�� */
void LCD1602_Show_Str(u8 x, u8 y, u8 *str)
{
	LCD1602_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}
 
/* ��ʼ��Һ��*/
void LCD1602_Init(void)
{
	LCD1602_GPIO_Configuration();
	
	LCD1602_Write_Cmd(0x28);	//16*2��ʾ,5*7����,4λ���ݿ�
	LCD1602_Write_Cmd(0x0C);	//����ʾ�����ر�
	LCD1602_Write_Cmd(0x06);	//���ֲ�������ַ�Զ�+1
	LCD1602_Write_Cmd(0x01);	//����
	
	LCD1602_Show_Str(0,0,"*WELCOME TO USE*"); //lcd1602����ʾ
	LCD1602_Show_Str(0,2,"***NENU***AGV***"); //lcd1602����ʾ
}

void LCD1602_clear(void)
{
	LCD1602_Write_Cmd(0x01);	
}
/*
void LCD1602_Refresh(u8 mode)
{
	u8 ch_1602[10];
	s32 temp;
	if(mode == 0)									//��ʾMPU9250���� 
	{
		LCD1602_Write_Cmd(0x01);	//����
		LCD1602_Show_Str(0,0,"P"); //lcd1602����ʾ
		LCD1602_Show_Str(0,1,"R");
		LCD1602_Show_Str(5,0,"Y");
		LCD1602_Show_Str(6,1,"x");
		LCD1602_Show_Str(11,0,"y");
		LCD1602_Show_Str(11,1,"z");
		
		temp=(int)SENSOR_DATA.IMU[0];		//lcd1602��ʾ������
		if(temp<0)
		{
			LCD1602_Show_Str(1,0,"-");
			temp=-temp;
		}else LCD1602_Show_Str(1,0," ");
			
		ch_1602[0] = (temp/100)%10+'0';
		ch_1602[1] = (temp/10)%10+'0';
		ch_1602[2] = temp%10+'0';
		ch_1602[3] = 0;
		LCD1602_Show_Str(2,0,ch_1602);

		temp=(int)SENSOR_DATA.IMU[1];		//lcd1602��ʾ�����
		if(temp<0)
		{
			LCD1602_Show_Str(1,1,"-");
			temp=-temp;
		}else LCD1602_Show_Str(1,1," ");
		ch_1602[0] = (temp/100)%10+'0';
		ch_1602[1] = (temp/10)%10+'0';
		ch_1602[2] = temp%10+'0';
		ch_1602[3] = 0;
		LCD1602_Show_Str(2,1,ch_1602);
			
		temp=(int)SENSOR_DATA.IMU[2];		//lcd1602��ʾ�����
		if(temp<0)
		{
			LCD1602_Show_Str(6,0,"-");
			temp=-temp;
		}else LCD1602_Show_Str(6,0," ");
		ch_1602[0] = (temp/100)%10+0x30;
		ch_1602[1] = (temp/10)%10+0x30;
		ch_1602[2] = temp%10+0x30;
		ch_1602[3] = 0;
		LCD1602_Show_Str(7,0,ch_1602);
		
		temp=(int)SENSOR_DATA.IMU[6];			//gyrox
		if(temp<0)
			{
				LCD1602_Show_Str(7,1,"-");
				temp=-temp;
			}else LCD1602_Show_Str(7,1," ");
			ch_1602[0] = (temp/100)%10+0x30;
			ch_1602[1] = (temp/10)%10+0x30;
			ch_1602[2] = temp%10+0x30;
			ch_1602[3] = 0;
			LCD1602_Show_Str(8,1,ch_1602);
			
		temp=(int)SENSOR_DATA.IMU[7];				//gyroy
		if(temp<0)
		{
			LCD1602_Show_Str(12,0,"-");
			temp=-temp;
		}else LCD1602_Show_Str(12,0," ");
			ch_1602[0] = (temp/100)%10+0x30;
			ch_1602[1] = (temp/10)%10+0x30;
			ch_1602[2] = temp%10+0x30;
			ch_1602[3] = 0;
			LCD1602_Show_Str(13,0,ch_1602);
		
		temp=(int)SENSOR_DATA.IMU[8];					////gyroz
		if(temp<0)
		{
				LCD1602_Show_Str(12,1,"-");
				temp=-temp;
		}else LCD1602_Show_Str(12,1," ");
		ch_1602[0] = (temp/100)%10+0x30;
		ch_1602[1] = (temp/10)%10+0x30;
		ch_1602[2] = temp%10+0x30;
		ch_1602[3] = 0;
		LCD1602_Show_Str(13,1,ch_1602);
	}
	
	else if(mode == 1)							//��ʾ������ַ��������������
	{
		LCD1602_Write_Cmd(0x01);	    //����
		LCD1602_Show_Str(0,0,"P");
		LCD1602_Show_Str(0,8,"A");
		LCD1602_Show_Str(0,1,"E");
		
		temp = GET_DIRECT_POS();     //��ȡ��ǰ���ת��ĽǶ�
		ch_1602[0] = (temp/1000)%10 + 0x30;
		ch_1602[1] = (temp/100)%10 + 0x30;
		ch_1602[2] = (temp/10)%10 + 0x30;
		ch_1602[3] = temp%10 + 0x30;
		ch_1602[4] = 0;
		LCD1602_Show_Str(9,0,ch_1602);
		
		temp = (int)(SENSOR_DATA.theta*10);//��λ��
		if(temp<0)
		{
				LCD1602_Show_Str(1,0,"-");
				temp=-temp;
		}else LCD1602_Show_Str(1,0," ");
		ch_1602[0] = (temp/1000)%10 + 0x30;
		ch_1602[1] = (temp/100)%10 + 0x30;
		ch_1602[2] = (temp/10)%10 + 0x30;
		ch_1602[3] = '.';
		ch_1602[4] = temp%10 + 0x30;
		ch_1602[5] = 0;
		LCD1602_Show_Str(2,0,ch_1602);
		
		temp = (int)SENSOR_DATA.encoder_delta_car/2000;//����������λ��
		if(temp<0)
		{
				LCD1602_Show_Str(1,1,"-");
				temp=-temp;
		}else LCD1602_Show_Str(1,1," ");
		ch_1602[0] = (temp/1000)%10 + 0x30;
		ch_1602[1] = (temp/100)%10 + 0x30;
		ch_1602[2] = (temp/10)%10 + 0x30;
		ch_1602[3] = temp%10 + 0x30;
		ch_1602[4] = 0;
		LCD1602_Show_Str(2,1,ch_1602);
	}
	else
	{
		LCD1602_Write_Cmd(0x01);	//����
		LCD1602_Show_Str(0,0,"*WELCOME TO USE*"); //lcd1602����ʾ
		LCD1602_Show_Str(0,2,"***NENU***AGV***"); //lcd1602����ʾ
	}
}
*/
