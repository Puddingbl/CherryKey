#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__

//头文件
#include "main.h"

////宏定义
//TFT屏幕背光控制
#define	TFT_LCD_BL_ON	  HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_SET)
#define	TFT_LCD_BL_OFF	HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_RESET)

/******************************************************************************
NOR/PSRAM存储区地址：
64MB:FSMC_Bank1_NORSRAM1:0x6000 0000 ~ 0x63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0x6400 0000 ~ 0x67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0x6800 0000 ~ 0x6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0x6C00 0000 ~ 0x6FFF FFFF

选择BANK1-BORSRAM1 连接 TFT，地址范围为0x6000 0000 ~ 0x63FF FFFF
STM32物联网实战项目开发板选择 FSMC_A0 接LCD的D/CX(命令/数据选择)脚
命令地址 = 0x6000 0000
数据地址 = 0x6000 0002 = 0x6000 0000+(1<<(0+1))
如果电路设计时选择不同的地址线时，地址要重新计算        
eg:选择A10，则数据基地址 = 0x6000 0000+(1<<(10+1)) = 0x6000 0800
*******************************************************************************/
#define	FSMC_LCD_CMD_ADDR		((uint32_t)0x60000000)  //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define	FSMC_LCD_DATA_ADDR  ((uint32_t)0x60100000)  //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址  

//LCD读写函数宏定义
#define	LCD_Write_CMD(CMD) 	  *((__IO uint16_t *)FSMC_LCD_CMD_ADDR)  = (uint16_t)CMD
#define	LCD_Write_DATA(DATA)  *((__IO uint16_t *)FSMC_LCD_DATA_ADDR) = (uint16_t)DATA  
#define	LCD_Read_DATA()       *((__IO uint16_t *)FSMC_LCD_DATA_ADDR)

//显示方向选择，可选(1,2,3,4)四个方向
#define LCD_DIRECTION                  1  // 竖屏,逆时针旋转0度， 原点在屏幕左上角 X*Y=240*320
//#define LCD_DIRECTION                  2  // 横屏,逆时针旋转90度, 原点在屏幕右上角 X*Y=320*240
//#define LCD_DIRECTION                  3  // 竖屏,逆时针旋转180度,原点在屏幕右下角 X*Y=240*320
//#define LCD_DIRECTION                  4  // 横屏,逆时针旋转270度,原点在屏幕左下角 X*Y=320*240

//LCD屏幕的宽度与高度
#if (LCD_DIRECTION == 1)||(LCD_DIRECTION == 3)
 #define LCD_WIDTH		         240  // X轴长度
 #define LCD_HEIGTH         	 300  // Y轴长度
#else
 #define LCD_WIDTH		         320  // X轴长度
 #define LCD_HEIGTH         	 240  // Y轴长度 
#endif

//LCD命令
#define LCD_CMD_SETxOrgin		0x2A
#define LCD_CMD_SETyOrgin   0x2B
#define LCD_CMD_WRgram      0x2C
#define LCD_CMD_RDgram      0x2E

////定义枚举类型
//颜色定义
typedef enum
{
	Color_BLACK     = 0x0000,	 //黑色
	Color_WHITE     = 0xFFFF,	 //白色
  Color_RED       = 0xF800,	 //红色
  Color_GREEN     = 0x07E0,	 //绿色 
  Color_BLUE      = 0x001F,	 //蓝色
  Color_YELLOW    = 0xFFE0,  //黄色
  Color_GRAY      =	0X8430,  //灰色
	Color_MAGENTA   = 0xF81F,  //洋红色
	Color_CYAN      = 0x07FF,  //青色
	Color_BROWN     = 0xA145,  //棕色
	Color_LIGHTBLUE = 0x841F,  //浅绿
} LCD_Color_t;

//ASCII码字体
typedef enum
{
  ASCII_font_16 = 16,
  ASCII_font_24 = 24,
} ASCII_font_t;
#define IS_ASCII_font(font)   (((font) == ASCII_font_16) || ((font) == ASCII_font_24))

//中文字体
typedef enum
{
  CHN_font_16 = 16,
  CHN_font_24 = 24,
} CHN_font_t;
#define IS_CHN_font(font)   (((font) == CHN_font_16) || ((font) == CHN_font_24))

//画图填充
typedef enum
{
  Filled   = 1,
  unFilled = 0,
} Filled_t;
#define IS_Filled(ucFilled)   (((ucFilled) == Filled) || ((ucFilled) == Unfilled))

//定义结构体类型
typedef struct
{
	uint32_t ID; //屏幕ID
	
	void (*Init)(void);                                                 //LCD屏幕初始化
	void (*FillColor)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t); //LCD屏幕填充颜色
	//Show
	void (*LCD_ShowChar)(uint16_t, uint16_t, const char, uint16_t, uint16_t,ASCII_font_t);         //在LCD屏幕上显示一个英文字符
  void (*LCD_ShowString)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,ASCII_font_t);     //在LCD屏幕上显示英文字符串	
	void (*LCD_ShowCHN)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t);          //在LCD屏幕上显示一个中文字符
	void (*LCD_ShowCHNandENGstring)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t,ASCII_font_t); //在LCD屏幕上显示中英文字符串
	void (*LCD_ShowPicture)(uint16_t, uint16_t, uint16_t, uint16_t,uint8_t);                       //在LCD屏幕上显示图片
	void (*LCD_SetWindows)(uint16_t,uint16_t,uint16_t,uint16_t);                                   //设置LCD显示窗口
	//Shape
	LCD_Color_t (*LCD_GetPointPiexl)(uint16_t,uint16_t);                                 //获取像素点
	void        (*LCD_SetPointPiexl)(uint16_t,uint16_t,LCD_Color_t);                     //设置像素点
	void (*LCD_DrawLine)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t);               //画直线
	void (*LCD_DrawRectangle)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t); //画矩形
	void (*LCD_DrawCircle)(uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t);             //画圆形
} TFT_LCD_t;

/* extern variables-----------------------------------------------------------*/
extern TFT_LCD_t  TFT_LCD;
/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
