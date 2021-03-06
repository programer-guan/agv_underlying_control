#include <stm32f10x.h>	   
#include "lcd12864.h"	
#include "delay.h"	

void LCD12864_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	delay_init();	
/*--------------------LCD基本指令-----------------------*/
    delay_ms(2);
    Write_LCD_Command(0x30);  //30--基本指令动作
    delay_ms(2);
    Write_LCD_Command(0x0c);  //光标右移画面不动
    delay_ms(2);
    Write_LCD_Command(0x01);  //清屏
    delay_ms(5);              //清屏时间较长
    Write_LCD_Command(0x06);  //显示开，反白关
    delay_ms(2);

	  Draw_Clear();					// 清屏

}
/*********************************************************
函数名:Send_Byte()
返回值:无
功能:	写数据到LCD
*********************************************************/
void Send_Byte(u8 zdata)
{
  u16 i;
  for(i=0; i<8; i++)
    {
	  if((zdata << i) & 0x80) 
			SID_H;
	  else   
			SID_L;
	  SCLK_H;
		SCLK_L;
	  }
	}
/*********************************************************
函数名:Receive_Byte()
返回值:temp1+temp2
功能:读LCD数据
*********************************************************/
u8 Receive_Byte(void)
	{
	   u8 i,temp1,temp2,value;
	   temp1=0;
	   temp2=0;
	   for(i=0;i<8;i++)
	      {
		  	 temp1=temp1<<1;
		  	 SCLK_L;
			   SCLK_H;
		  	 SCLK_L;
			 if(PAout(7)==1)
			   {
			     temp1++;
				}
		  }
	   for(i=0;i<8;i++)
	      {
		  	 temp2=temp2<<1;
		  	 SCLK_L;
			 SCLK_H;
		  	 SCLK_L;
			 if(PAout(7)==1)
			   {
			      temp2++;
			   }
		  }
	    temp1=0xf0&temp1;
		  temp2=0x0f&temp2;
		  value=temp1+temp2;
      return   value ;
	}
/*********************************************************
函数名:Check_Busy()
返回值:无
功能:	LCD忙检查
*********************************************************/
void Check_Busy(void)
{
	do
		Send_Byte(0xfc);		  //11111,RW(1),RS(0),0
	while(0x80&Receive_Byte());
}
/*********************************************************
函数名: Write_LCD_Command()
返回值:无
功能:	写命令到LCD
*********************************************************/
void Write_LCD_Command(u8 cmdcode)
{ 
   
   CS_H;
   //Check_Busy();
   Send_Byte(0xf8);
   Send_Byte(cmdcode & 0xf0);
   Send_Byte((cmdcode << 4) & 0xf0);
   delay_ms(2);
   CS_L;
}
/*********************************************************
函数名: Write_LCD_Data()
返回值:无
功能:	写显示内容到LCD
*********************************************************/
void Write_LCD_Data(u8 Dispdata)
{  
  CS_H;
  //Check_Busy();     //如果判忙，中断会卡死
  Send_Byte(0xfa);	  //11111,RW(0),RS(1),0
  Send_Byte(Dispdata & 0xf0);
  Send_Byte((Dispdata << 4) & 0xf0);
  delay_ms(2);
  CS_L;
}
/*********************************************************
函数名: Read_LCD_Data()
返回值:LCD收到的数据
功能:	读取LCD显示的内容
*********************************************************/
u8 Read_Data(void)
	{
	  Check_Busy();
	  Send_Byte(0xfe);    //11111,RW(1),RS(1),0 LCD->MCU
	  return  Receive_Byte();
	
	}
/*********************************************************
函数名: LCD_Clear_Txt
返回值:无
功能:	文本区清除
*********************************************************/
void LCD_Clear_TXT( void )
{
     u8 i;
     Write_LCD_Command(0x30);      //8BitMCU,基本指令集合
     Write_LCD_Command(0x80);      //AC归起始位
     for(i=0;i<64;i++)
	  {
       Write_LCD_Data(0x20);
	  }
}
/*********************************************************
函数名: LCD_Clear_BMP
返回值:无
功能:图片区清除
*********************************************************/
void LCD_Clear_BMP( void )
{
     u8 i,j;
     Write_LCD_Command(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     Write_LCD_Command(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x80);      //列位置
           for(j=0;j<32;j++)            //256/8=32 byte
                Write_LCD_Data(0);
     }
}
/*********************************************************
函数名: Display_LCD_Pos
返回值:无
功能:设置显示位置
*********************************************************/
void Display_LCD_Pos(u8 x,u8 y) 
	{
	   u8 pos;
	   switch(x)
	       {
		   	  case 0: x=0x80;break;
			  case 1: x=0x90;break;
		   	  case 2: x=0x88;break;
			  case 3: x=0x98;break;
		   }
	   pos=x+y;
	   Write_LCD_Command(pos);
    }

/*********************************************************
函数名:Display_LCD_String()
返回值:无
功能:显示字符串
*********************************************************/

void Display_LCD_String(u8 *s)
{
    while(*s != '\0')
    {
        Write_LCD_Data(*s);
        s++;
        delay_ms(2);               //控制每一个字符之间显示的时间 间隔
    }
}
/*************************************************************
* 函数名称   :PutBMP
**************************************************************/
void PutBMP(const u8 *puts)
{
     u16 x=0;
     u8 i,j;
		 Write_LCD_Command(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     Write_LCD_Command(0x36);      //绘图ON,基本指令集里面36H不能开绘?
     for(i=0;i<32;i++)             //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x80);       //列位置
           for(j=0;j<16;j++)      //256/8=32 byte
           {                      //列位置每行自动增加
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x88);        //列位置
           for(j=0;j<16;j++)      //256/8=32 byte
           {                      //列位置每行自动增加
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
}
void PHO_Display(const u8 *s) 
{ 
	u8 COUNT3 = 0X02,COUNT1,COUNT2,LCD_X = 0X80,LCD_Y;
		 
	for (;COUNT3 != 0;COUNT3 --) 
	{ 
		LCD_Y = 0X80; 
		COUNT2 = 0X20;
		for(;COUNT2 != 0;COUNT2 --) 
		{ 
			//COUNT1 = 0X10;//全屏
			COUNT1 = 0X08;  //半屏
			Write_LCD_Command(0x34); 
			Write_LCD_Command(LCD_Y); 
			Write_LCD_Command(LCD_X); 
			Write_LCD_Command(0x30); 
			for (;COUNT1 != 0;COUNT1 --) 
			{ 
				Write_LCD_Data(*s ++); 		
			} 
			LCD_Y += 1; 
		} 
		LCD_X = 0X88; 
	} 
	Write_LCD_Command(0x36); 
	Write_LCD_Command(0x30); 
}
/*-------------------------------------------------
 LCD显示缓存
--------------------------------------------------*/
u16 LCD12864_Buffer[64][16] = {0};

const u8 m_ch4x5[][6] = 
{
	{0x06,0x09,0x09,0x09,0x06,5},		// 0,字符宽度5点阵
	{0x02,0x06,0x02,0x02,0x07,5},		// 1
	{0x06,0x09,0x02,0x04,0x0F,5},		// 2
	{0x06,0x09,0x02,0x09,0x06,5},		// 3
	{0x02,0x06,0x0A,0x0F,0x02,5},		// 4
	{0x0E,0x08,0x0E,0x01,0x0E,5},		// 5
	{0x06,0x08,0x0E,0x09,0x06,5},		// 6
	{0x0F,0x01,0x02,0x04,0x04,5},		// 7
	{0x06,0x09,0x06,0x09,0x06,5},		// 8
	{0x06,0x09,0x07,0x01,0x06,5},		// 9
	{0x00,0x00,0x00,0x00,0x01,2},		// .
	{0x00,0x01,0x00,0x01,0x00,2},		// :
	{0x00,0x00,0x00,0x00,0x00,5}, 	// 空格






};
/*------------------------------------------------------------------------------
功能:画点
参数:
	x:横坐标
	y:纵坐标
	color:颜色,0不显示,1显示
------------------------------------------------------------------------------*/
const u16 DrawDotsTable[] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
void Draw_Dots(u8 x, u8 y, u8 color)
{
	u8 i0, y0;
	u16 value;

	//获得坐标
	if ( y >= 32 )
	{
		y0 = y-32;
		i0 = x/16+8;
	}
	else
	{
		y0 = y;
		i0 = x/16;
	}
	value = DrawDotsTable[x%16];
	if (color == 0)
	{
		LCD12864_Buffer[y0][i0] &= ~value;
	}
	else
	{
		LCD12864_Buffer[y0][i0] |= value;
	}
}

/*------------------------------------------------------------------------------
功能:刷新显示
------------------------------------------------------------------------------*/
void Draw_Update(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		Write_LCD_Command(0x80|y);
		Write_LCD_Command(0x80);
		for (x=0; x<16; x++)
		{
			Write_LCD_Data(LCD12864_Buffer[y][x]>>8);
			Write_LCD_Data(LCD12864_Buffer[y][x]);
		}
	}
}

/*------------------------------------------------------------------------------
功能:显示清屏
------------------------------------------------------------------------------*/
void Draw_Clear(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		for (x=0; x<16; x++)
		{
			LCD12864_Buffer[y][x] = 0;
		}
	}
}

/*------------------------------------------------------------------------------
功能:显示4*5字符
参数:
	x:写出横坐标
	y:写出纵坐标
	value:字符
------------------------------------------------------------------------------*/
void Draw_Char4x5(u8 x, u8 y, u8 value)
{
	u8 i, j, p, z, a;
	
	a = m_ch4x5[value][5];	
	for (i=0; i<5; i++)
	{
		p = 0x10>>(5-a);
		for (j=0; j<a; j ++)
		{
			z = m_ch4x5[value][i];
			if ( z & p )
			{
				Draw_Dots(j+x, i+y, 1);
			}
			p >>= 1;
		}
	}
}

/*--------------------------------------------------------
功能:显示一行4*5字符
参数:
	x:写出横坐标
	y:写出纵坐标
	value:字符串
	len:长度
---------------------------------------------------------*/
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len)
{
	u8 i;

	for (i=0; i<len; i++)
	{
		Draw_Char4x5(x, y, value[i]);
		x += m_ch4x5[value[i]][5];
	}
}
// 向x,y输出字符
void Dis_string(u8 x,u8 y,u8 *string)
{
	Display_LCD_Pos(x,y);
	Display_LCD_String(string);
}
// 输出数字
void dis_int(u16 num)                             
{
	u8 temp[17];
	u8 str[17];
	int i=0,j=0;
	if(num==0) Display_LCD_String("0");
	while(num != 0)	                                     
	                                                   
	{
		temp[i] = (num%10)+0x30;
		num/=10;                                           
		i++;
	}
	i--;                                          
	                                                   
	while(i != -1)	                                      
	{
		str[j] = temp[i];
		j++;
		i--;	
	}
	str[j]='\0';                                          
	                                                     
	Display_LCD_String(str);	
}
//向X Y位置输出数字
void Dis_int(u8 x,u8 y,u16 num)
{
	Display_LCD_Pos(x,y);
	dis_int(num);
}
