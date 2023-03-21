#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__

//ͷ�ļ�
#include "main.h"

////�궨��
//TFT��Ļ�������
#define	TFT_LCD_BL_ON	  HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_SET)
#define	TFT_LCD_BL_OFF	HAL_GPIO_WritePin(lcd_bl_GPIO_Port,lcd_bl_Pin,GPIO_PIN_RESET)

/******************************************************************************
NOR/PSRAM�洢����ַ��
64MB:FSMC_Bank1_NORSRAM1:0x6000 0000 ~ 0x63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0x6400 0000 ~ 0x67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0x6800 0000 ~ 0x6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0x6C00 0000 ~ 0x6FFF FFFF

ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0x6000 0000 ~ 0x63FF FFFF
STM32������ʵս��Ŀ������ѡ�� FSMC_A0 ��LCD��D/CX(����/����ѡ��)��
�����ַ = 0x6000 0000
���ݵ�ַ = 0x6000 0002 = 0x6000 0000+(1<<(0+1))
�����·���ʱѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���        
eg:ѡ��A10�������ݻ���ַ = 0x6000 0000+(1<<(10+1)) = 0x6000 0800
*******************************************************************************/
#define	FSMC_LCD_CMD_ADDR		((uint32_t)0x60000000)  //FSMC_Bank1_NORSRAM1����LCD��������ĵ�ַ
#define	FSMC_LCD_DATA_ADDR  ((uint32_t)0x60100000)  //FSMC_Bank1_NORSRAM1����LCD���ݲ����ĵ�ַ  

//LCD��д�����궨��
#define	LCD_Write_CMD(CMD) 	  *((__IO uint16_t *)FSMC_LCD_CMD_ADDR)  = (uint16_t)CMD
#define	LCD_Write_DATA(DATA)  *((__IO uint16_t *)FSMC_LCD_DATA_ADDR) = (uint16_t)DATA  
#define	LCD_Read_DATA()       *((__IO uint16_t *)FSMC_LCD_DATA_ADDR)

//��ʾ����ѡ�񣬿�ѡ(1,2,3,4)�ĸ�����
#define LCD_DIRECTION                  1  // ����,��ʱ����ת0�ȣ� ԭ������Ļ���Ͻ� X*Y=240*320
//#define LCD_DIRECTION                  2  // ����,��ʱ����ת90��, ԭ������Ļ���Ͻ� X*Y=320*240
//#define LCD_DIRECTION                  3  // ����,��ʱ����ת180��,ԭ������Ļ���½� X*Y=240*320
//#define LCD_DIRECTION                  4  // ����,��ʱ����ת270��,ԭ������Ļ���½� X*Y=320*240

//LCD��Ļ�Ŀ����߶�
#if (LCD_DIRECTION == 1)||(LCD_DIRECTION == 3)
 #define LCD_WIDTH		         240  // X�᳤��
 #define LCD_HEIGTH         	 300  // Y�᳤��
#else
 #define LCD_WIDTH		         320  // X�᳤��
 #define LCD_HEIGTH         	 240  // Y�᳤�� 
#endif

//LCD����
#define LCD_CMD_SETxOrgin		0x2A
#define LCD_CMD_SETyOrgin   0x2B
#define LCD_CMD_WRgram      0x2C
#define LCD_CMD_RDgram      0x2E

////����ö������
//��ɫ����
typedef enum
{
	Color_BLACK     = 0x0000,	 //��ɫ
	Color_WHITE     = 0xFFFF,	 //��ɫ
  Color_RED       = 0xF800,	 //��ɫ
  Color_GREEN     = 0x07E0,	 //��ɫ 
  Color_BLUE      = 0x001F,	 //��ɫ
  Color_YELLOW    = 0xFFE0,  //��ɫ
  Color_GRAY      =	0X8430,  //��ɫ
	Color_MAGENTA   = 0xF81F,  //���ɫ
	Color_CYAN      = 0x07FF,  //��ɫ
	Color_BROWN     = 0xA145,  //��ɫ
	Color_LIGHTBLUE = 0x841F,  //ǳ��
} LCD_Color_t;

//ASCII������
typedef enum
{
  ASCII_font_16 = 16,
  ASCII_font_24 = 24,
} ASCII_font_t;
#define IS_ASCII_font(font)   (((font) == ASCII_font_16) || ((font) == ASCII_font_24))

//��������
typedef enum
{
  CHN_font_16 = 16,
  CHN_font_24 = 24,
} CHN_font_t;
#define IS_CHN_font(font)   (((font) == CHN_font_16) || ((font) == CHN_font_24))

//��ͼ���
typedef enum
{
  Filled   = 1,
  unFilled = 0,
} Filled_t;
#define IS_Filled(ucFilled)   (((ucFilled) == Filled) || ((ucFilled) == Unfilled))

//����ṹ������
typedef struct
{
	uint32_t ID; //��ĻID
	
	void (*Init)(void);                                                 //LCD��Ļ��ʼ��
	void (*FillColor)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t); //LCD��Ļ�����ɫ
	//Show
	void (*LCD_ShowChar)(uint16_t, uint16_t, const char, uint16_t, uint16_t,ASCII_font_t);         //��LCD��Ļ����ʾһ��Ӣ���ַ�
  void (*LCD_ShowString)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,ASCII_font_t);     //��LCD��Ļ����ʾӢ���ַ���	
	void (*LCD_ShowCHN)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t);          //��LCD��Ļ����ʾһ�������ַ�
	void (*LCD_ShowCHNandENGstring)(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t,ASCII_font_t); //��LCD��Ļ����ʾ��Ӣ���ַ���
	void (*LCD_ShowPicture)(uint16_t, uint16_t, uint16_t, uint16_t,uint8_t);                       //��LCD��Ļ����ʾͼƬ
	void (*LCD_SetWindows)(uint16_t,uint16_t,uint16_t,uint16_t);                                   //����LCD��ʾ����
	//Shape
	LCD_Color_t (*LCD_GetPointPiexl)(uint16_t,uint16_t);                                 //��ȡ���ص�
	void        (*LCD_SetPointPiexl)(uint16_t,uint16_t,LCD_Color_t);                     //�������ص�
	void (*LCD_DrawLine)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t);               //��ֱ��
	void (*LCD_DrawRectangle)(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t); //������
	void (*LCD_DrawCircle)(uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t);             //��Բ��
} TFT_LCD_t;

/* extern variables-----------------------------------------------------------*/
extern TFT_LCD_t  TFT_LCD;
/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
