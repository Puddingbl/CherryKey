
#include "ST7789V.h"
//#include "lcdfont.h"
#include "public.h"
#include "spi.h"

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void ST7789V_Write8bit(uint8_t data) 
{	
	uint8_t pData =data;
	LCD_CS_Clr();
  HAL_SPI_Transmit(&hspi1, &pData, 1, 100);
	
//	LL_SPI_TransmitData8(SPI1, pData);
//	while(!LL_SPI_IsActiveFlag_TXP(SPI1));
	
//	HAL_SPI_Transmit_DMA(&hspi1, &pData, 1);
	LCD_CS_Set();
}

void ST7789V_Write16bit(uint16_t data) 
{	
	ST7789V_Write8bit(data>>8);
	ST7789V_Write8bit(data);
}

/**
	*一次写入最多256个16位数据
*/
void LCD_WriteData256(uint16_t *pData ,uint16_t size)
{
//	if(size <= 0 || size >256)
//	{
//		return;
//	}

	uint8_t data[size*2];
	
	LCD_CS_Clr();
	for(uint16_t i=0; i<size; i+=1)
	{
//		SW16(pData[i]);	
		data[2*i] = pData[i]>>8;
		data[2*i+1] = pData[i]&0xff;
//		printf("data[%d]=%d, data[%d]=%d\n",2*i,data[2*i], 2*i+1,data[2*i+1]);
	}		
	
//	SCB_CleanInvalidateDCache();	
//	HAL_SPI_Transmit_DMA(&hspi1, data, size*2);

	HAL_SPI_Transmit(&hspi1, data, size*2, 100);	
	LCD_CS_Set();		
}

/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void ST7789V_WR_REG(uint8_t command)
{
	LCD_DC_Clr();//写命令
	ST7789V_Write8bit(command);
	LCD_DC_Set();//写数据
}

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void ST7789V_AddressSet(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		ST7789V_WR_REG(0x2a);//列地址设置
		ST7789V_Write16bit(x1);
		ST7789V_Write16bit(x2);
		ST7789V_WR_REG(0x2b);//行地址设置
		ST7789V_Write16bit(y1);
		ST7789V_Write16bit(y2);
		ST7789V_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		ST7789V_WR_REG(0x2a);//列地址设置
		ST7789V_Write16bit(x1);
		ST7789V_Write16bit(x2);
		ST7789V_WR_REG(0x2b);//行地址设置
		ST7789V_Write16bit(y1);
		ST7789V_Write16bit(y2);
		ST7789V_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		ST7789V_WR_REG(0x2a);//列地址设置
		ST7789V_Write16bit(x1);
		ST7789V_Write16bit(x2);
		ST7789V_WR_REG(0x2b);//行地址设置
		ST7789V_Write16bit(y1);
		ST7789V_Write16bit(y2);
		ST7789V_WR_REG(0x2c);//储存器写
	}
	else
	{
		ST7789V_WR_REG(0x2a);//列地址设置
		ST7789V_Write16bit(x1);
		ST7789V_Write16bit(x2);
		ST7789V_WR_REG(0x2b);//行地址设置
		ST7789V_Write16bit(y1);
		ST7789V_Write16bit(y2);
		ST7789V_WR_REG(0x2c);//储存器写
	}
}

void ST7789V_Init(void)
{
//	LCD_GPIO_Init();//初始化GPIO
	
	LCD_RES_Clr();//复位
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
//	
//	LCD_BLK_Set();//打开背光
//  HAL_Delay(100);
	
	//************* Start Initial Sequence **********//
	ST7789V_WR_REG(0x11); //Sleep out 
	HAL_Delay(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	ST7789V_WR_REG(0x36);
	if(USE_HORIZONTAL==0)ST7789V_Write8bit(0x00);
	else if(USE_HORIZONTAL==1)ST7789V_Write8bit(0xC0);
	else if(USE_HORIZONTAL==2)ST7789V_Write8bit(0x70);
	else ST7789V_Write8bit(0xA0);

	ST7789V_WR_REG(0x3A);			
	ST7789V_Write8bit(0x05);

	ST7789V_WR_REG(0xB2);			
	ST7789V_Write8bit(0x0C);
	ST7789V_Write8bit(0x0C); 
	ST7789V_Write8bit(0x00); 
	ST7789V_Write8bit(0x33); 
	ST7789V_Write8bit(0x33); 			

	ST7789V_WR_REG(0xB7);			
	ST7789V_Write8bit(0x35);

	ST7789V_WR_REG(0xBB);			
	ST7789V_Write8bit(0x32); //Vcom=1.35V
					
	ST7789V_WR_REG(0xC2);
	ST7789V_Write8bit(0x01);

	ST7789V_WR_REG(0xC3);			
	ST7789V_Write8bit(0x15); //GVDD=4.8V  颜色深度
				
	ST7789V_WR_REG(0xC4);			
	ST7789V_Write8bit(0x20); //VDV, 0x20:0v

	ST7789V_WR_REG(0xC6);			
	ST7789V_Write8bit(0x0F); //0x0F:60Hz        	

	ST7789V_WR_REG(0xD0);			
	ST7789V_Write8bit(0xA4);
	ST7789V_Write8bit(0xA1); 

	ST7789V_WR_REG(0xE0);
	ST7789V_Write8bit(0xD0);   
	ST7789V_Write8bit(0x08);   
	ST7789V_Write8bit(0x0E);   
	ST7789V_Write8bit(0x09);   
	ST7789V_Write8bit(0x09);   
	ST7789V_Write8bit(0x05);   
	ST7789V_Write8bit(0x31);   
	ST7789V_Write8bit(0x33);   
	ST7789V_Write8bit(0x48);   
	ST7789V_Write8bit(0x17);   
	ST7789V_Write8bit(0x14);   
	ST7789V_Write8bit(0x15);   
	ST7789V_Write8bit(0x31);   
	ST7789V_Write8bit(0x34);   

	ST7789V_WR_REG(0xE1);     
	ST7789V_Write8bit(0xD0);   
	ST7789V_Write8bit(0x08);   
	ST7789V_Write8bit(0x0E);   
	ST7789V_Write8bit(0x09);   
	ST7789V_Write8bit(0x09);   
	ST7789V_Write8bit(0x15);   
	ST7789V_Write8bit(0x31);   
	ST7789V_Write8bit(0x33);   
	ST7789V_Write8bit(0x48);   
	ST7789V_Write8bit(0x17);   
	ST7789V_Write8bit(0x14);   
	ST7789V_Write8bit(0x15);   
	ST7789V_Write8bit(0x31);   
	ST7789V_Write8bit(0x34);
	ST7789V_WR_REG(0x21); 

	ST7789V_WR_REG(0x29);
} 

void ST7789V_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend ,uint16_t yend, uint16_t color)
{          
	uint16_t i,j; 
	
	ST7789V_AddressSet(xsta,ysta,xend-1,yend-1);//设置显示范围
	
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			ST7789V_Write16bit(color);			
		}
	}	
}