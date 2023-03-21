#ifndef __TOUCH_H__
#define __TOUCH_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/

#include "main.h"
#include "CST816T.h"
#include "filter.h"

/* define---------------------------------------------------------------------*/
#define  Touch_Calibrate_Para_Addr    0x00040000


typedef struct
{
	float a[7];
	int x[5];
	int y[5];
	int xfb[5];
	int yfb[5];
}	calibration;

typedef struct
{
//	float Xa;
//	float Xb;
//	float Xc;
//	float Ya;
//	float Yb;
//	float Yc;
//	calibration cal;
	uint8_t Calibrate_Flag;
	int16_t	  xOffset;     //X轴偏移量
	int16_t   yOffset;     //Y轴偏移量
	float   xFactor;     //X轴比例因子
	float   yFactor;     //Y轴比例因子
	
} touch_t;





/* extern variables-----------------------------------------------------------*/
extern touch_t   touch1;

/* exported functions ------------------------------------------------------- */
void touch_DrawCircle(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t r);
uint8_t touch_getADC(uint16_t *xValue, uint16_t *yValue);
void touch_test(void);
uint8_t Touch_Calibrate(touch_t *touch, lcd_t *lcd);
uint8_t touch_getAxis(touch_t *touch, uint16_t *xValue, uint16_t *yValue);
#ifdef __cplusplus
}
#endif

#endif
/********************************************************
  End Of File
********************************************************/
