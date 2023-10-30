#include "lcd.h"
#include "usart.h"
#include "stdlib.h"
#include "font.h" 


u16 POINT_COLOR=0x0000;
u16 BACK_COLOR=0xffff;

_lcd_dev lcddev;//����һ������


//д������
void LCD_WR_DATAX(u16 data)
{
	LCD_RS_SET;//ѡ������1
	LCD_CS_CLR;//����Ƭѡ
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

//ѡ��Ĵ�����ֵ
void LCD_WR_REG(u16 data)
{
    LCD_RS_CLR;//д��ַ��д��������0
    LCD_CS_CLR;
    DATAOUT(data);
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET; 
}
	
//д�Ĵ���
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATAX(LCD_RegValue);
	
}

//=׼��дGRMd
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
} 
//��������
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
	GPIOB->CRL =0X88888888;//PB0-7ѡ���������뵥�����͸���ѡ��������
	GPIOB->CRH =0X88888888;//PB8-15ѡ����������
	GPIOB->ODR =0X0000;   //ȫ�����0
	
	LCD_RS_SET;     //д����
	LCD_CS_CLR;
	LCD_RD_CLR;
	
	opt_delay(2);//�����ݸ�׼ȷ
	
	t=DATAIN;
  LCD_RD_SET;
  LCD_CS_SET;	
	
  GPIOB->CRL=0X33333333; //PB0-7  ������ٶ�
  GPIOB->CRH=0X33333333; //PB8-15 ������ٶ�
	GPIOB->ODR=0XFFFF;     //ȫ�������
	
	return t;
}

void LCD_Clear(u16 color)
{
    u32 index = 0;
    u32 totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    //�õ��ܵ���

    LCD_SetCursor(0x00, 0x0000);    //���ù��λ��
    LCD_WriteRAM_Prepare();         //��ʼд��GRAM

    for (index = 0; index < totalpoint; index++)
    {
        LCD_WR_DATAX(color);
    }
}



void LCD_Init(void)
{
		u16 regval = 0;
	
	
		 GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOBʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           	//����GPIOCʱ��
		
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
		lcddev.id =LCD_RD_DATA();//�ٶ�
		lcddev.id =LCD_RD_DATA();//0x00
		lcddev.id =LCD_RD_DATA();//0x93
		lcddev.id <<=8;
		lcddev.id |=LCD_RD_DATA();//0x41		
		printf(" LCD ID:%x\r\n", lcddev.id); //��ӡLCD ID
		
		if (lcddev.id == 0X9341)    //9341��ʼ��
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

//    LCD_Display_Dir(0);         //Ĭ��Ϊ����

		lcddev.width = 240;
		lcddev.height = 320;		
		lcddev.wramcmd = 0X2C;
		lcddev.setxcom = 0X2A;
		lcddev.setycom= 0X2B;		
		 regval |= (0 << 7) | (0 << 6) | (0 << 5);
		regval |= 0X08;
		LCD_WriteReg(0x36, regval);
		
    LCD_LED = 1;                //��������
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
    DATAOUT(lcddev.wramcmd);    //дָ��
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET;
    LCD_WR_DATAX(color);         //д����	

}




//void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
//{
//	uint8_t temp,t1,t;
//	u16 y0 = y;
//	
//	u8 csize = (size / 2)*(size / 8 + ((size % 8) ? 1:0));
//	//����һ���ַ���ӦҪռ�ö��ٸ��ֽ�
//	num = num - ' ';
//	
//	if(size == 16) temp=asc2_1608[num][t];
//	else return;
//	for(t = 0;t < csize; t++)//д��һ���ַ���Ҫ�����ֽ�
//	{
//		for(t1 = 0;t1 < 8;t++)
//		{
//		  if(temp & 0x80)//��Ϊһ��ֻ����һ����
//			{
//				 LCD_Fast_DrawPoint(x,y,POINT_COLOR);
//			}
//			else if(mode == 0) 
//			{	
//			LCD_Fast_DrawPoint(x, y, BACK_COLOR);
//			}
//			temp<<=1;//��һ�������
//			y++;//�ı�Y��
//			
//			if (y >= lcddev.height)return;      //��������
//			
//			if((y-y0) == size)//˵������Ի�����һ����
//			{
//				y=y0;//�����Y�ػص�һ��
//			  x++;
//        if (x >= lcddev.width)return;   //��������				
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
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    num = num - ' ';    //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

    for (t = 0; t < csize; t++)
//	  for (t = csize; t >=0; t--)
    {
//        if (size == 12)temp = asc2_1206[num][t];        //����1206����
        if (size == 16)temp = asc2_1608[num][t];   //����1608����
//        else if (size == 24)temp = asc2_2412[num][t];   //����2412����
        else return;                                    //û�е��ֿ�

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)LCD_Fast_DrawPoint(x, y, POINT_COLOR);
            else if (mode == 0)LCD_Fast_DrawPoint(x, y, BACK_COLOR);

            temp <<= 1;
            y++;

//            if (y >= lcddev.height)return;      //��������

            if ((y - y0) == size)
            {
                y = y0;
//                x--;
								x++;
							
//                if (x >= lcddev.width)return;   //��������
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
	while((*p >= ' ')&&(*p <= '~'))//��ӡһ�����ַ���
	{
		if(x>=width)
		{
		  x = x0;
		  y += size;
		}
    if (y >= height)break; //�˳�
		LCD_ShowChar(x, y, *p, size, 0);
		x += size / 2;
		p++;
		
	}

}



//���㺯��
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x, y);        //���ù��λ��
    LCD_WriteRAM_Prepare();     //��ʼд��GRAM
    LCD_WR_DATAX(POINT_COLOR);
}
//���㺯��
u16 LCD_ReadPoint(u16 x,u16 y)
{
    u16 r, g, b;

    if (x >= lcddev.width || y >= lcddev.height)return 0;   //�����˷�Χ,ֱ�ӷ���

    LCD_SetCursor(x, y);


   //����IC(9341/5310/1963/7789)���Ͷ�GRAMָ��
    LCD_WR_REG(0X2E);
    
    r = LCD_RD_DATA();          //�ٶ�

    r = LCD_RD_DATA();          //ʵ��������ɫ���ܶ���RG������ɫ��

    //9341/5310/5510/7789 Ҫ��2�ζ���
    b = LCD_RD_DATA();
    g = r & 0XFF;               //���� 9341/5310/5510/7789, ��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
    g <<= 8;
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  // 9341/5310/5510/7789 ��Ҫ��ʽת��һ�£�����RGB��
}





