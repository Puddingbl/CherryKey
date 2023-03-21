/* Includes ------------------------------------------------------------------*/
#include "TFT_LCD.h"
#include "font_ASCII.h"
#include "font_CHN.h"
#include "stdio.h"
#include "public.h"
//#include "Pic1.h"
//#include "Pic2.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void LCD_Init(void);                                                 //LCD屏幕初始化
static void LCD_FillColor(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t); //LCD屏幕填充颜色
static void LCD_ShowChar(uint16_t, uint16_t, const char, uint16_t, uint16_t,ASCII_font_t);     //在LCD屏幕上显示一个英文字符
static void LCD_ShowString(uint16_t, uint16_t, const char *, uint16_t, uint16_t,ASCII_font_t); //在LCD屏幕上显示英文字符串
static void LCD_ShowCHN(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t);      //在LCD屏幕上显示一个中文字符
static void LCD_ShowCHNandENGstring(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t,ASCII_font_t); //在LCD屏幕上显示中英文字符串
static void LCD_ShowPicture(uint16_t, uint16_t, uint16_t, uint16_t,uint8_t);                   //在LCD屏幕上显示图片
static void LCD_SetWindows(uint16_t,uint16_t,uint16_t,uint16_t);                               //设置LCD显示窗口

static LCD_Color_t LCD_GetPointPiexl(uint16_t,uint16_t);                                 //获取像素点
static void        LCD_SetPointPiexl(uint16_t,uint16_t,LCD_Color_t);                     //设置像素点
static void LCD_DrawLine(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t);               //画直线
static void LCD_DrawRectangle(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t); //画矩形
static void LCD_DrawCircle(uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t);             //画圆形

/* Public variables-----------------------------------------------------------*/
TFT_LCD_t TFT_LCD = 
{
	0,
	
	LCD_Init,
	LCD_FillColor,
	LCD_ShowChar,
	LCD_ShowString,
	LCD_ShowCHN,
	LCD_ShowCHNandENGstring,
	LCD_ShowPicture,
	LCD_SetWindows,
	
	LCD_GetPointPiexl,
	LCD_SetPointPiexl,
	LCD_DrawLine,
	LCD_DrawRectangle,
	LCD_DrawCircle
};

/* Private function prototypes------------------------------------------------*/
static uint32_t LCD_ReadID(void);                                //LCD读取ID
static void LCD_Disp_Direction(void);                            //LCD显示方向

/*
	* @name   LCD_ReadID
	* @brief  LCD读取ID
	* @param  None
	* @retval LCD_ID -> 返回LCD屏幕ID    
*/
static uint32_t LCD_ReadID(void)
{
	uint32_t LCD_ID = 0;
	uint32_t buf[4];
	
	LCD_Write_CMD(0xD3);
	buf[0] = LCD_Read_DATA();        // 第一个读取数据无效
	buf[1] = LCD_Read_DATA()&0x00FF; // 只有低8位数据有效
	buf[2] = LCD_Read_DATA()&0x00FF; // 只有低8位数据有效
	buf[3] = LCD_Read_DATA()&0x00FF; // 只有低8位数据有效
	
	LCD_ID = (buf[1] << 16) + (buf[2] << 8) + buf[3];
	return LCD_ID;
}

/*
	* @name   LCD_Init
	* @brief  LCD屏幕初始化
	* @param  None
	* @retval None      
*/
static void LCD_Init(void)
{
	//读取LCD屏幕ID
	TFT_LCD.ID = LCD_ReadID();
	printf("The ID of TFT LCD is 0x%.6X\r\n",TFT_LCD.ID);
	
	//2.8inch ILI9341初始化
	LCD_Write_CMD(0xCF);  
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0xC9);   //C1 
	LCD_Write_DATA(0x30); 
	LCD_Write_CMD(0xED);  
	LCD_Write_DATA(0x64); 
	LCD_Write_DATA(0x03); 
	LCD_Write_DATA(0X12); 
	LCD_Write_DATA(0X81); 
	LCD_Write_CMD(0xE8);  
	LCD_Write_DATA(0x85); 
	LCD_Write_DATA(0x10); 
	LCD_Write_DATA(0x7A); 
	LCD_Write_CMD(0xCB);  
	LCD_Write_DATA(0x39); 
	LCD_Write_DATA(0x2C); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x34); 
	LCD_Write_DATA(0x02); 
	LCD_Write_CMD(0xF7);  
	LCD_Write_DATA(0x20); 
	LCD_Write_CMD(0xEA);  
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_CMD(0xC0);    //Power control 
	LCD_Write_DATA(0x1B);   //VRH[5:0] 
	LCD_Write_CMD(0xC1);    //Power control 
	LCD_Write_DATA(0x00);   //SAP[2:0];BT[3:0] 01 
	LCD_Write_CMD(0xC5);    //VCM control 
	LCD_Write_DATA(0x30); 	//3F
	LCD_Write_DATA(0x30); 	//3C
	LCD_Write_CMD(0xC7);    //VCM control2 
	LCD_Write_DATA(0XB7); 
	//LCD_Write_CMD(0x36);    // Memory Access Control 
	//LCD_Write_DATA(0x08); 
	LCD_Disp_Direction();   //设置LCD显示方向
	LCD_Write_CMD(0x3A);   
	LCD_Write_DATA(0x55); 
	LCD_Write_CMD(0xB1);   
	LCD_Write_DATA(0x00);   
	LCD_Write_DATA(0x1A); 
	LCD_Write_CMD(0xB6);    // Display Function Control 
	LCD_Write_DATA(0x0A); 
	LCD_Write_DATA(0xA2); 
	LCD_Write_CMD(0xF2);    // 3Gamma Function Disable 
	LCD_Write_DATA(0x00); 
	LCD_Write_CMD(0x26);    //Gamma curve selected 
	LCD_Write_DATA(0x01); 
	LCD_Write_CMD(0xE0);    //Set Gamma 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x2A); 
	LCD_Write_DATA(0x28); 
	LCD_Write_DATA(0x08); 
	LCD_Write_DATA(0x0E); 
	LCD_Write_DATA(0x08); 
	LCD_Write_DATA(0x54); 
	LCD_Write_DATA(0XA9); 
	LCD_Write_DATA(0x43); 
	LCD_Write_DATA(0x0A); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 		 
	LCD_Write_CMD(0XE1);    //Set Gamma 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x15); 
	LCD_Write_DATA(0x17); 
	LCD_Write_DATA(0x07); 
	LCD_Write_DATA(0x11); 
	LCD_Write_DATA(0x06); 
	LCD_Write_DATA(0x2B); 
	LCD_Write_DATA(0x56); 
	LCD_Write_DATA(0x3C); 
	LCD_Write_DATA(0x05); 
	LCD_Write_DATA(0x10); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x3F); 
	LCD_Write_DATA(0x3F); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_CMD(0x2B); 
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x3f);
	LCD_Write_CMD(0x2A); 
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0xef);	 
	LCD_Write_CMD(0x11); //Exit Sleep
	HAL_Delay(120);
	LCD_Write_CMD(0x29); //display on		
  
	TFT_LCD_BL_ON;                  //打开背光
	TFT_LCD.FillColor(0,0,LCD_WIDTH,LCD_HEIGTH,Color_GRAY);  //屏幕填充灰色
}

/*
	* @name   LCD_Disp_Directio
	* @brief  LCD显示方向
	* @param  None
	* @retval None      
*/
static void LCD_Disp_Direction()
{
	switch(LCD_DIRECTION)
	{
		case 1:	LCD_Write_CMD(0x36); LCD_Write_DATA(1<<3); break;
		case 2: LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<5)|(1<<6)); break;
		case 3: LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<7)|(1<<4)|(1<<6)); break;
		case 4: LCD_Write_CMD(0x36); LCD_Write_DATA((1<<3)|(1<<7)|(1<<5)|(1<<4)); break;
		default:LCD_Write_CMD(0x36); LCD_Write_DATA(1<<3); break;			
	}
}

/*
	* @name   LCD_SetWindows
	* @brief  设置LCD显示窗口
	* @param  xStar  ->窗口的起点X坐标
						yStar  ->窗口的起点Y坐标
						xWidth ->窗口的宽度
						yHeight->窗口的高度
	* @retval None      
*/
static void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xWidth,uint16_t yHeight)
{	
	LCD_Write_CMD(LCD_CMD_SETxOrgin);	
	LCD_Write_DATA(xStar>>8);
	LCD_Write_DATA(0x00FF&xStar);		
	LCD_Write_DATA((xStar+xWidth-1)>>8);
	LCD_Write_DATA((xStar+xWidth-1)&0xFF);

	LCD_Write_CMD(LCD_CMD_SETyOrgin);
	LCD_Write_DATA(yStar>>8);
	LCD_Write_DATA(0x00FF&yStar);		
	LCD_Write_DATA((yStar+yHeight-1)>>8);
	LCD_Write_DATA((yStar+yHeight-1)&0xFF);

	LCD_Write_CMD(LCD_CMD_WRgram); //开始写入GRAM		
}	

/*
	* @name   LCD_FillColor
	* @brief  LCD屏幕填充颜色
	* @param  xStar  ->窗口的起点X坐标
						yStar  ->窗口的起点Y坐标
						xWidth ->窗口的宽度
						yHeight->窗口的高度 
						FillColor -> 填充色
	* @retval None      
*/
static void LCD_FillColor(uint16_t xStar, uint16_t yStar,uint16_t xWidth,uint16_t yHeight,LCD_Color_t FillColor)
{
	uint16_t i,j;
	//uint16_t k;
	
	//设置窗口
	LCD_SetWindows(xStar,yStar,xWidth,yHeight);
	//填充颜色
	for(i=xStar;i<(xStar+xWidth);i++)
	{
		for(j=0;j<(yStar+yHeight);j++)
		{
			LCD_Write_DATA(FillColor);
			//动态观看屏幕显示过程
			//for(k=0;k<100;k++);
		}
	}
}


/*
	* @name   LCD_ShowChar
	* @brief  在LCD屏幕上显示一个英文字符
	* @param  usX： 起始X坐标
*           usY ：起始Y坐标
*           cChar ：要显示的英文字符
*           usColor_Background ：选择英文字符的背景色
*           usColor_Foreground ：选择英文字符的前景色
*           font：字体选择
*             参数：ASCII_font_16 ：16号字体
*                   ASCII_font_24 ：24号字体 
	* @retval None      
*/
static void LCD_ShowChar(uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground,ASCII_font_t font)
{
	uint8_t ucTemp, ucIndex, ucPage, ucColumn;
  
  //检查输入参数是否合法
  assert_param(IS_ASCII_font(font));
  //ASCII字符集数组索引，需要减去偏移量(' ' -> 空格键的码值)
	ucIndex = cChar - ' ';
  
	//判断字体 - 16号字体
	if(font == ASCII_font_16)
	{
		//设置窗口，大小为8x16
		LCD_SetWindows(usX,usY,8,16);
		//逐行写入数据，共16行，每行8个像素点
		for(ucPage=0;ucPage<16;ucPage++)
		{
			//从ASCII字符集数组获取像素数据	
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
			ucTemp = ucAscii_1608[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write_DATA(usColor_Foreground);			
				else
					LCD_Write_DATA(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
	//判断字体 - 24号字体
	if(font == ASCII_font_24)
	{
		//设置窗口，大小为12x24
		LCD_SetWindows(usX,usY,12,24);
		//逐行写入数据，共24行，每行12个像素点(占2个字节)
		for(ucPage=0;ucPage<48;ucPage+=2)
		{
			//从ASCII字符集数组获取像素数据，前8个像素点
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
			ucTemp = ucAscii_2412[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write_DATA(usColor_Foreground);			
				else
					LCD_Write_DATA(usColor_Background);								
				ucTemp >>= 1;					
			}
			//从ASCII字符集数组获取像素数据，后4个像素点
			//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
      ucTemp=ucAscii_2412[ucIndex][ucPage+1];
			for(ucColumn=0;ucColumn<4;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write_DATA(usColor_Foreground);			
				else
					LCD_Write_DATA(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
}

/*
	* @name   LCD_ShowString
	* @brief  在LCD屏幕上显示英文字符串
	* @param  usX： 起始X坐标
*           usY ：起始Y坐标
*           pStr：要显示的英文字符串的首地址
*           usColor_Background ：选择英文字符的背景色
*           usColor_Foreground ：选择英文字符的前景色
*           font：字体选择
*             参数：ASCII_font_16 ：16号字体
*                   ASCII_font_24 ：24号字体 
	* @retval None      
*/
static void LCD_ShowString(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,ASCII_font_t font)
{
	while (* pStr != '\0')
	{
		//自动换行
		if ((usX + font/2) > LCD_WIDTH)
		{
			usX = 0;
			usY += font;
		} 
		//自动换页
		if ((usY + font) > LCD_HEIGTH)
		{
			usX = 0;
			usY = 0;
		}
		//显示字符
		TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font);
		//更新位置
		pStr ++;      
		usX += font/2;
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
static void LCD_ShowCHN(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,CHN_font_t font)
{
	uint8_t ucTemp, ucPage, ucColumn;
	uint16_t usIndex; //字库中的汉字索引
	uint16_t CHN_Num; //字库中的汉字数量
	
	//检查输入参数是否合法
  assert_param(IS_ASCII_font(font));
	//判断字体 - 16号字体
	if(font == CHN_font_16)
	{
		//统计汉字数量
		CHN_Num = sizeof(FONT_CHN16) / sizeof(FONT_CHN16_t);
		//for循环查找汉字位置
		for(usIndex=0;usIndex<CHN_Num;usIndex++)
		{
			if((FONT_CHN16[usIndex].Index[0] == *pStr) && (FONT_CHN16[usIndex].Index[1] == *(pStr+1)))
			{
				//设置窗口，大小为16x16
				LCD_SetWindows(usX,usY,16,16);
				//逐行写入数据，共16行，每行16个像素点
				for(ucPage=0;ucPage<32;ucPage++)
				{
					//从ASCII字符集数组获取像素数据	
					//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
					ucTemp = FONT_CHN16[usIndex].CHN_code[ucPage];
					for(ucColumn=0;ucColumn<8;ucColumn++)
					{
						if((ucTemp & BIT0) == BIT0)
							LCD_Write_DATA(usColor_Foreground);			
						else
							LCD_Write_DATA(usColor_Background);								
						ucTemp >>= 1;					
					}
				}
						
				break; //已找到并显示了汉字，退出循环
			}
		}	
	}
	
	//判断字体 - 24号字体
	if(font == CHN_font_24)
	{
		//统计汉字数量
		CHN_Num = sizeof(FONT_CHN24) / sizeof(FONT_CHN24_t);
		//for循环查找汉字位置
		for(usIndex=0;usIndex<CHN_Num;usIndex++)
		{
			if((FONT_CHN24[usIndex].Index[0] == *pStr) && (FONT_CHN24[usIndex].Index[1] == *(pStr+1)))
			{
				//设置窗口，大小为24x24
				LCD_SetWindows(usX,usY,24,24);
				//逐行写入数据，共24行，每行24个像素点
				for(ucPage=0;ucPage<72;ucPage++)
				{
					//从ASCII字符集数组获取像素数据	
					//像素点数据为1时，写入字符颜色，为0时，写入背景颜色
					ucTemp = FONT_CHN24[usIndex].CHN_code[ucPage];
					for(ucColumn=0;ucColumn<8;ucColumn++)
					{
						if((ucTemp & BIT0) == BIT0)
							LCD_Write_DATA(usColor_Foreground);			
						else
							LCD_Write_DATA(usColor_Background);								
						ucTemp >>= 1;					
					}
				}
						
				break; //已找到并显示了汉字，退出循环
			}
		}	
	}
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
static void LCD_ShowCHNandENGstring(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,CHN_font_t font_CHN,ASCII_font_t font_ASCII)
{
	while (* pStr != '\0')
	{
		//中文字符
		if((* pStr) > 127)
		{
			//自动换行
			if ((usX + font_CHN) > LCD_WIDTH)
			{
				usX = 0;
				usY += font_CHN;
			} 
			//自动换页
			if ((usY + font_CHN) > LCD_HEIGTH)
			{
				usX = 0;
				usY = 0;
			}
			//显示中文字符
			TFT_LCD.LCD_ShowCHN(usX, usY, pStr, usColor_Background, usColor_Foreground,font_CHN);
			//更新位置
			pStr += 2;      
			usX += font_CHN;
		}
		//英文字符
		else
		{
			if((* pStr == '\r') | (* pStr == '\n'))
		  {
				//前面的字符为中文
				if((* (pStr-1)) > 127)
				{
					//换行
					usX = 0;
					usY += font_CHN;
				}
				//前面的字符为英文
				else
				{
					//换行
					usX = 0;
					usY += font_ASCII;	
				}							
			}
			else
			{
				//自动换行
				if ((usX + font_ASCII/2) > LCD_WIDTH)
				{
					usX = 0;
					usY += font_ASCII;
				} 
				//自动换页
				if ((usY + font_ASCII) > LCD_HEIGTH)
				{
					usX = 0;
					usY = 0;
				}
				//显示字符
				TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font_ASCII);
				//更新位置     
				usX += font_ASCII/2;
			}
			//指向下一个字符
      pStr ++; 			
		}		
	}
}

/*
	* @name   LCD_ShowPicture
	* @brief  在LCD屏幕上显示图片
	* @param  usX    ：起始X坐标
*           usY    ：起始Y坐标
*           Pic_H  ：图片水平分辨率
*           Pic_V  ：图片垂直分辨率
*           Pic_num：图片序号

	* @retval None      
*/
static void LCD_ShowPicture(uint16_t usX, uint16_t usY,uint16_t Pic_H, uint16_t Pic_V,uint8_t Pic_num)
{
	//uint32_t ulIndex;
	//const uint8_t* pic = NULL;
	
	//设置窗口，大小为Pic_HxPic_V
	LCD_SetWindows(usX,usY,Pic_H,Pic_V);
	//获取图像数据首地址
	/*switch(Pic_num)
	{
		case 1: pic = gImage_Pic1; break;
		case 2: pic = gImage_Pic2; break;
		default:pic = gImage_Pic1;
	}*/

	//逐行写入图片数据
	/*for(ulIndex=0;ulIndex<Pic_H*Pic_V*2;ulIndex+=2)
	{
		LCD_Write_DATA((pic[ulIndex]<<8) | pic[ulIndex+1]);
	}*/
}

/*
	* @name   LCD_GetPointPiexl
	* @brief  获取像素点
	* @param  usX    ：X坐标
*           usY    ：Y坐标

  * @retval usColor :色彩数据，RGB565格式      
*/
static LCD_Color_t LCD_GetPointPiexl(uint16_t usX, uint16_t usY)  
{
	uint16_t usR=0, usG=0, usB=0;
	LCD_Color_t Color; //颜色，数据格式RGB565
	
	//判断坐标是否有效
	if((usX < LCD_WIDTH) && (usY < LCD_HEIGTH))
	{
		//设置窗口，大小为1x1
		LCD_SetWindows(usX,usY,1,1);
		//写入读GRAM命令
		LCD_Write_CMD(LCD_CMD_RDgram);
		//开始读数据
		usR = LCD_Read_DATA();  //读Dummy数据，舍弃	
		usR = LCD_Read_DATA();  //读RED数据
		usB = LCD_Read_DATA();  //读BLUE数据
		usG = LCD_Read_DATA();  //读GREEN数据		
		//数据转化为RGB565
		Color = (LCD_Color_t)(((usR>>11)<<11) | ((usG>>10)<<5) | (usB>>11));
		printf("The Pixel color is 0x%.4X\r\n",Color);
	}
	//返回颜色数据
	return	Color;
}	

/*
	* @name   LCD_SetPointPiexl
	* @brief  设置像素点
	* @param  usX     ：X坐标
*           usY     ：Y坐标
*           usColor ：颜色
  * @retval none 
*/
static void  LCD_SetPointPiexl(uint16_t usX, uint16_t usY, LCD_Color_t usColor)              
{
	//判断坐标是否有效
	if((usX < LCD_WIDTH) && (usY < LCD_HEIGTH))
	{
		//设置窗口，大小为1x1
		LCD_SetWindows(usX,usY,1,1);
		//写入颜色数据
		LCD_Write_DATA(usColor);
	}
}

/*
	* @name   LCD_DrawLine
	* @brief  画直线
	* @param  usX_Start ：X起点坐标
*           usY_Start ：Y起点坐标
*           usX_End   ：X终点坐标
*           usY_End   ：Y终点坐标
*           usColor   ：颜色
  * @retval none 
*/
static void LCD_DrawLine(uint16_t usX_Start,uint16_t usY_Start,uint16_t usX_End,uint16_t usY_End,LCD_Color_t usColor)      
{
	uint16_t i; 
	//uint16_t k;
	uint16_t usX_Current, usY_Current;
	int16_t sError_X=0,sError_Y=0,sDelta_X,sDelta_Y,sDistance; 
	int16_t sIncrease_X, sIncrease_Y;
	
	/* 计算坐标增量 */
	sDelta_X = usX_End - usX_Start; 
	sDelta_Y = usY_End - usY_Start; 
	usX_Current = usX_Start; 
	usY_Current = usY_Start; 
	
	if(sDelta_X > 0)
  {
    /* 设置单步方向 */
    sIncrease_X=1; 
	}
  else if(sDelta_X == 0)
  {
    /* 垂直线 */
		sIncrease_X = 0;
	}
	else 
  { 
    sIncrease_X = -1;
    sDelta_X    = -sDelta_X;
  }
  
	if(sDelta_Y > 0)
  {
		sIncrease_Y = 1;
	}
	else if(sDelta_Y==0)
  {
    /* 水平线  */
		sIncrease_Y = 0;
	}
	else
  {
    sIncrease_Y = -1;
    sDelta_Y    = -sDelta_Y;
  }
	
	if(sDelta_X > sDelta_Y)
  {
    /* 选取基本增量坐标轴 */
		sDistance = sDelta_X;  
	}
	else
  {
		sDistance = sDelta_Y; 
  }
	
	//画线输出
	for(i=0;i<=sDistance+1;i++) 
	{
    //画点
		TFT_LCD.LCD_SetPointPiexl(usX_Current,usY_Current,usColor); 
		sError_X += sDelta_X; 
		sError_Y += sDelta_Y;
		if(sError_X > sDistance)
		{ 
			sError_X    -= sDistance; 
			usX_Current += sIncrease_X; 
		}
		if(sError_Y > sDistance) 
		{ 
			sError_Y    -= sDistance; 
			usY_Current += sIncrease_Y; 
		}	
		
		//动态看画线效果
		//for(k=0;k<20000;k++);
	}
}

/*
	* @name   LCD_DrawRectangle
	* @brief  画矩形
	* @param  usX_Start ：X起始坐标
*           usY_Start ：Y起始坐标
*           usWidth   ：矩形的宽度
*           usHeight  ：矩形的高度 
*           usColor   ：矩形的颜色
*           ucFilled  ：选择是否填充矩形
							参数Filled   ：填充
*                   unFilled ：不填充
  * @retval none 
*/
static void LCD_DrawRectangle(uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, LCD_Color_t usColor, Filled_t ucFilled)
{
	//矩形填充
	if(ucFilled == Filled)
	{
		TFT_LCD.FillColor(usX_Start, usY_Start, usWidth, usHeight, usColor);
	}
	//矩形不填充
	else
	{
		//画四条线
		TFT_LCD.LCD_DrawLine(usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start + usWidth - 1, usY_Start + usHeight - 1, usX_Start, usY_Start + usHeight - 1, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start, usY_Start + usHeight - 1, usX_Start, usY_Start, usColor);
	}
}

/*
	* @name   LCD_DrawCircle
	* @brief  在LCD屏幕上使用 Bresenham 算法画圆
	* @param  usX_Center ：圆心X坐标
*           usY_Center ：圆心Y坐标
*           usRadius   ：圆的半径(单位像素)
*           usColor    ：矩形的颜色
*           ucFilled   ：选择是否填充圆形
							参数Filled   ：填充
*                   unFilled ：不填充
  * @retval none 
*/
static void LCD_DrawCircle(uint16_t usX_Center,uint16_t usY_Center,uint16_t usRadius,LCD_Color_t usColor,Filled_t ucFilled)            
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
  
	sCurrentX = 0;
  sCurrentY = usRadius;	
	sError    = 3- (usRadius<<1);   //判断下个点位置的标志
	
	while(sCurrentX <= sCurrentY)
	{
		int16_t sCountY;		
		if(ucFilled)
    {			
			for(sCountY=sCurrentX;sCountY<=sCurrentY;sCountY++)
			{                      
				TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center + sCountY, usColor);     //1，研究对象 
				TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentX, usY_Center + sCountY, usColor);     //2       
				TFT_LCD.LCD_SetPointPiexl(usX_Center - sCountY,   usY_Center + sCurrentX, usColor);   //3
				TFT_LCD.LCD_SetPointPiexl(usX_Center - sCountY,   usY_Center - sCurrentX, usColor);   //4
				TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentX, usY_Center - sCountY, usColor);     //5    
        TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center - sCountY, usColor);     //6
				TFT_LCD.LCD_SetPointPiexl(usX_Center + sCountY,   usY_Center - sCurrentX, usColor);   //7 	
        TFT_LCD.LCD_SetPointPiexl(usX_Center + sCountY,   usY_Center + sCurrentX, usColor);   //0				
			}
    }		
		else
		{          
			TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center + sCurrentY, usColor);     //1，研究对象
			TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentX, usY_Center + sCurrentY, usColor);     //2      
			TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentY, usY_Center + sCurrentX, usColor);     //3
			TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentY, usY_Center - sCurrentX, usColor);     //4
			TFT_LCD.LCD_SetPointPiexl(usX_Center - sCurrentX, usY_Center - sCurrentY, usColor);     //5       
			TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center - sCurrentY, usColor);     //6
			TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentY, usY_Center - sCurrentX, usColor);     //7 
			TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentY, usY_Center + sCurrentX, usColor);     //0
    }			
		sCurrentX++;		
		if(sError < 0) 
    {
			sError += (4*sCurrentX + 6);	  
		}
		else
		{
			sError += (10 + 4*(sCurrentX - sCurrentY));   
			sCurrentY--;
		} 
	}
}
/********************************************************
  End Of File
********************************************************/
