#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "ST7789V.h"
#include "TFT_LCD.h"

/* define---------------------------------------------------------------------*/
//оƬ����
#define   ST7789V                     1

//���ݴ��䷽ʽ
#define   DISPLAY_I2C									0
#define		DISPLAY_SPI									0
#define		DISPLAY_8080_8							0
#define		DISPLAY_8080_16							1





//����ṹ������
typedef struct lcd
{
	/*��ʾ���ֱ���*/
	uint16_t   lcd_w;
	uint16_t   lcd_h;
	
	/*��������*/
	void (*LCD_DriveInit)(void);                                               													  									//LCD��Ļ��ʼ��
	void (*LCD_Write16bit)(uint16_t data);
	void (*LCD_WriteDataCnt)(struct lcd *lcd, uint16_t *pData ,uint16_t size);
	void (*LCD_AddressSet)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);																					//�������꺯��	
	
	/*ͼ�κ���*/
	void (*LCD_FillColor)(struct lcd *lcd, uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color); 									//LCD��Ļ�����ɫ
	void (*LCD_DrawPoint)(struct lcd *lcd, uint16_t x, uint16_t y, uint16_t color);//��ָ��λ�û�һ����
	void (*LCD_DrawLine)(struct lcd *lcd, uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);																//��ָ��λ�û�һ����
	void (*LCD_DrawCircle)(struct lcd *lcd, uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);																						//��ָ��λ�û�һ��Բ	
	void (*LCD_DrawRectangle)(struct lcd *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);												//��ָ��λ�û�һ������
	
	/*�ַ���ʾ����*/
	void (*LCD_ShowChar)(struct lcd *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t size);			//��ʾһ���ַ�
	void (*LCD_ShowString)(struct lcd *lcd, uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t size);			//��ʾ�ַ���
	void (*LCD_ShowIntNum)(struct lcd *lcd, uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size);				//��ʾ��������
	void (*LCD_ShowFloatNum)(struct lcd *lcd, uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size);				//��ʾ��λС������
	void (*LCD_ShowCHN)(struct lcd *lcd, uint16_t x,uint16_t y, const char *num, uint16_t fc, uint16_t bc, uint8_t size);          										//��LCD��Ļ����ʾһ�������ַ�
	void (*LCD_ShowCHNandENGstring)(struct lcd *lcd, uint16_t x,uint16_t y, const char *num, uint16_t fc, uint16_t bc, uint8_t size1,uint8_t size2); 			//��LCD��Ļ����ʾ��Ӣ���ַ���
	
	uint16_t (*LCD_GetPointPiexl)(struct lcd *lcd, uint16_t,uint16_t);                                 																	//��ȡ���ص���ɫ
} lcd_t;


/* extern variables-----------------------------------------------------------*/
extern lcd_t lcd1;

/* exported functions ------------------------------------------------------- */
void lcd_init(lcd_t *lcd, void *LCD_DriveInit, void *LCD_Write16bit, void *LCD_AddressSet, uint16_t lcd_w, uint16_t lcd_h);
//void lcd_init(lcd_t *lcd, void LCD_DriveInit(void), void (*LCD_Write16bit)(uint16_t data), void (*LCD_AddressSet)(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2));

#ifdef __cplusplus
}
#endif

#endif