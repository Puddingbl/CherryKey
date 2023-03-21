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
static void LCD_Init(void);                                                 //LCD��Ļ��ʼ��
static void LCD_FillColor(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t); //LCD��Ļ�����ɫ
static void LCD_ShowChar(uint16_t, uint16_t, const char, uint16_t, uint16_t,ASCII_font_t);     //��LCD��Ļ����ʾһ��Ӣ���ַ�
static void LCD_ShowString(uint16_t, uint16_t, const char *, uint16_t, uint16_t,ASCII_font_t); //��LCD��Ļ����ʾӢ���ַ���
static void LCD_ShowCHN(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t);      //��LCD��Ļ����ʾһ�������ַ�
static void LCD_ShowCHNandENGstring(uint16_t, uint16_t, const char *, uint16_t, uint16_t,CHN_font_t,ASCII_font_t); //��LCD��Ļ����ʾ��Ӣ���ַ���
static void LCD_ShowPicture(uint16_t, uint16_t, uint16_t, uint16_t,uint8_t);                   //��LCD��Ļ����ʾͼƬ
static void LCD_SetWindows(uint16_t,uint16_t,uint16_t,uint16_t);                               //����LCD��ʾ����

static LCD_Color_t LCD_GetPointPiexl(uint16_t,uint16_t);                                 //��ȡ���ص�
static void        LCD_SetPointPiexl(uint16_t,uint16_t,LCD_Color_t);                     //�������ص�
static void LCD_DrawLine(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t);               //��ֱ��
static void LCD_DrawRectangle(uint16_t,uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t); //������
static void LCD_DrawCircle(uint16_t,uint16_t,uint16_t,LCD_Color_t,Filled_t);             //��Բ��

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
static uint32_t LCD_ReadID(void);                                //LCD��ȡID
static void LCD_Disp_Direction(void);                            //LCD��ʾ����

/*
	* @name   LCD_ReadID
	* @brief  LCD��ȡID
	* @param  None
	* @retval LCD_ID -> ����LCD��ĻID    
*/
static uint32_t LCD_ReadID(void)
{
	uint32_t LCD_ID = 0;
	uint32_t buf[4];
	
	LCD_Write_CMD(0xD3);
	buf[0] = LCD_Read_DATA();        // ��һ����ȡ������Ч
	buf[1] = LCD_Read_DATA()&0x00FF; // ֻ�е�8λ������Ч
	buf[2] = LCD_Read_DATA()&0x00FF; // ֻ�е�8λ������Ч
	buf[3] = LCD_Read_DATA()&0x00FF; // ֻ�е�8λ������Ч
	
	LCD_ID = (buf[1] << 16) + (buf[2] << 8) + buf[3];
	return LCD_ID;
}

/*
	* @name   LCD_Init
	* @brief  LCD��Ļ��ʼ��
	* @param  None
	* @retval None      
*/
static void LCD_Init(void)
{
	//��ȡLCD��ĻID
	TFT_LCD.ID = LCD_ReadID();
	printf("The ID of TFT LCD is 0x%.6X\r\n",TFT_LCD.ID);
	
	//2.8inch ILI9341��ʼ��
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
	LCD_Disp_Direction();   //����LCD��ʾ����
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
  
	TFT_LCD_BL_ON;                  //�򿪱���
	TFT_LCD.FillColor(0,0,LCD_WIDTH,LCD_HEIGTH,Color_GRAY);  //��Ļ����ɫ
}

/*
	* @name   LCD_Disp_Directio
	* @brief  LCD��ʾ����
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
	* @brief  ����LCD��ʾ����
	* @param  xStar  ->���ڵ����X����
						yStar  ->���ڵ����Y����
						xWidth ->���ڵĿ��
						yHeight->���ڵĸ߶�
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

	LCD_Write_CMD(LCD_CMD_WRgram); //��ʼд��GRAM		
}	

/*
	* @name   LCD_FillColor
	* @brief  LCD��Ļ�����ɫ
	* @param  xStar  ->���ڵ����X����
						yStar  ->���ڵ����Y����
						xWidth ->���ڵĿ��
						yHeight->���ڵĸ߶� 
						FillColor -> ���ɫ
	* @retval None      
*/
static void LCD_FillColor(uint16_t xStar, uint16_t yStar,uint16_t xWidth,uint16_t yHeight,LCD_Color_t FillColor)
{
	uint16_t i,j;
	//uint16_t k;
	
	//���ô���
	LCD_SetWindows(xStar,yStar,xWidth,yHeight);
	//�����ɫ
	for(i=xStar;i<(xStar+xWidth);i++)
	{
		for(j=0;j<(yStar+yHeight);j++)
		{
			LCD_Write_DATA(FillColor);
			//��̬�ۿ���Ļ��ʾ����
			//for(k=0;k<100;k++);
		}
	}
}


/*
	* @name   LCD_ShowChar
	* @brief  ��LCD��Ļ����ʾһ��Ӣ���ַ�
	* @param  usX�� ��ʼX����
*           usY ����ʼY����
*           cChar ��Ҫ��ʾ��Ӣ���ַ�
*           usColor_Background ��ѡ��Ӣ���ַ��ı���ɫ
*           usColor_Foreground ��ѡ��Ӣ���ַ���ǰ��ɫ
*           font������ѡ��
*             ������ASCII_font_16 ��16������
*                   ASCII_font_24 ��24������ 
	* @retval None      
*/
static void LCD_ShowChar(uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground,ASCII_font_t font)
{
	uint8_t ucTemp, ucIndex, ucPage, ucColumn;
  
  //�����������Ƿ�Ϸ�
  assert_param(IS_ASCII_font(font));
  //ASCII�ַ���������������Ҫ��ȥƫ����(' ' -> �ո������ֵ)
	ucIndex = cChar - ' ';
  
	//�ж����� - 16������
	if(font == ASCII_font_16)
	{
		//���ô��ڣ���СΪ8x16
		LCD_SetWindows(usX,usY,8,16);
		//����д�����ݣ���16�У�ÿ��8�����ص�
		for(ucPage=0;ucPage<16;ucPage++)
		{
			//��ASCII�ַ��������ȡ��������	
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
	//�ж����� - 24������
	if(font == ASCII_font_24)
	{
		//���ô��ڣ���СΪ12x24
		LCD_SetWindows(usX,usY,12,24);
		//����д�����ݣ���24�У�ÿ��12�����ص�(ռ2���ֽ�)
		for(ucPage=0;ucPage<48;ucPage+=2)
		{
			//��ASCII�ַ��������ȡ�������ݣ�ǰ8�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
			ucTemp = ucAscii_2412[ucIndex][ucPage];
			for(ucColumn=0;ucColumn<8;ucColumn++)
			{
				if((ucTemp & BIT0) == BIT0)
					LCD_Write_DATA(usColor_Foreground);			
				else
					LCD_Write_DATA(usColor_Background);								
				ucTemp >>= 1;					
			}
			//��ASCII�ַ��������ȡ�������ݣ���4�����ص�
			//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
	* @brief  ��LCD��Ļ����ʾӢ���ַ���
	* @param  usX�� ��ʼX����
*           usY ����ʼY����
*           pStr��Ҫ��ʾ��Ӣ���ַ������׵�ַ
*           usColor_Background ��ѡ��Ӣ���ַ��ı���ɫ
*           usColor_Foreground ��ѡ��Ӣ���ַ���ǰ��ɫ
*           font������ѡ��
*             ������ASCII_font_16 ��16������
*                   ASCII_font_24 ��24������ 
	* @retval None      
*/
static void LCD_ShowString(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,ASCII_font_t font)
{
	while (* pStr != '\0')
	{
		//�Զ�����
		if ((usX + font/2) > LCD_WIDTH)
		{
			usX = 0;
			usY += font;
		} 
		//�Զ���ҳ
		if ((usY + font) > LCD_HEIGTH)
		{
			usX = 0;
			usY = 0;
		}
		//��ʾ�ַ�
		TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font);
		//����λ��
		pStr ++;      
		usX += font/2;
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
static void LCD_ShowCHN(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,CHN_font_t font)
{
	uint8_t ucTemp, ucPage, ucColumn;
	uint16_t usIndex; //�ֿ��еĺ�������
	uint16_t CHN_Num; //�ֿ��еĺ�������
	
	//�����������Ƿ�Ϸ�
  assert_param(IS_ASCII_font(font));
	//�ж����� - 16������
	if(font == CHN_font_16)
	{
		//ͳ�ƺ�������
		CHN_Num = sizeof(FONT_CHN16) / sizeof(FONT_CHN16_t);
		//forѭ�����Һ���λ��
		for(usIndex=0;usIndex<CHN_Num;usIndex++)
		{
			if((FONT_CHN16[usIndex].Index[0] == *pStr) && (FONT_CHN16[usIndex].Index[1] == *(pStr+1)))
			{
				//���ô��ڣ���СΪ16x16
				LCD_SetWindows(usX,usY,16,16);
				//����д�����ݣ���16�У�ÿ��16�����ص�
				for(ucPage=0;ucPage<32;ucPage++)
				{
					//��ASCII�ַ��������ȡ��������	
					//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
						
				break; //���ҵ�����ʾ�˺��֣��˳�ѭ��
			}
		}	
	}
	
	//�ж����� - 24������
	if(font == CHN_font_24)
	{
		//ͳ�ƺ�������
		CHN_Num = sizeof(FONT_CHN24) / sizeof(FONT_CHN24_t);
		//forѭ�����Һ���λ��
		for(usIndex=0;usIndex<CHN_Num;usIndex++)
		{
			if((FONT_CHN24[usIndex].Index[0] == *pStr) && (FONT_CHN24[usIndex].Index[1] == *(pStr+1)))
			{
				//���ô��ڣ���СΪ24x24
				LCD_SetWindows(usX,usY,24,24);
				//����д�����ݣ���24�У�ÿ��24�����ص�
				for(ucPage=0;ucPage<72;ucPage++)
				{
					//��ASCII�ַ��������ȡ��������	
					//���ص�����Ϊ1ʱ��д���ַ���ɫ��Ϊ0ʱ��д�뱳����ɫ
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
						
				break; //���ҵ�����ʾ�˺��֣��˳�ѭ��
			}
		}	
	}
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
static void LCD_ShowCHNandENGstring(uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground,CHN_font_t font_CHN,ASCII_font_t font_ASCII)
{
	while (* pStr != '\0')
	{
		//�����ַ�
		if((* pStr) > 127)
		{
			//�Զ�����
			if ((usX + font_CHN) > LCD_WIDTH)
			{
				usX = 0;
				usY += font_CHN;
			} 
			//�Զ���ҳ
			if ((usY + font_CHN) > LCD_HEIGTH)
			{
				usX = 0;
				usY = 0;
			}
			//��ʾ�����ַ�
			TFT_LCD.LCD_ShowCHN(usX, usY, pStr, usColor_Background, usColor_Foreground,font_CHN);
			//����λ��
			pStr += 2;      
			usX += font_CHN;
		}
		//Ӣ���ַ�
		else
		{
			if((* pStr == '\r') | (* pStr == '\n'))
		  {
				//ǰ����ַ�Ϊ����
				if((* (pStr-1)) > 127)
				{
					//����
					usX = 0;
					usY += font_CHN;
				}
				//ǰ����ַ�ΪӢ��
				else
				{
					//����
					usX = 0;
					usY += font_ASCII;	
				}							
			}
			else
			{
				//�Զ�����
				if ((usX + font_ASCII/2) > LCD_WIDTH)
				{
					usX = 0;
					usY += font_ASCII;
				} 
				//�Զ���ҳ
				if ((usY + font_ASCII) > LCD_HEIGTH)
				{
					usX = 0;
					usY = 0;
				}
				//��ʾ�ַ�
				TFT_LCD.LCD_ShowChar(usX, usY, * pStr, usColor_Background, usColor_Foreground,font_ASCII);
				//����λ��     
				usX += font_ASCII/2;
			}
			//ָ����һ���ַ�
      pStr ++; 			
		}		
	}
}

/*
	* @name   LCD_ShowPicture
	* @brief  ��LCD��Ļ����ʾͼƬ
	* @param  usX    ����ʼX����
*           usY    ����ʼY����
*           Pic_H  ��ͼƬˮƽ�ֱ���
*           Pic_V  ��ͼƬ��ֱ�ֱ���
*           Pic_num��ͼƬ���

	* @retval None      
*/
static void LCD_ShowPicture(uint16_t usX, uint16_t usY,uint16_t Pic_H, uint16_t Pic_V,uint8_t Pic_num)
{
	//uint32_t ulIndex;
	//const uint8_t* pic = NULL;
	
	//���ô��ڣ���СΪPic_HxPic_V
	LCD_SetWindows(usX,usY,Pic_H,Pic_V);
	//��ȡͼ�������׵�ַ
	/*switch(Pic_num)
	{
		case 1: pic = gImage_Pic1; break;
		case 2: pic = gImage_Pic2; break;
		default:pic = gImage_Pic1;
	}*/

	//����д��ͼƬ����
	/*for(ulIndex=0;ulIndex<Pic_H*Pic_V*2;ulIndex+=2)
	{
		LCD_Write_DATA((pic[ulIndex]<<8) | pic[ulIndex+1]);
	}*/
}

/*
	* @name   LCD_GetPointPiexl
	* @brief  ��ȡ���ص�
	* @param  usX    ��X����
*           usY    ��Y����

  * @retval usColor :ɫ�����ݣ�RGB565��ʽ      
*/
static LCD_Color_t LCD_GetPointPiexl(uint16_t usX, uint16_t usY)  
{
	uint16_t usR=0, usG=0, usB=0;
	LCD_Color_t Color; //��ɫ�����ݸ�ʽRGB565
	
	//�ж������Ƿ���Ч
	if((usX < LCD_WIDTH) && (usY < LCD_HEIGTH))
	{
		//���ô��ڣ���СΪ1x1
		LCD_SetWindows(usX,usY,1,1);
		//д���GRAM����
		LCD_Write_CMD(LCD_CMD_RDgram);
		//��ʼ������
		usR = LCD_Read_DATA();  //��Dummy���ݣ�����	
		usR = LCD_Read_DATA();  //��RED����
		usB = LCD_Read_DATA();  //��BLUE����
		usG = LCD_Read_DATA();  //��GREEN����		
		//����ת��ΪRGB565
		Color = (LCD_Color_t)(((usR>>11)<<11) | ((usG>>10)<<5) | (usB>>11));
		printf("The Pixel color is 0x%.4X\r\n",Color);
	}
	//������ɫ����
	return	Color;
}	

/*
	* @name   LCD_SetPointPiexl
	* @brief  �������ص�
	* @param  usX     ��X����
*           usY     ��Y����
*           usColor ����ɫ
  * @retval none 
*/
static void  LCD_SetPointPiexl(uint16_t usX, uint16_t usY, LCD_Color_t usColor)              
{
	//�ж������Ƿ���Ч
	if((usX < LCD_WIDTH) && (usY < LCD_HEIGTH))
	{
		//���ô��ڣ���СΪ1x1
		LCD_SetWindows(usX,usY,1,1);
		//д����ɫ����
		LCD_Write_DATA(usColor);
	}
}

/*
	* @name   LCD_DrawLine
	* @brief  ��ֱ��
	* @param  usX_Start ��X�������
*           usY_Start ��Y�������
*           usX_End   ��X�յ�����
*           usY_End   ��Y�յ�����
*           usColor   ����ɫ
  * @retval none 
*/
static void LCD_DrawLine(uint16_t usX_Start,uint16_t usY_Start,uint16_t usX_End,uint16_t usY_End,LCD_Color_t usColor)      
{
	uint16_t i; 
	//uint16_t k;
	uint16_t usX_Current, usY_Current;
	int16_t sError_X=0,sError_Y=0,sDelta_X,sDelta_Y,sDistance; 
	int16_t sIncrease_X, sIncrease_Y;
	
	/* ������������ */
	sDelta_X = usX_End - usX_Start; 
	sDelta_Y = usY_End - usY_Start; 
	usX_Current = usX_Start; 
	usY_Current = usY_Start; 
	
	if(sDelta_X > 0)
  {
    /* ���õ������� */
    sIncrease_X=1; 
	}
  else if(sDelta_X == 0)
  {
    /* ��ֱ�� */
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
    /* ˮƽ��  */
		sIncrease_Y = 0;
	}
	else
  {
    sIncrease_Y = -1;
    sDelta_Y    = -sDelta_Y;
  }
	
	if(sDelta_X > sDelta_Y)
  {
    /* ѡȡ�������������� */
		sDistance = sDelta_X;  
	}
	else
  {
		sDistance = sDelta_Y; 
  }
	
	//�������
	for(i=0;i<=sDistance+1;i++) 
	{
    //����
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
		
		//��̬������Ч��
		//for(k=0;k<20000;k++);
	}
}

/*
	* @name   LCD_DrawRectangle
	* @brief  ������
	* @param  usX_Start ��X��ʼ����
*           usY_Start ��Y��ʼ����
*           usWidth   �����εĿ��
*           usHeight  �����εĸ߶� 
*           usColor   �����ε���ɫ
*           ucFilled  ��ѡ���Ƿ�������
							�����FFilled   �����
*                   unFilled �������
  * @retval none 
*/
static void LCD_DrawRectangle(uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, LCD_Color_t usColor, Filled_t ucFilled)
{
	//�������
	if(ucFilled == Filled)
	{
		TFT_LCD.FillColor(usX_Start, usY_Start, usWidth, usHeight, usColor);
	}
	//���β����
	else
	{
		//��������
		TFT_LCD.LCD_DrawLine(usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start + usWidth - 1, usY_Start + usHeight - 1, usX_Start, usY_Start + usHeight - 1, usColor);
		TFT_LCD.LCD_DrawLine(usX_Start, usY_Start + usHeight - 1, usX_Start, usY_Start, usColor);
	}
}

/*
	* @name   LCD_DrawCircle
	* @brief  ��LCD��Ļ��ʹ�� Bresenham �㷨��Բ
	* @param  usX_Center ��Բ��X����
*           usY_Center ��Բ��Y����
*           usRadius   ��Բ�İ뾶(��λ����)
*           usColor    �����ε���ɫ
*           ucFilled   ��ѡ���Ƿ����Բ��
							�����FFilled   �����
*                   unFilled �������
  * @retval none 
*/
static void LCD_DrawCircle(uint16_t usX_Center,uint16_t usY_Center,uint16_t usRadius,LCD_Color_t usColor,Filled_t ucFilled)            
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
  
	sCurrentX = 0;
  sCurrentY = usRadius;	
	sError    = 3- (usRadius<<1);   //�ж��¸���λ�õı�־
	
	while(sCurrentX <= sCurrentY)
	{
		int16_t sCountY;		
		if(ucFilled)
    {			
			for(sCountY=sCurrentX;sCountY<=sCurrentY;sCountY++)
			{                      
				TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center + sCountY, usColor);     //1���о����� 
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
			TFT_LCD.LCD_SetPointPiexl(usX_Center + sCurrentX, usY_Center + sCurrentY, usColor);     //1���о�����
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
