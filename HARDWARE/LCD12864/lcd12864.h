/*************************************************
函数名:lcd12864.h

功能 :驱动12864液晶(带字库,串行方式)

时间:2013/06/27
*************************************************/
#ifndef __LCD12864_H
#define __LCD12864_H	 
#include "sys.h"

//如果要修改IO口,记得修改.c文件里对IO口的初始化

#define CS_H   PGout(10)=1    //RS
#define CS_L   PGout(10)=0
#define SID_H  PGout(11)=1    //R/W 
#define SID_L  PGout(11)=0
#define SCLK_H PGout(12)=1    //E 
#define SCLK_L PGout(12)=0


void LCD12864_Init(void);
void Send_Byte(u8 zdata);
u8 Receive_Byte(void);
void Check_Busy(void);
void Write_LCD_Command(u8 cmdcode);
void Write_LCD_Data(u8 Dispdata);
u8 Read_Data(void);
void LCD_Clear_TXT( void );
void LCD_Clear_BMP( void );
void Display_LCD_Pos(u8 x,u8 y) ;
void Draw_dots(u8 x,u8 y);
void Display_LCD_String(u8 *p);	 
void PutBMP(const u8 *puts);
void Draw_Dots(u8 x, u8 y, u8 color);
void Draw_Update(void);
void Draw_Clear(void);
void Draw_Char4x5(u8 x, u8 y, u8 value);
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len);

//????
void dis_int(u16 num);
void Dis_string(u8 x,u8 y,u8 *string);   //先初始化。通常只需用到以下3个函数
void PHO_Display(const u8 *s);
void Dis_int(u8 x,u8 y,u16 num);
#endif
