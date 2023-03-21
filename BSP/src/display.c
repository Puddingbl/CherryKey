
#include "display.h"
#include "lcdfont.h"

lcd_t lcd1;


static void LCD_WriteDataCnt(lcd_t *lcd, uint16_t *pData ,uint16_t size);
static void LCD_Fill(lcd_t *lcd, uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
static void LCD_DrawPoint(lcd_t *lcd, uint16_t x,uint16_t y,uint16_t color);//��ָ��λ�û�һ����
static void LCD_DrawLine(lcd_t *lcd, uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//��ָ��λ�û�һ����
static void LCD_DrawRectangle(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//��ָ��λ�û�һ������
static void LCD_DrawCircle(lcd_t *lcd, uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//��ָ��λ�û�һ��Բ
static void LCD_ShowChar(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾһ���ַ�
static void LCD_ShowString(lcd_t *lcd, uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ�ַ���
static void LCD_ShowChar1(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey);
static void LCD_ShowString1(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font);
static void LCD_ShowCHN(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font);
static void LCD_ShowCHNandENGstring(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground, uint8_t font_CHN,uint8_t font_ASCII);
static void LCD_ShowIntNum(lcd_t *lcd, uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
static void LCD_ShowFloatNum(lcd_t *lcd, uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������


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
	*һ��д�����256��16λ����
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
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
static void LCD_Fill(lcd_t *lcd, uint16_t xsta, uint16_t ysta, uint16_t xend ,uint16_t yend, uint16_t color)
{          
	uint16_t i,j; 
	
	lcd->LCD_AddressSet(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			lcd->LCD_Write16bit(color);			
		}
	}	
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
static void LCD_DrawPoint(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t color)
{
	lcd->LCD_AddressSet(x,y,x,y);//���ù��λ�� 
	lcd->LCD_Write16bit(color);
} 


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
static void LCD_DrawLine(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		lcd->LCD_DrawPoint(lcd, uRow,uCol,color);//����
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
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
static void LCD_DrawRectangle(lcd_t *lcd, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	lcd->LCD_DrawLine(lcd, x1,y1,x2,y1,color);
	lcd->LCD_DrawLine(lcd, x1,y1,x1,y2,color);
	lcd->LCD_DrawLine(lcd, x1,y2,x2,y2,color);
	lcd->LCD_DrawLine(lcd, x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
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
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
static void LCD_ShowChar1(lcd_t *lcd, uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground, uint8_t font)
{
	uint8_t ucTemp, ucIndex, ucPage, ucColumn;
  
  //�����������Ƿ�Ϸ�
//  assert_param(IS_ASCII_font(font));
  //ASCII�ַ���������������Ҫ��ȥƫ����(' ' -> �ո������ֵ)
	ucIndex = cChar - ' ';
  
	//�ж����� - 16������
	if(font == 16)
	{
		//���ô��ڣ���СΪ8x16
		lcd->LCD_AddressSet(usX,usY,8,16);  //���ù��λ��
//		LCD_SetWindows(usX,usY,8,16);
		//����д�����ݣ���16�У�ÿ��8�����ص�
		for(ucPage=0;ucPage<16;ucPage++)
		{
			//��ASCII�ַ��������ȡ��������	
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
	//�ж����� - 24������
	if(font == 24)
	{
		//���ô��ڣ���СΪ12x24
		lcd->LCD_AddressSet(usX,usY,12,24);
		//����д�����ݣ���24�У�ÿ��12�����ص�(ռ2���ֽ�)
		for(ucPage=0;ucPage<48;ucPage+=2)
		{
			//��ASCII�ַ��������ȡ�������ݣ�ǰ8�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
			ucTemp = ascii_2412[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & 0x01) == 0x01)
					lcd->LCD_Write16bit(usColor_Foreground);			
				else
					lcd->LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
			//��ASCII�ַ��������ȡ�������ݣ���4�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
		//�Զ�����
		if ((usX + font/2) > LCD_W)
		{
			usX = 0;
			usY += font;
		} 
		//�Զ���ҳ
		if ((usY + font) > LCD_H)
		{
			usX = 0;
			usY = 0;
		}
		//��ʾ�ַ�
		LCD_ShowChar1(lcd, usX, usY, *pStr, usColor_Background, usColor_Foreground, font);
		//����λ��
		pStr ++;      
		usX += font/2;
	}
}

static void LCD_ShowChar(lcd_t *lcd, uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t temp,sizex,t,ucIndex,m=0;
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	ucIndex=num-' ';    //�õ�ƫ�ƺ��ֵ
	lcd->LCD_AddressSet(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)
		{
			temp=ascii_1206[ucIndex][i];		       //����6x12����
		}
		else if(sizey==16)
		{
			temp=ascii_1608[ucIndex][i];		 //����8x16����
		}
		else if(sizey==24)
		{
			temp=ascii_2412[ucIndex][i];		 //����12x24����
		}
		else if(sizey==32)
		{	
		  temp=ascii_3216[ucIndex][i];		 //����16x32����
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
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
static void LCD_ShowString(lcd_t *lcd, uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey)
{         
	while(*p!='\0')
	{ 
		//�Զ�����
		if ((x + sizey/2) > LCD_W)
		{
			x = 0;
			y += sizey;
		} 
		//�Զ���ҳ
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
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
static uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
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
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
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
	* @brief  ��LCD��Ļ����ʾ1�������ַ�
	* @param  usX�� ��ʼX����
*           usY ����ʼY����
*           pStr��Ҫ��ʾ��Ӣ���ַ������׵�ַ
*           usColor_Background ��ѡ��Ӣ���ַ��ı���ɫ
*           usColor_Foreground ��ѡ��Ӣ���ַ���ǰ��ɫ
*           font������ѡ��
*             ������CHN_font_16 ��16������
*                   CHN_font_24 ��24������ 
	* @retval None      
*/
static void LCD_ShowCHN(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font)
{
//	uint8_t ucTemp, ucPage, ucColumn;
//	uint16_t usIndex; //�ֿ��еĺ�������
//	uint16_t CHN_Num; //�ֿ��еĺ�������
//	
//	//�����������Ƿ�Ϸ�
////  assert_param(IS_ASCII_font(font));
//	//�ж����� - 16������
//	if(font == CHN_font_16)
//	{
//		//ͳ�ƺ�������
//		CHN_Num = sizeof(FONT_CHN16) / sizeof(FONT_CHN16_t);
//		//forѭ�����Һ���λ��
//		for(usIndex=0;usIndex<CHN_Num;usIndex++)
//		{
//			if((FONT_CHN16[usIndex].Index[0] == *pStr) && (FONT_CHN16[usIndex].Index[1] == *(pStr+1)))
//			{
//				//���ô��ڣ���СΪ16x16
//				LCD_SetWindows(usX,usY,16,16);
//				//����д�����ݣ���16�У�ÿ��16�����ص�
//				for(ucPage=0;ucPage<32;ucPage++)
//				{
//					//��ASCII�ַ��������ȡ��������	
//					//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
//				break; //���ҵ�����ʾ�˺��֣��˳�ѭ��
//			}
//		}	
//	}
//	
//	//�ж����� - 24������
//	if(font == CHN_font_24)
//	{
//		//ͳ�ƺ�������
//		CHN_Num = sizeof(FONT_CHN24) / sizeof(FONT_CHN24_t);
//		//forѭ�����Һ���λ��
//		for(usIndex=0;usIndex<CHN_Num;usIndex++)
//		{
//			if((FONT_CHN24[usIndex].Index[0] == *pStr) && (FONT_CHN24[usIndex].Index[1] == *(pStr+1)))
//			{
//				//���ô��ڣ���СΪ24x24
//				LCD_SetWindows(usX,usY,24,24);
//				//����д�����ݣ���24�У�ÿ��24�����ص�
//				for(ucPage=0;ucPage<72;ucPage++)
//				{
//					//��ASCII�ַ��������ȡ��������	
//					//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
//				break; //���ҵ�����ʾ�˺��֣��˳�ѭ��
//			}
//		}	
//	}
}

/*
	* @name   LCD_ShowCHNandENGstring
	* @brief  ��LCD��Ļ����ʾ��Ӣ���ַ���
	* @param  usX�� ��ʼX����
*           usY ����ʼY����
*           pStr��Ҫ��ʾ����Ӣ���ַ������׵�ַ
*           usColor_Background ��ѡ���ַ��ı���ɫ
*           usColor_Foreground ��ѡ���ַ���ǰ��ɫ
*           font_CHN��  ��������ѡ��
*             ������CHN_font_16 ��16������
*                   CHN_font_24 ��24������ 
*           font_ASCII��ASCII������ѡ��
*             ������ASCII_font_16 ��16������
*                   ASCII_font_24 ��24������ 
	* @retval None      
*/
static void LCD_ShowCHNandENGstring(lcd_t *lcd, uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font_CHN,uint8_t font_ASCII)
{
//	while (* pStr != '\0')
//	{
//		//�����ַ�
//		if((* pStr) > 127)
//		{
//			//�Զ�����
//			if ((usX + font_CHN) > LCD_WIDTH)
//			{
//				usX = 0;
//				usY += font_CHN;
//			} 
//			//�Զ���ҳ
//			if ((usY + font_CHN) > LCD_HEIGTH)
//			{
//				usX = 0;
//				usY = 0;
//			}
//			//��ʾ�����ַ�
//			TFT_LCD.LCD_ShowCHN(usX, usY, pStr, usColor_Background, usColor_Foreground,font_CHN);
//			//����λ��
//			pStr += 2;      
//			usX += font_CHN;
//		}
//		//Ӣ���ַ�
//		else
//		{
//			if((* pStr == '\r') | (* pStr == '\n'))
//		  {
//				//ǰ����ַ�Ϊ����
//				if((* (pStr-1)) > 127)
//				{
//					//����
//					usX = 0;
//					usY += font_CHN;
//				}
//				//ǰ����ַ�ΪӢ��
//				else
//				{
//					//����
//					usX = 0;
//					usY += font_ASCII;	
//				}							
//			}
//			else
//			{
//				//�Զ�����
//				if ((usX + font_ASCII/2) > LCD_WIDTH)
//				{
//					usX = 0;
//					usY += font_ASCII;
//				} 
//				//�Զ���ҳ
//				if ((usY + font_ASCII) > LCD_HEIGTH)
//				{
//					usX = 0;
//					usY = 0;
//				}
//				//��ʾ�ַ�
//				TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font_ASCII);
//				//����λ��     
//				usX += font_ASCII/2;
//			}
//			//ָ����һ���ַ�
//      pStr ++; 			
//		}		
//	}
}
