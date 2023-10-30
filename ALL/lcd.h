#ifndef __LCD_H
#define __LCD_H

#include "sys.h"
#include "delay.h"

typedef struct
{
	u16 width;
	u16 height;
	u16 id;
	u8 dir;
	u16 wramcmd;
	u16 setxcom;
	u16 setycom;
}_lcd_dev;

extern u16  POINT_COLOR;//默认红色  

//========LCD端口定义==========
#define LCD_LED PCout(10)     //LCD背光

#define LCD_CS_SET GPIOC->BSRR = 1<<9;  //片选端口  PC9
#define LCD_RS_SET GPIOC->BSRR = 1<<8;  //数据/命令  PC8
#define LCD_WR_SET GPIOC->BSRR = 1<<7;  //写数据    PC7
#define LCD_RD_SET GPIOC->BSRR = 1<<6;  //读数据    PC6

#define LCD_CS_CLR GPIOC->BRR = 1<<9;  //片选端口  PC9  CLK即clear将该位值0
#define LCD_RS_CLR GPIOC->BRR = 1<<8;  //数据/命令  PC8
#define LCD_WR_CLR GPIOC->BRR = 1<<7;  //写数据    PC7
#define LCD_RD_CLR GPIOC->BRR = 1<<6;  //读数据    PC6

//PB0~15,作为数据线
#define DATAOUT(x) GPIOB->ODR = x;
#define DATAIN GPIOB->IDR;

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE           0xFFFF
#define BLACK           0x0000
#define BLUE            0x001F
#define BRED            0XF81F
#define GRED            0XFFE0
#define GBLUE           0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN           0x07E0
#define CYAN            0x7FFF
#define YELLOW          0xFFE0
#define BROWN           0XBC40  //棕色
#define BRRED           0XFC07  //棕红色
#define GRAY            0X8430  //灰色
//GUI颜色

void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void LCD_WR_DATAX(u16 data);
void LCD_WR_REG(u16 data);
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void opt_delay(u8 i);
u16 LCD_RD_DATA(void);
void LCD_Clear(u16 color);
void LCD_Init(void);
void LCD_DrawPoint(u16 x,u16 y);
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

#endif

