#ifndef __ST7789V_H__
#define __ST7789V_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"

/* define---------------------------------------------------------------------*/
#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 300

#else
#define LCD_W 300
#define LCD_H 240
#endif

//#define LCD_SCLK_Clr() HAL_GPIO_WritePin(lcd18_lck_GPIO_Port,lcd18_lck_Pin,GPIO_PIN_RESET)//SCL=SCLK
//#define LCD_SCLK_Set() HAL_GPIO_WritePin(lcd18_lck_GPIO_Port,lcd18_lck_Pin,GPIO_PIN_SET)

//#define LCD_MOSI_Clr() HAL_GPIO_WritePin(lcd18_mosi_GPIO_Port,lcd18_mosi_Pin,GPIO_PIN_RESET)//SDA=MOSI
//#define LCD_MOSI_Set() HAL_GPIO_WritePin(lcd18_mosi_GPIO_Port,lcd18_mosi_Pin,GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_RESET)
#define LCD_RES_Set()  HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(lcd18_dc_GPIO_Port,lcd18_dc_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(lcd18_dc_GPIO_Port,lcd18_dc_Pin,GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_SET)

#define HARDWARE	1
#if HARDWARE
	#define LCD_CS_Clr()   
	#define LCD_CS_Set()   
#else
	#define LCD_CS_Clr()   HAL_GPIO_WritePin(spi1_cs_GPIO_Port,spi1_cs_Pin,GPIO_PIN_RESET)//CS
	#define LCD_CS_Set()   HAL_GPIO_WritePin(spi1_cs_GPIO_Port,spi1_cs_Pin,GPIO_PIN_SET)	
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


/* extern variables-----------------------------------------------------------*/

/* exported functions ------------------------------------------------------- */

void ST7789V_Write16bit(uint16_t data);
void ST7789V_WR_REG(uint8_t dat);//写入一个指令
void LCD_WriteData256(uint16_t *pData ,uint16_t size);
void ST7789V_Init(void);//LCD初始化
void ST7789V_AddressSet(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ST7789V_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend ,uint16_t yend, uint16_t color);


#ifdef __cplusplus
}
#endif

#endif