#ifndef __CST816T_H__
#define __CST816T_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "display.h"
#include "ST7789V.h"
#include "public.h"
#include "filter.h"
/* define---------------------------------------------------------------------*/
#define CST_WR_CMD 0x2A  //д�����ַ
#define CST_RD_CMD 0x2B  //д���ݵ�ַ

#define TP_PRES_DOWN 0x81  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 

#define CTP_MAX_TOUCH 2       

#define  FT_DEVIDE_MODE 0x00	   // CST816ģʽ���ƼĴ���
#define  GT_ADDR_REG    0x02     //����״̬�Ĵ���
#define  CST816S_ID     0xA7     //CST816SID�Ĵ���

//CST816�Ĵ���
#define Gesture 			0x01		//���ƼĴ���
#define FingerNum			0x02		//��ָ����   ʵ����ֻ�ܼ��һ����ָ
#define XposH				0x03		//x����λ
#define XposL				0x04		//x�Ͱ�λ
#define YposH				0x05		//y����λ
#define YposL				0x06		//y�Ͱ�λ
#define ChipID				0xA7		//оƬ�ͺ�
#define	MotionMask		    0xEC		//��������
#define AutoSleepTime	    0xF9		//�Զ�����
#define IrqCrl				0xFA		//�жϿ���
#define AutoReset			0xFB		//����������
#define LongPressTime	    0xFC		//��������
#define DisAutoSleep	    0xFE		//ʹ�ܵ͹���ģʽ

//����
enum GESTURE
{
    None = 0x00,       //������
    SlideDown = 0x01,  //���»���
    SlideUp = 0x02,    //���ϻ���
    SlideLeft = 0x03,  //���󻬶�
    SlideRight = 0x04, //���һ���
    SingleTap = 0x05,  //����
    DoubleTap = 0x0B,  //˫��
    LongPress = 0x0C   //����
};

#define  CTP_RES_Set()	 HAL_GPIO_WritePin(TP_REST_GPIO_Port,TP_REST_Pin,GPIO_PIN_SET)
#define  CTP_RES_Clr()	 HAL_GPIO_WritePin(TP_REST_GPIO_Port,TP_REST_Pin,GPIO_PIN_RESET)

#define  CTP_INT_IN()
#define  CTP_INT_Set()   HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_SET)
#define  CTP_INT_Clr()	 HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_RESET)

#define   Touch_Error        15    //���
//X����Y������߽��趨
#define   Touch_X_MAX        35000  //X�����ֵ
#define   Touch_X_MIN        32000   //X����Сֵ
#define   Touch_Y_MAX        300  //Y�����ֵ
#define   Touch_Y_MIN        0   //Y����Сֵ
typedef struct
{
	void (*init)(void);   //����IC��ʼ��
	uint8_t (*scan)(uint8_t);       //����ɨ��
	uint16_t x[CTP_MAX_TOUCH]; //
	uint16_t y[CTP_MAX_TOUCH];
	uint8_t  sta;              //����״̬  
} cst816t_t;





/* extern variables-----------------------------------------------------------*/
extern cst816t_t tp_dev;

/* exported functions ------------------------------------------------------- */
uint8_t CST816S_WR_DATA(uint16_t addr,uint8_t data);
uint8_t CST816S_RD_DATA(uint16_t addr,uint8_t len,uint8_t *value);
void  CST816S_Init(void);
uint8_t cst816t_getADC(uint16_t *xValue, uint16_t *yValue);





uint8_t CST816S_Scan(uint8_t mode);
void CST816S_test(void);
#ifdef __cplusplus
}
#endif

#endif