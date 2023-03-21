#include "spi_144lcd.h"
#include "lcdfont.h"
#include "public.h"
#include "spi.h"

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Write8bit(uint8_t data) 
{	
	uint8_t pData =data;
	LCD_CS_Clr();
  HAL_SPI_Transmit(&hspi1, &pData, 1, 1000);
	LCD_CS_Set();
}

void LCD_Write16bit(uint16_t data) 
{	
	LCD_Write8bit(data>>8);
	LCD_Write8bit(data);
	
//	uint8_t pData[2] = {0};
//	pData[0] = (data>>8);
//	pData[1] = data&0x00ff;
//  HAL_SPI_Transmit(&hspi1, pData, 2, 1000);
}
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t command)
{
	LCD_DC_Clr();//写命令
	LCD_Write8bit(command);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
//	uint8_t X1[2] = {0};
//	uint8_t X2[2] = {0};
//	uint8_t Y1[2] = {0};
//	uint8_t Y2[2] = {0};
//	if((USE_HORIZONTAL==0) || (USE_HORIZONTAL==1))
//	{	
//	  x1+=2; x2+=2;
//		y1+=1; y2+=1;
//	}
//	else if((USE_HORIZONTAL==2) || (USE_HORIZONTAL==3))
//	{
//	  x1+=1; x2+=1;
//		y1+=2; y2+=2;		
//	}		
//	X1[0] = x1>>8; X1[1] = 0X00FF&x1;
//	X2[0] = x2>>8; X2[1] = 0X00FF&x2;
//	Y1[0] = y1>>8; Y1[1] = 0X00FF&y1;
//	Y2[0] = y2>>8; Y2[1] = 0X00FF&y2;

	LCD_WR_REG(0x2a);//列地址设置
	LCD_Write16bit(x1);
	LCD_Write16bit(x2);
	LCD_WR_REG(0x2b);//行地址设置
	LCD_Write16bit(y1+32);
	LCD_Write16bit(y2+32);
	LCD_WR_REG(0x2c);//储存器写
}

void LCD_Init(void)
{
// uint8_t Frame_Rate[3];
	
	LCD_RES_Clr();//复位
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//打开背光
  HAL_Delay(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	HAL_Delay(120);             //Delay 120ms 
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_WR_REG(0xB1);
	LCD_Write8bit(0x05); 
	LCD_Write8bit(0x3C); 
	LCD_Write8bit(0x3C); 
	LCD_WR_REG(0xB2); 
	LCD_Write8bit(0x05);
	LCD_Write8bit(0x3C); 
	LCD_Write8bit(0x3C); 
	LCD_WR_REG(0xB3); 
	LCD_Write8bit(0x05); 
	LCD_Write8bit(0x3C); 
	LCD_Write8bit(0x3C); 
	LCD_Write8bit(0x05); 
	LCD_Write8bit(0x3C); 
	LCD_Write8bit(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	LCD_WR_REG(0xB4); //Dot inversion 
	LCD_Write8bit(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	LCD_WR_REG(0xC0); 
	LCD_Write8bit(0x28); 
	LCD_Write8bit(0x08); 
	LCD_Write8bit(0x04); 
	LCD_WR_REG(0xC1); 
	LCD_Write8bit(0XC0); 
	LCD_WR_REG(0xC2); 
	LCD_Write8bit(0x0D); 
	LCD_Write8bit(0x00); 
	LCD_WR_REG(0xC3); 
	LCD_Write8bit(0x8D); 
	LCD_Write8bit(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_Write8bit(0x8D); 
	LCD_Write8bit(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	LCD_WR_REG(0xC5); //VCOM 
	LCD_Write8bit(0x1A); 
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	if(USE_HORIZONTAL==0)LCD_Write8bit(0x00);
	else if(USE_HORIZONTAL==1)LCD_Write8bit(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Write8bit(0x70);
	else LCD_Write8bit(0xA0); 
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	LCD_WR_REG(0xE0); 
	LCD_Write8bit(0x04); 
	LCD_Write8bit(0x22); 
	LCD_Write8bit(0x07); 
	LCD_Write8bit(0x0A); 
	LCD_Write8bit(0x2E); 
	LCD_Write8bit(0x30); 
	LCD_Write8bit(0x25); 
	LCD_Write8bit(0x2A); 
	LCD_Write8bit(0x28); 
	LCD_Write8bit(0x26); 
	LCD_Write8bit(0x2E); 
	LCD_Write8bit(0x3A); 
	LCD_Write8bit(0x00); 
	LCD_Write8bit(0x01); 
	LCD_Write8bit(0x03); 
	LCD_Write8bit(0x13); 
	LCD_WR_REG(0xE1); 
	LCD_Write8bit(0x04); 
	LCD_Write8bit(0x16); 
	LCD_Write8bit(0x06); 
	LCD_Write8bit(0x0D); 
	LCD_Write8bit(0x2D); 
	LCD_Write8bit(0x26); 
	LCD_Write8bit(0x23); 
	LCD_Write8bit(0x27); 
	LCD_Write8bit(0x27); 
	LCD_Write8bit(0x25); 
	LCD_Write8bit(0x2D); 
	LCD_Write8bit(0x3B); 
	LCD_Write8bit(0x00); 
	LCD_Write8bit(0x01); 
	LCD_Write8bit(0x04); 
	LCD_Write8bit(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	LCD_WR_REG(0x3A); //65k mode 
	LCD_Write8bit(0x05); 
	LCD_WR_REG(0x29); //Display on 	 
} 

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_Write16bit(color);
		}
	} 					  	    
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_Write16bit(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
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
		LCD_DrawPoint(uRow,uCol,color);//画点
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
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
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
void LCD_ShowChar(uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,ucIndex,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	ucIndex=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	
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
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))
				{
					LCD_Write16bit(fc);
				}
				else 
				{
					LCD_Write16bit(bc);
				}
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))
				{
					LCD_DrawPoint(x,y,fc);//画一个点
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
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
void LCD_ShowString(uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
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
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
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
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
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
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}
			else 
			{
				enshow=1; 
			}
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
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
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
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
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}

/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: uint16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	LCD_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	LCD_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	LCD_DrawLine(x1+1,y2-1,x2,y2-1, 0x8410);  //H
	LCD_DrawLine(x1,  y2,  x2,y2, 0x4208);  //H
	LCD_DrawLine(x2-1,y1+1,x2-1,y2, 0x8410);  //V
  LCD_DrawLine(x2  ,y1  ,x2,y2, 0x4208); //V
}

void DisplayButtonDown(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	LCD_DrawLine(x1,  y1,  x2,y1, 0x4208);  //H
	LCD_DrawLine(x1+1,y1+1,x2,y1+1, 0x8410);  //H
	LCD_DrawLine(x1,  y1,  x1,y2, 0x4208);  //V
	LCD_DrawLine(x1+1,y1+1,x1+1,y2, 0x8410);  //V
	LCD_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	LCD_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}
