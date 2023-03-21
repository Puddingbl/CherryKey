#include "SPI_18LCD.h"
#include "lcdfont.h"
#include "public.h"
#include "spi.h"

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Write8bit(uint8_t data) 
{	
	uint8_t pData =data;
	LCD_CS_Clr();
  HAL_SPI_Transmit(&hspi1, &pData, 1, 100);
	
//	LL_SPI_TransmitData8(SPI1, pData);
//	while(!LL_SPI_IsActiveFlag_TXP(SPI1));
	
//	HAL_SPI_Transmit_DMA(&hspi1, &pData, 1);
	LCD_CS_Set();
}

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
//void LCD_Write8bit(uint8_t dat) 
//{	
//	uint8_t i;
//	LCD_CS_Clr();
//	for(i=0;i<8;i++)
//	{			  
//		LCD_SCLK_Clr();
//		if(dat&0x80)
//		{
//		   LCD_MOSI_Set();
//		}
//		else
//		{
//		   LCD_MOSI_Clr();
//		}
//		LCD_SCLK_Set();
//		dat<<=1;
//	}	
//  LCD_CS_Set();	
//}

void LCD_Write16bit(uint16_t data) 
{	
	LCD_Write8bit(data>>8);
	LCD_Write8bit(data);
}


void LCD_WriteDataCount(uint16_t *pData ,uint8_t size)
{
	
}

//#define sw16(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
//#define SW16(A)	sw16(((uint16_t)A))

/**
	*һ��д�����256��16λ����
*/
void LCD_WriteData256(uint16_t *pData ,uint16_t size)
{
//	if(size <= 0 || size >256)
//	{
//		return;
//	}

	uint8_t data[size*2];
	
	LCD_CS_Clr();
	for(uint16_t i=0; i<size; i+=1)
	{
//		SW16(pData[i]);	
		data[2*i] = pData[i]>>8;
		data[2*i+1] = pData[i]&0xff;
//		printf("data[%d]=%d, data[%d]=%d\n",2*i,data[2*i], 2*i+1,data[2*i+1]);
	}		
	
//	SCB_CleanInvalidateDCache();	
//	HAL_SPI_Transmit_DMA(&hspi1, data, size*2);

	HAL_SPI_Transmit(&hspi1, data, size*2, 100);	
	LCD_CS_Set();		
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t command)
{
	LCD_DC_Clr();//д����
	LCD_Write8bit(command);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
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

		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_Write16bit(x1);
		LCD_Write16bit(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_Write16bit(y1);
		LCD_Write16bit(y2);
		LCD_WR_REG(0x2c);//������д		

}

void LCD_Init(void)
{
// uint8_t Frame_Rate[3];
	
	LCD_RES_Clr();//��λ
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//�򿪱���
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
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
//	uint32_t size = (xend-xsta) * (yend-ysta);
//	uint16_t size1 = size/10;
//	uint8_t  size2 = size%10;
	
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_Write16bit(color);			
		}
	}

//	if(size1 <= 1)
//	{
//		uint16_t pData[size];
//		
//		for(uint8_t k=0; k<size; k++)
//		{
//			pData[k] = color;
//		}
//		LCD_WriteData256(pData , size);	
//	}
//	else 
//	{
//		for(uint16_t t=0; t<size1; t++)
//		{
//			uint16_t pData[size];
//			
//			for(uint16_t k=0; k<10; k++)
//			{
//				pData[k] = color;
//			}	
//			LCD_WriteData256(pData , 10);	
//		}
//		if(size2 > 0)
//		{
//			uint16_t pData[size2];
//			
//			for(uint16_t k=0; k<size2; k++)
//			{
//				pData[k] = color;
//			}	
//			LCD_WriteData256(pData , size2);	
//		}			
//	}	
	
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_Write16bit(color);
} 


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
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
		LCD_DrawPoint(uRow,uCol,color);//����
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
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
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
static void LCD_ShowChar1(uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground, uint8_t font)
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
		LCD_Address_Set(usX,usY,8,16);  //���ù��λ��
//		LCD_SetWindows(usX,usY,8,16);
		//����д�����ݣ���16�У�ÿ��8�����ص�
		for(ucPage=0;ucPage<16;ucPage++)
		{
			//��ASCII�ַ��������ȡ��������	
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
			ucTemp = ascii_1608[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write16bit(usColor_Foreground);			
				else
					LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
	//�ж����� - 24������
	if(font == 24)
	{
		//���ô��ڣ���СΪ12x24
		LCD_Address_Set(usX,usY,12,24);
		//����д�����ݣ���24�У�ÿ��12�����ص�(ռ2���ֽ�)
		for(ucPage=0;ucPage<48;ucPage+=2)
		{
			//��ASCII�ַ��������ȡ�������ݣ�ǰ8�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
			ucTemp = ascii_2412[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write16bit(usColor_Foreground);			
				else
					LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
			//��ASCII�ַ��������ȡ�������ݣ���4�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
      ucTemp=ascii_2412[ucIndex][ucPage+1];
			for(ucColumn=0;ucColumn<4;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write16bit(usColor_Foreground);			
				else
					LCD_Write16bit(usColor_Background);								
				ucTemp >>= 1;					
			}
		} 
	}
}

void LCD_ShowString1(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,uint8_t font)
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
		LCD_ShowChar1(usX, usY, *pStr, usColor_Background, usColor_Foreground, font);
		//����λ��
		pStr ++;      
		usX += font/2;
	}
}

void LCD_ShowChar(uint16_t x,uint16_t y,const char num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,ucIndex,m=0;
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	ucIndex=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	
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
			if(!mode)//�ǵ���ģʽ
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
			else//����ģʽ
			{
				if(temp&(0x01<<t))
				{
					LCD_DrawPoint(x,y,fc);//��һ����
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
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const char *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
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
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
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
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
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
