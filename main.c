#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exit.h"
#include "usart.h"

#include "lcd.h"


int main(void)
{
	u8 x =0;
	u8 a= 0;
	u8 b= 0;
	u8 *p[] = {"abc"};
	
	u16 point_value = 0;
	HAL_Init();
	Stm32_Clock_Init(RCC_PLL_MUL9);//选择锁相环确认72MHZ
	delay_init(72);//延时初始化这个要弄的
 
	uart_init(115200);					//初始化串口
	Led_Init();
	POINT_COLOR=RED;
	LCD_Init();
	LCD_DrawPoint(0,0);
	LCD_DrawPoint(1,1);
	LCD_DrawPoint(2,2);
	LCD_DrawPoint(3,3);
	LCD_DrawPoint(4,4);
	LCD_DrawPoint(5,5);
	
	LCD_DrawPoint(100,100);

//	LCD_ShowChar(0,0,'A',16,1);
//	LCD_ShowChar(100,100,'B',16,1);

//	a=sizeof(p);
//	b=sizeof(*p);
//	printf("p=%d,*p=%d\r\n",&a,&b);
	point_value = LCD_ReadPoint(0,1);
	printf("value=%#x\r\n",point_value);
	printf("value=a\r\n");
	while(1)
	{
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break;
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(GREEN);break;
			case 11:LCD_Clear(BROWN);break;
		}
		x++;
		if(x==12)
		{
		 x=0;
		}
		LCD_ShowString(30,40,200,16,16,"Mini STM32 ^_^");	
		
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");	
		delay_ms(1000);	
	}
	
	
	
}

