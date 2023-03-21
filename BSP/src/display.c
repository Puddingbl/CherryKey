
#include "display.h"
#include "lcdfont.h"

lcd_t lcd1;


static void LCD_WriteDataCnt(lcd_t *lcd, uint16_t *pData ,uint16_t size);
static void LCD_Fill(lcd_t *lcd, uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
static void LCD_DrawPoint(lcd_t *lcd, uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点
static void LCD_DrawLine(lcd_t *lcd, uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
static void LCD_DrawRectangle(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩形
static void LCD_DrawCircle(lcd_t *lcd, uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆
static void LCD_ShowChar(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey);//显示一个字符
static void LCD_ShowString(lcd_t *lcd, uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey);//显示字符串
static void LCD_ShowChar1(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey);
static void LCD_ShowString1(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font);
static void LCD_ShowCHN(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font);
static void LCD_ShowCHNandENGstring(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground, uint8_t font_CHN,uint8_t font_ASCII);
static void LCD_ShowIntNum(lcd_t *lcd, uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示整数变量
static void LCD_ShowFloatNum(lcd_t *lcd, uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示两位小数变量


//void lcd_init(lcd_t *lcd, void LCD_DriveInit(void), void (*LCD_Write16bit)(uint16_t data), void (*LCD_AddressSet)(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2))
//{
void lcd_init(lcd_t *lcd, void *LCD_DriveInit, void *LCD_Write16bit, void *LCD_AddressSet, uint16_t lcd_w, uint16_t lcd_h)
{
	lcd->lcd_w = lcd_w;
	lcd->lcd_h = lcd_h;
	lcd->LCD_DriveInit = LCD_DriveInit;
	lcd->LCD_Write16bit = LCD_Write16bit;
	lcd->LCD_AddressSet = LCD_AddressSet;
	
	lcd->LCD_WriteDataCnt = LCD_WriteDataCnt;
	lcd->LCD_FillColor = LCD_Fill;
	lcd->LCD_DrawPoint = LCD_DrawPoint;
	lcd->LCD_DrawLine = LCD_DrawLine;
	lcd->LCD_DrawRectangle = LCD_DrawRectangle;
	lcd->LCD_DrawCircle = LCD_DrawCircle;
	lcd->LCD_ShowChar = LCD_ShowChar;
	lcd->LCD_ShowString = LCD_ShowString;
	lcd->LCD_ShowIntNum = LCD_ShowIntNum;
	lcd->LCD_ShowFloatNum = LCD_ShowFloatNum;
	lcd->LCD_ShowCHN = LCD_ShowCHN;
	lcd->LCD_ShowCHNandENGstring = LCD_ShowCHNandENGstring;
	
	lcd->LCD_GetPointPiexl = NULL;
}


/**
	*一次写入最多256个16位数据
*/
static void LCD_WriteDataCnt(lcd_t *lcd, uint16_t *pData ,uint16_t size)
{
//	if(size <= 0 || size >256)
//	{
//		return;
//	}

//	uint8_t data[size*2];
	
	LCD_CS_Clr();
//	for(uint16_t i=0; i<size; i+=1)
//	{
////		SW16(pData[i]);	
//		data[2*i] = pData[i]>>8;
//		data[2*i+1] = pData[i]&0xff;
////		printf("data[%d]=%d, data[%d]=%d\n",2*i,data[2*i], 2*i+1,data[2*i+1]);
//	}		
	
//	SCB_CleanInvalidateDCache();	
//	HAL_SPI_Transmit_DMA(&hspi1, data, size*2);

	HAL_SPI_Transmit(&hspi1, (uint8_t *)pData, size*2, 100);	
	LCD_CS_Set();		
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
static void LCD_Fill(lcd_t *lcd, uint16_t xsta, uint16_t ysta, uint16_t xend ,uint16_t yend, uint16_t color)
{          
	uint16_t i,j; 
	
	lcd->LCD_AddressSet(xsta,ysta,xend-1,yend-1);//设置显示范围
	
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			lcd->LCD_Write16bit(color);			
		}
	}	
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
static void LCD_DrawPoint(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t color)
{
	lcd->LCD_AddressSet(x,y,x,y);//设置光标位置 
	lcd->LCD_Write16bit(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
static void LCD_DrawLine(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		lcd->LCD_DrawPoint(lcd, uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
static void LCD_DrawRectangle(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	lcd->LCD_DrawLine(lcd, x1,y1,x2,y1,color);
	lcd->LCD_DrawLine(lcd, x1,y1,x1,y2,color);
	lcd->LCD_DrawLine(lcd, x1,y2,x2,y2,color);
	lcd->LCD_DrawLine(lcd, x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
static void LCD_DrawCircle(lcd_t *lcd, uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		lcd->LCD_DrawPoint(lcd, x0-b,y0-a,color);             //3           
		lcd->LCD_DrawPoint(lcd, x0+b,y0-a,color);             //0           
		lcd->LCD_DrawPoint(lcd, x0-a,y0+b,color);             //1                
		lcd->LCD_DrawPoint(lcd, x0-a,y0-b,color);             //2             
		lcd->LCD_DrawPoint(lcd, x0+b,y0+a,color);             //4               
		lcd->LCD_DrawPoint(lcd, x0+a,y0-b,color);             //5
		lcd->LCD_DrawPoint(lcd, x0+a,y0+b,color);             //6 
		lcd->LCD_DrawPoint(lcd, x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
static void LCD_ShowChar1(lcd_t *lcd, uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground, uint8_t font)
{
	uint8_t ucTemp, ucIndex, ucPage, ucColumn;
  
  //检查输入参数是否合法
//  assert_param(IS_ASCII_font(font));
  //ASCII字符集数组索引，需要减去偏移量(' ' -> 空格键的码值)
	ucIndex = cChar - ' ';
  
	//判断字体 - 16号字体
	if(font == 16)
	{
		//设置窗口，大小为8x16
		lcd->LCD_AddressSet(usX,usY,8,16);  //设置光标位置
//		LCD_SetWindows(usX,usY,8,16);
		//逐行写入数据，共16行，每行8个像素点
		for(ucPage=0;ucPage<16;ucPage++)
		{
			//从ASCII字符集数组获取像素数据	
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
			ucTemp = ascii_1608[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & 0x01) == 0x01)
					lcd->LCD_Write16bit(usColor_Foreground);			
				else
					lcd->LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
	//判断字体 - 24号字体
	if(font == 24)
	{
		//设置窗口，大小为12x24
		lcd->LCD_AddressSet(usX,usY,12,24);
		//逐行写入数据，共24行，每行12个像素点(占2个字节)
		for(ucPage=0;ucPage<48;ucPage+=2)
		{
			//从ASCII字符集数组获取像素数据，前8个像素点
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
			ucTemp = ascii_2412[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & 0x01) == 0x01)
					lcd->LCD_Write16bit(usColor_Foreground);			
				else
					lcd->LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
			//从ASCII字符集数组获取像素数据，后4个像素点
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
      ucTemp=ascii_2412[ucIndex][ucPage+1];
			for(ucColumn=0;ucColumn<4;ucColumn++)
			{
				if((ucTemp & 0x01) == 0x01)
					lcd->LCD_Write16bit(usColor_Foreground);			
				else
					lcd->LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
}

static void LCD_ShowString1(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font)
{
	while (* pStr != '\0')
	{
		//自动换行
		if ((usX + font/2) > LCD_W)
		{
			usX = 0;
			usY += font;
		} 
		//自动换页
		if ((usY + font) > LCD_H)
		{
			usX = 0;
			usY = 0;
		}
		//显示字符
		LCD_ShowChar1(lcd, usX, usY, *pStr, usColor_Background, usColor_Foreground, font);
		//更新位置
		pStr ++;      
		usX += font/2;
	}
}

static void LCD_ShowChar(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t temp,sizex,t,ucIndex,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	ucIndex=num-' ';    //得到偏移后的值
	lcd->LCD_AddressSet(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)
		{
			temp=ascii_1206[ucIndex][i];		       //调用6x12字体
		}
		else if(sizey==16)
		{
			temp=ascii_1608[ucIndex][i];		 //调用8x16字体
		}
		else if(sizey==24)
		{
			temp=ascii_2412[ucIndex][i];		 //调用12x24字体
		}
		else if(sizey==32)
		{	
		  temp=ascii_3216[ucIndex][i];		 //调用16x32字体
		}
		else return;
		
		for(t=0;t<8;t++)
		{
			if(temp&(0x01<<t))
			{
				lcd->LCD_Write16bit(fc);
			}
			else 
			{
				lcd->LCD_Write16bit(bc);
			}
			m++;
			if(m%sizex==0)
			{
				m=0;
				break;
			}
		}
	}   	 	  
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
static void LCD_ShowString(lcd_t *lcd, uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey)
{         
	while(*p!='\0')
	{ 
		//自动换行
		if ((x + sizey/2) > LCD_W)
		{
			x = 0;
			y += sizey;
		} 
		//自动换页
		if ((y + sizey) > LCD_H)
		{
			x = 0;
			y = 0;
		}		
		lcd->LCD_ShowChar(lcd, x,y,*p,fc,bc,sizey);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
static uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
static void LCD_ShowIntNum(lcd_t *lcd, uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd->LCD_ShowChar(lcd, x+t*sizex,y,' ',fc,bc,sizey);
				continue;
			}
			else 
			{
				enshow=1; 
			}
		 	 
		}
	 	lcd->LCD_ShowChar(lcd, x+t*sizex,y,temp+48,fc,bc,sizey);
	}
} 


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
static void LCD_ShowFloatNum(lcd_t *lcd, uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			lcd->LCD_ShowChar(lcd, x+(len-2)*sizex,y,'.',fc,bc,sizey);
			t++;
			len+=1;
		}
	 	lcd->LCD_ShowChar(lcd, x+t*sizex,y,temp+48,fc,bc,sizey);
	}
}

/*
	* @name   LCD_ShowCHN
	* @brief  在LCD屏幕上显示1个中文字符
	* @param  usX： 起始X坐标
*           usY ：起始Y坐标
*           pStr：要显示的英文字符串的首地址
*           usColor_Background ：选择英文字符的背景色
*           usColor_Foreground ：选择英文字符的前景色
*           font：字体选择
*             参数：CHN_font_16 ：16号字体
*                   CHN_font_24 ：24号字体 
	* @retval None      
*/
static void LCD_ShowCHN(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font)
{
//	uint8_t ucTemp, ucPage, ucColumn;
//	uint16_t usIndex; //字库中的汉字索引
//	uint16_t CHN_Num; //字库中的汉字数量
//	
//	//检查输入参数是否合法
////  assert_param(IS_ASCII_font(font));
//	//判断字体 - 16号字体
//	if(font == CHN_font_16)
//	{
//		//统计汉字数量
//		CHN_Num = sizeof(FONT_CHN16) / sizeof(FONT_CHN16_t);
//		//for循环查找汉字位置
//		for(usIndex=0;usIndex<CHN_Num;usIndex++)
//		{
//			if((FONT_CHN16[usIndex].Index[0] == *pStr) && (FONT_CHN16[usIndex].Index[1] == *(pStr+1)))
//			{
//				//设置窗口，大小为16x16
//				LCD_SetWindows(usX,usY,16,16);
//				//逐行写入数据，共16行，每行16个像素点
//				for(ucPage=0;ucPage<32;ucPage++)
//				{
//					//从ASCII字符集数组获取像素数据	
//					//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
//					ucTemp = FONT_CHN16[usIndex].CHN_code[ucPage];
//					for(ucColumn=0;ucColumn<8;ucColumn++)
//					{
//						if((ucTemp & BIT0) == BIT0)
//							LCD_Write_DATA(usColor_Foreground);			
//						else
//							LCD_Write_DATA(usColor_Background);								
//						ucTemp >>= 1;					
//					}
//				}
//						
//				break; //已找到并显示了汉字，退出循环
//			}
//		}	
//	}
//	
//	//判断字体 - 24号字体
//	if(font == CHN_font_24)
//	{
//		//统计汉字数量
//		CHN_Num = sizeof(FONT_CHN24) / sizeof(FONT_CHN24_t);
//		//for循环查找汉字位置
//		for(usIndex=0;usIndex<CHN_Num;usIndex++)
//		{
//			if((FONT_CHN24[usIndex].Index[0] == *pStr) && (FONT_CHN24[usIndex].Index[1] == *(pStr+1)))
//			{
//				//设置窗口，大小为24x24
//				LCD_SetWindows(usX,usY,24,24);
//				//逐行写入数据，共24行，每行24个像素点
//				for(ucPage=0;ucPage<72;ucPage++)
//				{
//					//从ASCII字符集数组获取像素数据	
//					//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
//					ucTemp = FONT_CHN24[usIndex].CHN_code[ucPage];
//					for(ucColumn=0;ucColumn<8;ucColumn++)
//					{
//						if((ucTemp & BIT0) == BIT0)
//							LCD_Write_DATA(usColor_Foreground);			
//						else
//							LCD_Write_DATA(usColor_Background);								
//						ucTemp >>= 1;					
//					}
//				}
//						
//				break; //已找到并显示了汉字，退出循环
//			}
//		}	
//	}
}

/*
	* @name   LCD_ShowCHNandENGstring
	* @brief  在LCD屏幕上显示中英文字符串
	* @param  usX： 起始X坐标
*           usY ：起始Y坐标
*           pStr：要显示的中英文字符串的首地址
*           usColor_Background ：选择字符的背景色
*           usColor_Foreground ：选择字符的前景色
*           font_CHN：  中文字体选择
*             参数：CHN_font_16 ：16号字体
*                   CHN_font_24 ：24号字体 
*           font_ASCII：ASCII码字体选择
*             参数：ASCII_font_16 ：16号字体
*                   ASCII_font_24 ：24号字体 
	* @retval None      
*/
static void LCD_ShowCHNandENGstring(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font_CHN,uint8_t font_ASCII)
{
//	while (* pStr != '\0')
//	{
//		//中文字符
//		if((* pStr) > 127)
//		{
//			//自动换行
//			if ((usX + font_CHN) > LCD_WIDTH)
//			{
//				usX = 0;
//				usY += font_CHN;
//			} 
//			//自动换页
//			if ((usY + font_CHN) > LCD_HEIGTH)
//			{
//				usX = 0;
//				usY = 0;
//			}
//			//显示中文字符
//			TFT_LCD.LCD_ShowCHN(usX, usY, pStr, usColor_Background, usColor_Foreground,font_CHN);
//			//更新位置
//			pStr += 2;      
//			usX += font_CHN;
//		}
//		//英文字符
//		else
//		{
//			if((* pStr == '\r') | (* pStr == '\n'))
//		  {
//				//前面的字符为中文
//				if((* (pStr-1)) > 127)
//				{
//					//换行
//					usX = 0;
//					usY += font_CHN;
//				}
//				//前面的字符为英文
//				else
//				{
//					//换行
//					usX = 0;
//					usY += font_ASCII;	
//				}							
//			}
//			else
//			{
//				//自动换行
//				if ((usX + font_ASCII/2) > LCD_WIDTH)
//				{
//					usX = 0;
//					usY += font_ASCII;
//				} 
//				//自动换页
//				if ((usY + font_ASCII) > LCD_HEIGTH)
//				{
//					usX = 0;
//					usY = 0;
//				}
//				//显示字符
//				TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font_ASCII);
//				//更新位置     
//				usX += font_ASCII/2;
//			}
//			//指向下一个字符
//      pStr ++; 			
//		}		
//	}
}
