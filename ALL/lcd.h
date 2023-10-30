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

extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ  

//========LCD�˿ڶ���==========
#define LCD_LED PCout(10)     //LCD����

#define LCD_CS_SET GPIOC->BSRR = 1<<9;  //Ƭѡ�˿�  PC9
#define LCD_RS_SET GPIOC->BSRR = 1<<8;  //����/����  PC8
#define LCD_WR_SET GPIOC->BSRR = 1<<7;  //д����    PC7
#define LCD_RD_SET GPIOC->BSRR = 1<<6;  //������    PC6

#define LCD_CS_CLR GPIOC->BRR = 1<<9;  //Ƭѡ�˿�  PC9  CLK��clear����λֵ0
#define LCD_RS_CLR GPIOC->BRR = 1<<8;  //����/����  PC8
#define LCD_WR_CLR GPIOC->BRR = 1<<7;  //д����    PC7
#define LCD_RD_CLR GPIOC->BRR = 1<<6;  //������    PC6

//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR = x;
#define DATAIN GPIOB->IDR;

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
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
#define BROWN           0XBC40  //��ɫ
#define BRRED           0XFC07  //�غ�ɫ
#define GRAY            0X8430  //��ɫ
//GUI��ɫ

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

