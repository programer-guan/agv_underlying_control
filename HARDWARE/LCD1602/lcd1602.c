/*
        1602.c文件
	1602采用四线传输
*/
#include "lcd1602.h"
#include "delay.h" 
#include "sensor.h"
#include "usart.h"

void LCD1602_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);
       //dataIO GPIOF4~7初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);				
        // RS RW E IO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);				
}
 
/* 检测液晶忙不忙*/
void LCD1602_Wait_Ready(void)
{
	int8_t sta;
	
	DATAOUT(0xff);    //PA端口全部置1
	LCD_RS_Clr();     //RS 0
	LCD_RW_Set();     //RW 1
	do
	{
		LCD_EN_Set();   //EN 1
		delay_ms(1);  	//延时5ms  若采用滴答定时器完成延时，按键按下的时刻程序会卡死，具体原因待知!
 		sta = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);//读取状态字
		LCD_EN_Clr();  //EN  0
	}while(sta & 0x80);//bit7=1表示忙，重复检测到其为0停止
}
 
/*向1602写入一字节命令,cmd-待写入命令 */
//写入指令PA4-7 RS-0  RW-0
void LCD1602_Write_Cmd(u8 cmd)
{
	LCD1602_Wait_Ready();  //判断忙不忙
	LCD_RS_Clr();   
	LCD_RW_Clr();
	DATAOUT(cmd);  //先传高四位
	LCD_EN_Set();
	LCD_EN_Clr();
	
	DATAOUT(cmd<<4); //低四位左移到高四位
	LCD_EN_Set();
	LCD_EN_Clr();
}
 
/* 向1602写入一字节数据*/
void LCD1602_Write_Dat(u8 dat)
{
	LCD1602_Wait_Ready(); //判断忙不忙
	LCD_RS_Set();   //1
	LCD_RW_Clr();   //0
	
	DATAOUT(dat);   //先传高四位
	LCD_EN_Set();  
	LCD_EN_Clr();
	
	DATAOUT(dat<<4); //低四位左移到高四位
	LCD_EN_Set(); 
	LCD_EN_Clr();
	
}
 
/*清屏*/
void LCD1602_ClearScreen(void)
{
	LCD1602_Write_Cmd(0x01);
	
}
 
/*设置RAM起始地址,即光标位置,(X,Y)对应屏幕上的字符坐标 */
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

/*在液晶上显示字符串,(X,Y)-对应屏幕上的起始坐标,str-?字符串指针 */
void LCD1602_Show_Str(u8 x, u8 y, u8 *str)
{
	LCD1602_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}
 
/* 初始化液晶*/
void LCD1602_Init(void)
{
	LCD1602_GPIO_Configuration();
	
	LCD1602_Write_Cmd(0x28);	//16*2显示,5*7点阵,4位数据口
	LCD1602_Write_Cmd(0x0C);	//开显示，光标关闭
	LCD1602_Write_Cmd(0x06);	//文字不动，地址自动+1
	LCD1602_Write_Cmd(0x01);	//清屏
	
	LCD1602_Show_Str(0,0,"*WELCOME TO USE*"); //lcd1602上显示
	LCD1602_Show_Str(0,2,"***NENU***AGV***"); //lcd1602上显示
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
	if(mode == 0)									//显示MPU9250数据 
	{
		LCD1602_Write_Cmd(0x01);	//清屏
		LCD1602_Show_Str(0,0,"P"); //lcd1602上显示
		LCD1602_Show_Str(0,1,"R");
		LCD1602_Show_Str(5,0,"Y");
		LCD1602_Show_Str(6,1,"x");
		LCD1602_Show_Str(11,0,"y");
		LCD1602_Show_Str(11,1,"z");
		
		temp=(int)SENSOR_DATA.IMU[0];		//lcd1602显示俯仰角
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

		temp=(int)SENSOR_DATA.IMU[1];		//lcd1602显示横滚角
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
			
		temp=(int)SENSOR_DATA.IMU[2];		//lcd1602显示航向角
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
	
	else if(mode == 1)							//显示电机舵轮方向与编码器数据
	{
		LCD1602_Write_Cmd(0x01);	    //清屏
		LCD1602_Show_Str(0,0,"P");
		LCD1602_Show_Str(0,8,"A");
		LCD1602_Show_Str(0,1,"E");
		
		temp = GET_DIRECT_POS();     //获取当前电机转向的角度
		ch_1602[0] = (temp/1000)%10 + 0x30;
		ch_1602[1] = (temp/100)%10 + 0x30;
		ch_1602[2] = (temp/10)%10 + 0x30;
		ch_1602[3] = temp%10 + 0x30;
		ch_1602[4] = 0;
		LCD1602_Show_Str(9,0,ch_1602);
		
		temp = (int)(SENSOR_DATA.theta*10);//方位角
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
		
		temp = (int)SENSOR_DATA.encoder_delta_car/2000;//两车轮中心位移
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
		LCD1602_Write_Cmd(0x01);	//清屏
		LCD1602_Show_Str(0,0,"*WELCOME TO USE*"); //lcd1602上显示
		LCD1602_Show_Str(0,2,"***NENU***AGV***"); //lcd1602上显示
	}
}
*/
