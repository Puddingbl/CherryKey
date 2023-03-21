
#include "CST816T.h"
#include "i2c.h"



const uint16_t POINT_COLOR_TBL[CTP_MAX_TOUCH]={RED,GREEN}; //触摸笔颜色
const uint16_t TPX[]={0x03,0x09}; //触摸地址
cst816t_t tp_dev=
{
 CST816S_Init,
 CST816S_Scan,
 0,
 0,	
 0,
};


//写数据
uint8_t CST816S_WR_DATA(uint16_t addr,uint8_t data)
{
	uint8_t pdata[1];
	
	pdata[0] = data;
	HAL_I2C_Mem_Write(&hi2c1, CST_WR_CMD, addr, I2C_MEMADD_SIZE_8BIT, pdata, 1,200);
	
//	uint8_t reg;
//	reg=SUCCESS;
	return SUCCESS;
}

//读数据
uint8_t CST816S_RD_DATA(uint16_t addr,uint8_t len,uint8_t *value)
{
	
	HAL_I2C_Mem_Read(&hi2c1, CST_RD_CMD, addr, I2C_MEMADD_SIZE_8BIT, value, len, 10000);
	
//	uint8_t reg=ERROR,i;
//	reg=SUCCESS;
	return SUCCESS;
}


void  CST816S_Init(void)
{
	uint8_t ID;
	
	CTP_RES_Clr();
//  CTP_INT_Clr();
	HAL_Delay(50);
//	CTP_INT_Set();
//	HAL_Delay(50);
	CTP_RES_Set();
  HAL_Delay(10);
	CST816S_WR_DATA(0xfe, 0x00);  //0：不会自动进入低功耗；1：自动进入低功耗
	CST816S_WR_DATA(0xfb, 0x00);  //0：不会复位；1：长按x秒自动复位
//  CTP_INT_IN();	
	HAL_Delay(5);
	CST816S_RD_DATA(CST816S_ID,1,&ID);
	printf("CST816T-id:%d\r\n", ID);
}


uint8_t CST816S_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	uint8_t tempsta;
 	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{ 
		CST816S_RD_DATA(GT_ADDR_REG,1,&mode);
		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;	
			CST816S_WR_DATA(GT_ADDR_REG,temp);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[1]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[1]=tp_dev.y[0];
			for(i=0;i<2;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					CST816S_RD_DATA(TPX[i],4,buf);
					printf("x=0x%x%x, y=0x%x%x\n", *buf, *(buf+1), *(buf+2), *(buf+3));
          if(USE_HORIZONTAL==2)//横屏
					{
						tp_dev.y[i]=buf[1];
						tp_dev.x[i]=buf[3];
					}
					else if(USE_HORIZONTAL==0)
					{
						tp_dev.x[i]=buf[1];
						tp_dev.y[i]=buf[3];
					} 
					else if(USE_HORIZONTAL==1)
					{
						tp_dev.x[i]=buf[1];
						tp_dev.y[i]=240-buf[3];
					} 
					else 
					{
						tp_dev.y[i]=240-buf[1];
						tp_dev.x[i]=240-buf[3];
					} 					
					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]>LCD_W||tp_dev.y[0]>LCD_H)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)  
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	else//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
			tp_dev.x[0]=0;
			tp_dev.y[0]=0;
			tp_dev.sta&=0XE0;	//清除点有效标记
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0;
			tp_dev.y[0]=0;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}



void CST816S_test(void)
{
	uint8_t t=0; 	    
 	uint16_t lastpos[2][2];		//最后一次的数据 
	
	lcd1.LCD_FillColor(&lcd1, 0,0,LCD_W,LCD_H,WHITE);
	while(1)
	{   
			
		tp_dev.scan(0);
		for(t=0;t<CTP_MAX_TOUCH;t++)
		{
			if((tp_dev.sta&(1<<t)))//判断是否有点触摸？
			{    
				HAL_Delay(1);
				if(tp_dev.x[t]<LCD_W&&tp_dev.y[t]<LCD_H)//在LCD范围内
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd1.LCD_DrawLine(&lcd1, lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],POINT_COLOR_TBL[t]);
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
				}
			}else {lastpos[t][0]=0XFFFF;}
		}
		HAL_Delay(5);
	}	
}

/**
	@ cst816t_getADC
	@ 获取x\y的ADC值
*/
uint8_t cst816t_getADC(uint16_t *xValue, uint16_t *yValue)
{
	uint8_t touch;
	uint8_t coordinate[4] = {0};
	
	CST816S_RD_DATA(FingerNum,1, &touch);
//	printf("touch=%d\n", touch);
//	HAL_Delay(200);
	if(touch == 1) 															//touch = 0:未按下; 1:按下
	{
		CST816S_RD_DATA(XposH,4,coordinate);  //由x高位开始读，自动增加地址
		*xValue = (coordinate[0]&0x0f << 8 | coordinate[1]);
		*yValue = (coordinate[2]&0x0f << 8 | coordinate[3]);
//		printf("x[0]:%d,y[0]:%d\r\n", *xValue, *yValue);
		return 1;
	}	
	else
		return 0;
		
}
