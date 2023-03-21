/* Includes ------------------------------------------------------------------*/
#include "touch.h"
#include "public.h"
#include "spi_norflash.h"


touch_t   touch1;

static int32_t touch_arrayAverage(int32_t *array, uint8_t len);

uint8_t touch_getADC(uint16_t *xValue, uint16_t *yValue)
{
	return cst816t_getADC(xValue, yValue);
}

 void touch_DrawCircle(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t r)
{
	lcd->LCD_FillColor(lcd, 0, 0, lcd->lcd_w, lcd->lcd_h, 0x0000);
	lcd->LCD_DrawCircle(lcd, x, y, r, 0xffff);
}


uint8_t Touch_Calibrate(touch_t *touch, lcd_t *lcd)  
{
	uint16_t circleRadius;
	int32_t touchX[5][6] = {0};
	int32_t touchY[5][6] = {0};
	uint16_t circleX[5], circleY[5];			//要画的圆的圆心坐标	
	int32_t touchX_temp[5] = {0};
	int32_t touchY_temp[5] = {0};	

	uint16_t xOpposite[2],yOpposite[2];   //计算得到对角的坐标
	uint16_t Avr_xOpposite,Avr_yOpposite; //对角坐标的平均值，用于与屏幕中央的坐标值比较
	uint8_t  i, j;

	/*计算四个圆坐标*/
	lcd->lcd_h < lcd->lcd_w ? (circleRadius = lcd->lcd_h) : (circleRadius = lcd->lcd_w);
	circleRadius = circleRadius/30;
	
	circleX[0] = lcd->lcd_w/5;
	circleY[0] = lcd->lcd_h/5;
	
	circleX[1] = lcd->lcd_w/5;
	circleY[1] = lcd->lcd_h - lcd->lcd_h/5;
	
	circleX[2] = lcd->lcd_w - lcd->lcd_w/5;
	circleY[2] = lcd->lcd_h - lcd->lcd_h/5;	

	circleX[3] = lcd->lcd_w - lcd->lcd_w/5;
	circleY[3] = lcd->lcd_h/5;	
	
	circleX[4] = lcd->lcd_w/2;
	circleY[4] = lcd->lcd_h/2;
	
	//读取5个校准点的坐标值
	lcd->LCD_FillColor(lcd,0,0,lcd->lcd_w,lcd->lcd_h,Color_BLACK);
	/*读取四个圆的触摸值，每个读5次*/
	for(i=0; i<5; i++)
	{
//		printf("circleX[%d]=%d, circleY[%d]=%d\n", i, circleX[i], i, circleY[i]);		
		touch_DrawCircle(lcd, circleX[i], circleY[i], circleRadius);
		
		for(j=0; j<5; j++)
		{
//			touch_DrawCircle(lcd, circleX[i], circleY[i], circleRadius);
			while(!touch_getADC((uint16_t*)&touchX[i][j], (uint16_t*)&touchY[i][j]))
			{
				HAL_Delay(50);
			}	
			printf("touchX[%d][%d]=%d, touchY[%d][%d]=%d\n", i,j, touchX[i][j], i,j, touchY[i][j]);
			HAL_Delay(100);
		}
		HAL_Delay(800);
	}		

	for(i=0; i<5; i++)
	{	
		touchX_temp[i] = MedianFilter(&touchX[i][0], 5);
		touchY_temp[i] = MedianFilter(&touchY[i][0], 5);
		printf("touchX_temp[%d]=%d, touchY_temp[%d]=%d\n",i,touchX_temp[i],i, touchY_temp[i]);
	}
	
	//将正方形的4个校准点整合成对角两点，减小触摸误差
	xOpposite[0] = (touchX_temp[0] + touchX_temp[1]) / 2;
	yOpposite[0] = (touchY_temp[0] + touchY_temp[3]) / 2;
	xOpposite[1] = (touchX_temp[2] + touchX_temp[3]) / 2;
	yOpposite[1] = (touchY_temp[1] + touchY_temp[2]) / 2;
	//计算对角两点的平均值
	Avr_xOpposite = (xOpposite[0]+xOpposite[1])/2;
	Avr_yOpposite = (yOpposite[0]+yOpposite[1])/2;
	
	printf("触摸屏坐标值(ADC)：xAvr=%u,yAvr=%u\r\n",Avr_xOpposite,Avr_yOpposite);
//	printf("触摸屏坐标值(ADC)：xMid=%u,yMid=%u\r\n",xValue[4],yValue[4]);
	
	//对校准点进行校验
	if(Avr_xOpposite >= touchX_temp[4]) 
	{
		if((Avr_xOpposite - touchX_temp[4]) > 100)
		{
			printf("校准失败1\r\n");
			lcd->LCD_ShowString(lcd,24,160,"Calibrate Fail",Color_BLACK,Color_RED,ASCII_font_24);
			HAL_Delay(1000);
			return FALSE;
		}
	}
	else
	{
		if((touchX_temp[4] - Avr_xOpposite) > 100)
		{
			printf("校准失败2\r\n");
			lcd->LCD_ShowString(lcd,24,160,"Calibrate Fail",Color_BLACK,Color_RED,ASCII_font_24);
			HAL_Delay(1000);
			return FALSE;
		}
	}
	printf("这里3\n");
	if(Avr_yOpposite >= touchY_temp[4]) 
	{
		if((Avr_yOpposite - touchY_temp[4]) > 100)
		{
			printf("校准失败3\r\n");
			lcd->LCD_ShowString(lcd,24,160,"Calibrate Fail",Color_BLACK,Color_RED,ASCII_font_24);
			HAL_Delay(1000);
			return FALSE;
		}
	}
	else
	{
		if((touchY_temp[4] - Avr_yOpposite) > 100)
		{
			printf("校准失败4\r\n");
			lcd->LCD_ShowString(lcd,24,160,"Calibrate Fail",Color_BLACK,Color_GREEN,ASCII_font_24);
			HAL_Delay(1000);
			return FALSE;
		}
	}
	printf("这里4\n");
	//计算比例因素
	touch->xFactor = (float)(lcd->lcd_w - 2*lcd->lcd_w/5) / (xOpposite[1] - xOpposite[0]);
	touch->yFactor = (float)(lcd->lcd_h - 2*lcd->lcd_h/5) / (yOpposite[1] - yOpposite[0]);
	
	//计算偏移量
		touch->xOffset = (int)(lcd->lcd_w/2 - touch->xFactor*Avr_xOpposite);
		touch->yOffset = (int)(lcd->lcd_h/2 - touch->yFactor*Avr_yOpposite);		
	
	//设置校准标志位
	touch->Calibrate_Flag = 1;
	
	printf("校准成功\r\n");
	printf("校准因素xFactor：%.2f\r\n",touch->xFactor);
	printf("校准因素yFactor：%.2f\r\n",touch->yFactor);
	printf("偏移量xOffset：  %d\r\n",touch->xOffset);
	printf("偏移量yOffset：  %d\r\n",touch->yOffset);
	lcd->LCD_ShowString(lcd,12,160,"Calibrate Success",Color_BLACK,Color_GREEN,ASCII_font_24);
	HAL_Delay(1000);
	
	////保存参数
	//扇区擦除
	SPI_Flash.EraseSector(Touch_Calibrate_Para_Addr);
	SPI_Flash.WriteUnfixed(&touch->Calibrate_Flag,Touch_Calibrate_Para_Addr,sizeof(touch));
	
	return TRUE;
}

void touch_test(void)
{
	uint8_t t=0; 	    
// 	uint16_t lastpos[2][2];		//最后一次的数据 
	uint16_t lastpos[2];	
//	lcd1.LCD_FillColor(&lcd1, 0,0,LCD_W,LCD_H,WHITE);

	while(!touch_getAxis(&touch1, &lastpos[0], &lastpos[1])){HAL_Delay(5);}
	lcd1.LCD_DrawPoint(&lcd1, lastpos[0], lastpos[1], 0XFC07);
	HAL_Delay(5);
}

/**
	@name touch_getAxis
	@     获取跟屏幕匹配的触摸坐标
@     touch:触摸屏  xValue\yValue:返回的坐标
*/
uint8_t touch_getAxis(touch_t *touch, uint16_t *xValue, uint16_t *yValue)
{
	uint16_t x, y;
	
	if(touch_getADC(&x, &y))
	{
//		*xValue = touch->cal.a[0]*x + touch->cal.a[1]*y + touch->cal.a[2];
//		*yValue = touch->cal.a[3]*x + touch->cal.a[4]*y + touch->cal.a[5];
//		*xValue = touch->Xa*x + touch->Xb*y + touch->Xc;
//		*yValue = touch->Ya*x + touch->Yb*y + touch->Yc;
		*xValue = touch->xFactor*x + touch->xOffset;
		*yValue = touch->yFactor*y + touch->yOffset;	
		return 1;
	}	
	return 0;
}

static int32_t touch_arrayAverage(int32_t *array, uint8_t len)
{
	int32_t sum = 0;
	int32_t avr = 0;
	uint8_t i;
	
	for(i=0; i<len; i++)
	{
		sum += array[i];
	}
	printf("sum=%d\n",sum);
	avr = sum/len;
	
	return avr;
}

//int perform_calibration(calibration *cal)
//{
//	int j;
//	float n, x, y, x2, y2, xy, z, zx, zy;
//	float det, a, b, c, e, f, i;
//	float scaling = 65536.0;

//	/* Get sums for matrix */
//	n = x = y = x2 = y2 = xy = 0;
//	for (j = 0; j < 5; j++) {
//		n += 1.0;
//		x += (float)cal->x[j];
//		y += (float)cal->y[j];
//		x2 += (float)(cal->x[j]*cal->x[j]);
//		y2 += (float)(cal->y[j]*cal->y[j]);
//		xy += (float)(cal->x[j]*cal->y[j]);
//	}

//	/* Get determinant of matrix -- check if determinant is too small */
//	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
//	if (det < 0.1 && det > -0.1) {
//		printf("ts_calibrate: determinant is too small -- %f\n", det);
//		return 0;
//	}

//	/* Get elements of inverse matrix */
//	a = (x2*y2 - xy*xy)/det;
//	b = (xy*y - x*y2)/det;
//	c = (x*xy - y*x2)/det;
//	e = (n*y2 - y*y)/det;
//	f = (x*y - n*xy)/det;
//	i = (n*x2 - x*x)/det;

//	/* Get sums for x calibration */
//	z = zx = zy = 0;
//	for (j = 0; j < 5; j++) {
//		z += (float)cal->xfb[j];
//		zx += (float)(cal->xfb[j]*cal->x[j]);
//		zy += (float)(cal->xfb[j]*cal->y[j]);
//	}

//	/* Now multiply out to get the calibration for framebuffer x coord */
////	cal->a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
////	cal->a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
////	cal->a[2] = (int)((c*z + f*zx + i*zy)*(scaling));
//	
//	cal->a[0] = ((a*z + b*zx + c*zy));
//	cal->a[1] = ((b*z + e*zx + f*zy));
//	cal->a[2] = ((c*z + f*zx + i*zy));	

//	printf("%f %f %f\n", (a*z + b*zx + c*zy),
//			     (b*z + e*zx + f*zy),
//			     (c*z + f*zx + i*zy));

//	/* Get sums for y calibration */
//	z = zx = zy = 0;
//	for (j = 0; j < 5; j++) {
//		z += (float)cal->yfb[j];
//		zx += (float)(cal->yfb[j]*cal->x[j]);
//		zy += (float)(cal->yfb[j]*cal->y[j]);
//	}

//	/* Now multiply out to get the calibration for framebuffer y coord */
//	cal->a[3] = ((a*z + b*zx + c*zy));
//	cal->a[4] = ((b*z + e*zx + f*zy));
//	cal->a[5] = ((c*z + f*zx + i*zy));

////	cal->a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
////	cal->a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
////	cal->a[5] = (int)((c*z + f*zx + i*zy)*(scaling));	
//	
//	printf("%f %f %f\n", (a*z + b*zx + c*zy),
//			     (b*z + e*zx + f*zy),
//			     (c*z + f*zx + i*zy));

//	/* If we got here, we're OK, so assign scaling to a[6] and return */
////	cal->a[6] = (int)scaling;

//	return 1;
//}

//void touch_calibrate(touch_t *touch, lcd_t *lcd)
//{
//	
//	
//	float k;
//	uint8_t i, j;
//	
//	uint16_t circleRadius;
//	int32_t touchX[5][6] = {0};
//	int32_t touchY[5][6] = {0};
//	uint16_t circleX[5], circleY[5];			//要画的圆的圆心坐标	
//	int32_t touchX_temp[5] = {0};
//	int32_t touchY_temp[5] = {0};
//	
//	/*计算四个圆坐标*/
//	lcd->lcd_h < lcd->lcd_w ? (circleRadius = lcd->lcd_h) : (circleRadius = lcd->lcd_w);
//	circleRadius = circleRadius/20;
//	
//	circleX[0] = lcd->lcd_w/5;
//	circleY[0] = lcd->lcd_h/5;
//	
//	circleX[1] = lcd->lcd_w/5;
//	circleY[1] = lcd->lcd_h - lcd->lcd_h/5;
//	
//	circleX[2] = lcd->lcd_w - lcd->lcd_w/5;
//	circleY[2] = lcd->lcd_h - lcd->lcd_h/5;	

//	circleX[3] = lcd->lcd_w - lcd->lcd_w/5;
//	circleY[3] = lcd->lcd_h/5;	
//	
//	circleX[4] = lcd->lcd_w/2;
//	circleY[4] = lcd->lcd_h/2;	
//	
//	touch->cal.xfb[0] = circleX[0];
//	touch->cal.yfb[0] = circleY[0];
//	
//	touch->cal.xfb[1] = circleX[1];
//	touch->cal.yfb[1] = circleY[1];
//	
//	touch->cal.xfb[2] = circleX[2];
//	touch->cal.yfb[2] = circleY[2];
//	
//	touch->cal.xfb[3] = circleX[3];
//	touch->cal.yfb[3] = circleY[3];
//	
//	touch->cal.xfb[4] = circleX[4];
//	touch->cal.yfb[4] = circleY[4];	
//	
//	/*读取四个圆的触摸值，每个读5次*/
//	for(i=0; i<5; i++)
//	{
////		printf("circleX[%d]=%d, circleY[%d]=%d\n", i, circleX[i], i, circleY[i]);		
//		touch_DrawCircle(lcd, circleX[i], circleY[i], circleRadius);
//		
//		for(j=0; j<5; j++)
//		{
//			while(!touch_getADC((uint16_t*)&touchX[i][j], (uint16_t*)&touchY[i][j]))
//			{
//				HAL_Delay(50);
//			}	
//			printf("touchX[%d][%d]=%d, touchY[%d][%d]=%d\n", i,j, touchX[i][j], i,j, touchY[i][j]);
//		}
//		HAL_Delay(200);
//	}	
//	
////	for(i=0; i<5; i++)
////	{
////		touchX_temp[i] = touch_arrayAverage(&touchX[i][0], 5);
////		touchY_temp[i] = touch_arrayAverage(&touchY[i][0], 5);
////		printf("touchX_temp[%d]=%d, touchY_temp[%d]=%d\n",i,touchX_temp[i],i, touchY_temp[i]);
////	}
//	
//	for(i=0; i<5; i++)
//	{
//		touch->cal.x[i] = touch_arrayAverage(&touchX[i][0], 5);
//		touch->cal.y[i] = touch_arrayAverage(&touchY[i][0], 5);
//		printf("touch->cal.x[%d]=%d, touch->cal.y[%d]=%d\n",i,touch->cal.x[i],i, touch->cal.y[i]);
//	}	
//	
//	perform_calibration(&touch->cal);
//	
////	k = (float)(((touchX_temp[0] - touchX_temp[1])*(touchY_temp[1] - touchY_temp[2])) \
////			- ((touchX_temp[1] - touchX_temp[2])*(touchY_temp[0] - touchY_temp[1])));
////	printf("k=%f\n", k);
////	
////	/*计算X轴相关参数*/	
////	touch->Xa = (float)(((circleX[0] - circleX[1])*(touchY_temp[1] - touchY_temp[2])) \
////			- ((circleX[1] - circleX[2])*(touchY_temp[0] - touchY_temp[1]))) / k;
////	
////	touch->Xb = (float)(((touchX_temp[0] - touchX_temp[1])*(circleX[1] - circleX[2])) \
////			- ((touchX_temp[1] - touchX_temp[2])*(circleX[0] - circleX[1]))) / k;
////	
////	touch->Xc = circleX[0] - touch->Xa*touchX_temp[0] - touch->Xa*touchY_temp[0];
////	
////	/*计算Y轴相关参数*/
////	touch->Ya = (float)(((circleY[0] - circleY[1])*(touchY_temp[1] - touchY_temp[2])) \
////			- ((circleY[1] - circleY[2])*(touchY_temp[0] - touchY_temp[1]))) / k;
////	
////	touch->Yb = (float)(((touchX_temp[0] - touchX_temp[1])*(circleY[1] - circleY[2])) \
////			- ((touchX_temp[1] - touchX_temp[2])*(circleY[0] - circleY[1]))) / k;
////	
////	touch->Yc = circleY[0] - touch->Ya*touchY_temp[0] - touch->Yb*touchY_temp[0];		
////	
////	printf("Xa=%f, Xb=%f, Xc=%f\n", touch->Xa, touch->Xb, touch->Xc);
////	printf("Ya=%f, Yb=%f, Yc=%f\n", touch->Ya, touch->Yb, touch->Yc);
//	
//}

/********************************************************
  End Of File
********************************************************/
