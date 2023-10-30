#include "lcd.h"
#include "usart.h"
#include "stdlib.h"
#include "font.h" 


u16 POINT_COLOR=0x0000;
u16 BACK_COLOR=0xffff;

_lcd_dev lcddev;//定义一个类型


//写入数据
void LCD_WR_DATAX(u16 data)
{
	LCD_RS_SET;//选择数据1
	LCD_CS_CLR;//开启片选
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

//选择寄存器的值
void LCD_WR_REG(u16 data)
{
    LCD_RS_CLR;//写地址，写的是命令0
    LCD_CS_CLR;
    DATAOUT(data);
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET; 
}
	
//写寄存器
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATAX(LCD_RegValue);
	
}

//=准备写GRMd
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
} 
//设置坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_WR_REG(lcddev.setxcom );
	LCD_WR_DATAX(Xpos>>8);
	LCD_WR_DATAX(Xpos & 0XFF);
  LCD_WR_REG(lcddev.setycom );
	LCD_WR_DATAX(Ypos>>8);
	LCD_WR_DATAX(Ypos & 0XFF);	
	
	
}

void opt_delay(u8 i)
{
    while (i--);
}


u16 LCD_RD_DATA(void)
{
	u16 t;
	GPIOB->CRL =0X88888888;//PB0-7选择上拉输入单独，就给你选择输入了
	GPIOB->CRH =0X88888888;//PB8-15选择上拉输入
	GPIOB->ODR =0X0000;   //全部输出0
	
	LCD_RS_SET;     //写数据
	LCD_CS_CLR;
	LCD_RD_CLR;
	
	opt_delay(2);//让数据更准确
	
	t=DATAIN;
  LCD_RD_SET;
  LCD_CS_SET;	
	
  GPIOB->CRL=0X33333333; //PB0-7  输出的速度
  GPIOB->CRH=0X33333333; //PB8-15 输出的速度
	GPIOB->ODR=0XFFFF;     //全部输出高
	
	return t;
}

void LCD_Clear(u16 color)
{
    u32 index = 0;
    u32 totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    //得到总点数

    LCD_SetCursor(0x00, 0x0000);    //设置光标位置
    LCD_WriteRAM_Prepare();         //开始写入GRAM

    for (index = 0; index < totalpoint; index++)
    {
        LCD_WR_DATAX(color);
    }
}



void LCD_Init(void)
{
		u16 regval = 0;
	
	
		 GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOC时钟
		
		GPIO_Initure.Pin =GPIO_PIN_6 |GPIO_PIN_7 |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Speed =GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);

		GPIO_Initure.Pin =GPIO_PIN_All;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		__HAL_AFIO_REMAP_SWJ_DISABLE();
		
		delay_ms(50);
		LCD_WriteReg(0x0000,0x0001);
		delay_ms(50);

		LCD_WR_REG(0xD3);
		lcddev.id =LCD_RD_DATA();//假读
		lcddev.id =LCD_RD_DATA();//0x00
		lcddev.id =LCD_RD_DATA();//0x93
		lcddev.id <<=8;
		lcddev.id |=LCD_RD_DATA();//0x41		
		printf(" LCD ID:%x\r\n", lcddev.id); //打印LCD ID
		
		if (lcddev.id == 0X9341)    //9341初始化
    {
        LCD_WR_REG(0xCF);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0xC1);
        LCD_WR_DATAX(0X30);
        LCD_WR_REG(0xED);
        LCD_WR_DATAX(0x64);
        LCD_WR_DATAX(0x03);
        LCD_WR_DATAX(0X12);
        LCD_WR_DATAX(0X81);
        LCD_WR_REG(0xE8);
        LCD_WR_DATAX(0x85);
        LCD_WR_DATAX(0x10);
        LCD_WR_DATAX(0x7A);
        LCD_WR_REG(0xCB);
        LCD_WR_DATAX(0x39);
        LCD_WR_DATAX(0x2C);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x34);
        LCD_WR_DATAX(0x02);
        LCD_WR_REG(0xF7);
        LCD_WR_DATAX(0x20);
        LCD_WR_REG(0xEA);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_REG(0xC0);       //Power control
        LCD_WR_DATAX(0x1B);     //VRH[5:0]
        LCD_WR_REG(0xC1);       //Power control
        LCD_WR_DATAX(0x01);     //SAP[2:0];BT[3:0]
        LCD_WR_REG(0xC5);       //VCM control
        LCD_WR_DATAX(0x30);     //3F
        LCD_WR_DATAX(0x30);     //3C
        LCD_WR_REG(0xC7);       //VCM control2
        LCD_WR_DATAX(0XB7);
        LCD_WR_REG(0x36);       // Memory Access Control
        LCD_WR_DATAX(0x48);
        LCD_WR_REG(0x3A);
        LCD_WR_DATAX(0x55);
        LCD_WR_REG(0xB1);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x1A);
        LCD_WR_REG(0xB6);       // Display Function Control
        LCD_WR_DATAX(0x0A);
        LCD_WR_DATAX(0xA2);
        LCD_WR_REG(0xF2);       // 3Gamma Function Disable
        LCD_WR_DATAX(0x00);
        LCD_WR_REG(0x26);       //Gamma curve selected
        LCD_WR_DATAX(0x01);
        LCD_WR_REG(0xE0);       //Set Gamma
        LCD_WR_DATAX(0x0F);
        LCD_WR_DATAX(0x2A);
        LCD_WR_DATAX(0x28);
        LCD_WR_DATAX(0x08);
        LCD_WR_DATAX(0x0E);
        LCD_WR_DATAX(0x08);
        LCD_WR_DATAX(0x54);
        LCD_WR_DATAX(0XA9);
        LCD_WR_DATAX(0x43);
        LCD_WR_DATAX(0x0A);
        LCD_WR_DATAX(0x0F);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_REG(0XE1);       //Set Gamma
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x15);
        LCD_WR_DATAX(0x17);
        LCD_WR_DATAX(0x07);
        LCD_WR_DATAX(0x11);
        LCD_WR_DATAX(0x06);
        LCD_WR_DATAX(0x2B);
        LCD_WR_DATAX(0x56);
        LCD_WR_DATAX(0x3C);
        LCD_WR_DATAX(0x05);
        LCD_WR_DATAX(0x10);
        LCD_WR_DATAX(0x0F);
        LCD_WR_DATAX(0x3F);
        LCD_WR_DATAX(0x3F);
        LCD_WR_DATAX(0x0F);
        LCD_WR_REG(0x2B);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x01);
        LCD_WR_DATAX(0x3f);
        LCD_WR_REG(0x2A);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0x00);
        LCD_WR_DATAX(0xef);
        LCD_WR_REG(0x11);       //Exit Sleep
        delay_ms(120);
        LCD_WR_REG(0x29);       //display on
    }

//    LCD_Display_Dir(0);         //默认为竖屏

		lcddev.width = 240;
		lcddev.height = 320;		
		lcddev.wramcmd = 0X2C;
		lcddev.setxcom = 0X2A;
		lcddev.setycom= 0X2B;		
		 regval |= (0 << 7) | (0 << 6) | (0 << 5);
		regval |= 0X08;
		LCD_WriteReg(0x36, regval);
		
    LCD_LED = 1;                //点亮背光
    LCD_Clear(WHITE);

}
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{
		LCD_WR_REG(lcddev.setxcom);
		LCD_WR_DATAX(x >> 8);
		LCD_WR_DATAX(x & 0XFF);
		LCD_WR_REG(lcddev.setycom);
		LCD_WR_DATAX(y >> 8);
		LCD_WR_DATAX(y & 0XFF);	
    LCD_RS_CLR;
    LCD_CS_CLR;
    DATAOUT(lcddev.wramcmd);    //写指令
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET;
    LCD_WR_DATAX(color);         //写数据	

}




//void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
//{
//	uint8_t temp,t1,t;
//	u16 y0 = y;
//	
//	u8 csize = (size / 2)*(size / 8 + ((size % 8) ? 1:0));
//	//计算一个字符对应要占用多少个字节
//	num = num - ' ';
//	
//	if(size == 16) temp=asc2_1608[num][t];
//	else return;
//	for(t = 0;t < csize; t++)//写出一个字符需要几个字节
//	{
//		for(t1 = 0;t1 < 8;t++)
//		{
//		  if(temp & 0x80)//因为一次只能描一个点
//			{
//				 LCD_Fast_DrawPoint(x,y,POINT_COLOR);
//			}
//			else if(mode == 0) 
//			{	
//			LCD_Fast_DrawPoint(x, y, BACK_COLOR);
//			}
//			temp<<=1;//下一个点描绘
//			y++;//改变Y列
//			
//			if (y >= lcddev.height)return;      //超区域了
//			
//			if((y-y0) == size)//说明你可以换到下一列了
//			{
//				y=y0;//最后让Y重回第一行
//			  x++;
//        if (x >= lcddev.width)return;   //超区域了				
//				
//				break;
//			}
//		}
//	}

//}	

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{
	  
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);  //得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';    //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    for (t = 0; t < csize; t++)
//	  for (t = csize; t >=0; t--)
    {
//        if (size == 12)temp = asc2_1206[num][t];        //调用1206字体
        if (size == 16)temp = asc2_1608[num][t];   //调用1608字体
//        else if (size == 24)temp = asc2_2412[num][t];   //调用2412字体
        else return;                                    //没有的字库

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)LCD_Fast_DrawPoint(x, y, POINT_COLOR);
            else if (mode == 0)LCD_Fast_DrawPoint(x, y, BACK_COLOR);

            temp <<= 1;
            y++;

//            if (y >= lcddev.height)return;      //超区域了

            if ((y - y0) == size)
            {
                y = y0;
//                x--;
								x++;
							
//                if (x >= lcddev.width)return;   //超区域了
                break;
            }
        }
    } 
}

void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{
  u8 x0 = x;
	width +=x;
  height +=y;
	while((*p >= ' ')&&(*p <= '~'))//打印一整个字符串
	{
		if(x>=width)
		{
		  x = x0;
		  y += size;
		}
    if (y >= height)break; //退出
		LCD_ShowChar(x, y, *p, size, 0);
		x += size / 2;
		p++;
		
	}

}



//画点函数
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x, y);        //设置光标位置
    LCD_WriteRAM_Prepare();     //开始写入GRAM
    LCD_WR_DATAX(POINT_COLOR);
}
//读点函数
u16 LCD_ReadPoint(u16 x,u16 y)
{
    u16 r, g, b;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   //超过了范围,直接返回

    LCD_SetCursor(x, y);


   //其他IC(9341/5310/1963/7789)发送读GRAM指令
    LCD_WR_REG(0X2E);
    
    r = LCD_RD_DATA();          //假读

    r = LCD_RD_DATA();          //实际坐标颜色（能读出RG两种颜色）

    //9341/5310/5510/7789 要分2次读出
    b = LCD_RD_DATA();
    g = r & 0XFF;               //对于 9341/5310/5510/7789, 第一次读取的是RG的值,R在前,G在后,各占8位
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  // 9341/5310/5510/7789 需要公式转换一下（整合RGB）
}





