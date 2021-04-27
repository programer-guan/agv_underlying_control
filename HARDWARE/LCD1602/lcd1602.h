#ifndef __LCD1602_H
#define __LCD1602_H	  
#include "stm32f10x.h"
#define LCD_DATA_PORT GPIOG
 
//1602指令/数据/引脚
#define	LCD_RS_Set()	GPIO_SetBits(GPIOF, GPIO_Pin_0)
#define	LCD_RS_Clr()	GPIO_ResetBits(GPIOF, GPIO_Pin_0)
 
//1602读写引脚
#define	LCD_RW_Set()	GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define	LCD_RW_Clr()	GPIO_ResetBits(GPIOF, GPIO_Pin_1)
 
//1602使能引脚
#define	LCD_EN_Set()	GPIO_SetBits(GPIOF, GPIO_Pin_2)
#define	LCD_EN_Clr()	GPIO_ResetBits(GPIOF, GPIO_Pin_2)
 
//1602数据端口	PG4~7
#define	DATAOUT(x)	GPIO_Write(LCD_DATA_PORT, x)
 
#define u8  unsigned char 
 
void GPIO_Configuration(void);
void LCD1602_Wait_Ready(void);
void LCD1602_Write_Cmd(u8 cmd);
void LCD1602_Write_Dat(u8 dat);
void LCD1602_ClearScreen(void);
void LCD1602_Set_Cursor(u8 x, u8 y);
void LCD1602_Show_Str(u8 x, u8 y, u8 *str);
void LCD1602_Init(void);
void LCD1602_Refresh(u8 mode);

void LCD1602_clear(void);
void LCD1602_Show_char(u8 x, u8 y, u8 c);

#endif

