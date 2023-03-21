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
#define CST_WR_CMD 0x2A  //写命令地址
#define CST_RD_CMD 0x2B  //写数据地址

#define TP_PRES_DOWN 0x81  		//触屏被按下	  
#define TP_CATH_PRES 0x40  		//有按键按下了 

#define CTP_MAX_TOUCH 2       

#define  FT_DEVIDE_MODE 0x00	   // CST816模式控制寄存器
#define  GT_ADDR_REG    0x02     //触摸状态寄存器
#define  CST816S_ID     0xA7     //CST816SID寄存器

//CST816寄存器
#define Gesture 			0x01		//手势寄存器
#define FingerNum			0x02		//手指数量   实际上只能检测一个手指
#define XposH				0x03		//x高四位
#define XposL				0x04		//x低八位
#define YposH				0x05		//y高四位
#define YposL				0x06		//y低八位
#define ChipID				0xA7		//芯片型号
#define	MotionMask		    0xEC		//触发动作
#define AutoSleepTime	    0xF9		//自动休眠
#define IrqCrl				0xFA		//中断控制
#define AutoReset			0xFB		//无手势休眠
#define LongPressTime	    0xFC		//长按休眠
#define DisAutoSleep	    0xFE		//使能低功耗模式

//手势
enum GESTURE
{
    None = 0x00,       //无手势
    SlideDown = 0x01,  //向下滑动
    SlideUp = 0x02,    //向上滑动
    SlideLeft = 0x03,  //向左滑动
    SlideRight = 0x04, //向右滑动
    SingleTap = 0x05,  //单击
    DoubleTap = 0x0B,  //双击
    LongPress = 0x0C   //长按
};

#define  CTP_RES_Set()	 HAL_GPIO_WritePin(TP_REST_GPIO_Port,TP_REST_Pin,GPIO_PIN_SET)
#define  CTP_RES_Clr()	 HAL_GPIO_WritePin(TP_REST_GPIO_Port,TP_REST_Pin,GPIO_PIN_RESET)

#define  CTP_INT_IN()
#define  CTP_INT_Set()   HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_SET)
#define  CTP_INT_Clr()	 HAL_GPIO_WritePin(lcd_rst_GPIO_Port,lcd_rst_Pin,GPIO_PIN_RESET)

#define   Touch_Error        15    //误差
//X轴与Y轴坐标边界设定
#define   Touch_X_MAX        35000  //X轴最大值
#define   Touch_X_MIN        32000   //X轴最小值
#define   Touch_Y_MAX        300  //Y轴最大值
#define   Touch_Y_MIN        0   //Y轴最小值
typedef struct
{
	void (*init)(void);   //触摸IC初始化
	uint8_t (*scan)(uint8_t);       //触摸扫描
	uint16_t x[CTP_MAX_TOUCH]; //
	uint16_t y[CTP_MAX_TOUCH];
	uint8_t  sta;              //触摸状态  
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