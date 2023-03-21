
#include "ov7670.h"
#include "dcmi.h"



////////////////////////////
//功能：写OV7670寄存器
//返回：1-成功	0-失败
unsigned char wrOV7670Reg(unsigned char regID, unsigned char regDat)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x42, regID, I2C_MEMADD_SIZE_8BIT, &regDat, 1,200);
	
  return(1);
}
////////////////////////////
//功能：读OV7670寄存器
//返回：1-成功	0-失败
unsigned char rdOV7670Reg(unsigned char regID, unsigned char *regDat)
{
	HAL_I2C_Mem_Read(&hi2c1, 0x42, regID, I2C_MEMADD_SIZE_8BIT, regDat, 1, 10000);
	
  return(1);
}


//(140,16,640,480) is good for VGA
//(272,16,320,240) is good for QVGA
/* config_OV7670_window */
void OV7670_config_window(unsigned int startx,unsigned int starty,unsigned int width, unsigned int height)
{
	unsigned int endx;
	unsigned int endy;// "v*2"必须
	unsigned char temp_reg1, temp_reg2;
	unsigned char temp=0;
	
	endx=(startx+width);
	endy=(starty+height+height);// "v*2"必须
  rdOV7670Reg(0x03, &temp_reg1 );
	temp_reg1 &= 0xf0;
	rdOV7670Reg(0x32, &temp_reg2 );
	temp_reg2 &= 0xc0;
	
	// Horizontal
	temp = temp_reg2|((endx&0x7)<<3)|(startx&0x7);
	wrOV7670Reg(0x32, temp );
	temp = (startx&0x7F8)>>3;
	wrOV7670Reg(0x17, temp );
	temp = (endx&0x7F8)>>3;
	wrOV7670Reg(0x18, temp );
	
	// Vertical
	temp =temp_reg1|((endy&0x3)<<2)|(starty&0x3);
	wrOV7670Reg(0x03, temp );
	temp = starty>>2;
	wrOV7670Reg(0x19, temp );
	temp = endy>>2;
	wrOV7670Reg(0x1A, temp );
}

//
void set_OV7670reg(void)
{
	wrOV7670Reg(0x8c, 0x00);
	wrOV7670Reg(0x3a, 0x04);
	wrOV7670Reg(0x40, 0xd0);
	wrOV7670Reg(0x8c, 0x00);
	wrOV7670Reg(0x12, 0x14);
	wrOV7670Reg(0x32, 0x80);
	wrOV7670Reg(0x17, 0x16);
	wrOV7670Reg(0x18, 0x04);
	wrOV7670Reg(0x19, 0x02);
	wrOV7670Reg(0x1a, 0x7b);
	wrOV7670Reg(0x03, 0x06);
	wrOV7670Reg(0x0c, 0x04);
	wrOV7670Reg(0x3e, 0x00);
	wrOV7670Reg(0x70, 0x3a);
	wrOV7670Reg(0x71, 0x35); 
	wrOV7670Reg(0x72, 0x11); 
	wrOV7670Reg(0x73, 0x00);
	wrOV7670Reg(0xa2, 0x00);
	wrOV7670Reg(0x11, 0xff); 
	//wrOV7670Reg(0x15 , 0x31);
	wrOV7670Reg(0x7a, 0x20); 
	wrOV7670Reg(0x7b, 0x1c); 
	wrOV7670Reg(0x7c, 0x28); 
	wrOV7670Reg(0x7d, 0x3c);
	wrOV7670Reg(0x7e, 0x55); 
	wrOV7670Reg(0x7f, 0x68); 
	wrOV7670Reg(0x80, 0x76);
	wrOV7670Reg(0x81, 0x80); 
	wrOV7670Reg(0x82, 0x88);
	wrOV7670Reg(0x83, 0x8f);
	wrOV7670Reg(0x84, 0x96); 
	wrOV7670Reg(0x85, 0xa3);
	wrOV7670Reg(0x86, 0xaf);
	wrOV7670Reg(0x87, 0xc4); 
	wrOV7670Reg(0x88, 0xd7);
	wrOV7670Reg(0x89, 0xe8);
	 
	wrOV7670Reg(0x32,0xb6);
	
	wrOV7670Reg(0x13, 0xff); 
	wrOV7670Reg(0x00, 0x00);
	wrOV7670Reg(0x10, 0x00);
	wrOV7670Reg(0x0d, 0x00);
	wrOV7670Reg(0x14, 0x4e);
	wrOV7670Reg(0xa5, 0x05);
	wrOV7670Reg(0xab, 0x07); 
	wrOV7670Reg(0x24, 0x75);
	wrOV7670Reg(0x25, 0x63);
	wrOV7670Reg(0x26, 0xA5);
	wrOV7670Reg(0x9f, 0x78);
	wrOV7670Reg(0xa0, 0x68);
//	wrOV7670Reg(0xa1, 0x03);//0x0b,
	wrOV7670Reg(0xa6, 0xdf);
	wrOV7670Reg(0xa7, 0xdf);
	wrOV7670Reg(0xa8, 0xf0); 
	wrOV7670Reg(0xa9, 0x90); 
	wrOV7670Reg(0xaa, 0x94); 
	//wrOV7670Reg(0x13, 0xe5); 
	wrOV7670Reg(0x0e, 0x61);
	wrOV7670Reg(0x0f, 0x43);
	wrOV7670Reg(0x16, 0x02); 
	wrOV7670Reg(0x1e, 0x37);
	wrOV7670Reg(0x21, 0x02);
	wrOV7670Reg(0x22, 0x91);
	wrOV7670Reg(0x29, 0x07);
	wrOV7670Reg(0x33, 0x0b);
	wrOV7670Reg(0x35, 0x0b);
	wrOV7670Reg(0x37, 0x3f);
	wrOV7670Reg(0x38, 0x01);
	wrOV7670Reg(0x39, 0x00);
	wrOV7670Reg(0x3c, 0x78);
	wrOV7670Reg(0x4d, 0x40);
	wrOV7670Reg(0x4e, 0x20);
	wrOV7670Reg(0x69, 0x00);
	wrOV7670Reg(0x6b, 0x4a);
	wrOV7670Reg(0x74, 0x19);
	wrOV7670Reg(0x8d, 0x4f);
	wrOV7670Reg(0x8e, 0x00);
	wrOV7670Reg(0x8f, 0x00);
	wrOV7670Reg(0x90, 0x00);
	wrOV7670Reg(0x91, 0x00);
	wrOV7670Reg(0x92, 0x00);
	wrOV7670Reg(0x96, 0x00);
	wrOV7670Reg(0x9a, 0x80);
	wrOV7670Reg(0xb0, 0x84);
	wrOV7670Reg(0xb1, 0x0c);
	wrOV7670Reg(0xb2, 0x0e);
	wrOV7670Reg(0xb3, 0x82);
	wrOV7670Reg(0xb8, 0x0a);
	wrOV7670Reg(0x43, 0x14);
	wrOV7670Reg(0x44, 0xf0);
	wrOV7670Reg(0x45, 0x34);
	wrOV7670Reg(0x46, 0x58);
	wrOV7670Reg(0x47, 0x28);
	wrOV7670Reg(0x48, 0x3a);
	
	wrOV7670Reg(0x59, 0x88);
	wrOV7670Reg(0x5a, 0x88);
	wrOV7670Reg(0x5b, 0x44);
	wrOV7670Reg(0x5c, 0x67);
	wrOV7670Reg(0x5d, 0x49);
	wrOV7670Reg(0x5e, 0x0e);
	
	wrOV7670Reg(0x64, 0x04);
	wrOV7670Reg(0x65, 0x20);
	wrOV7670Reg(0x66, 0x05);

	wrOV7670Reg(0x94, 0x04);
	wrOV7670Reg(0x95, 0x08);	 

	wrOV7670Reg(0x6c, 0x0a);
	wrOV7670Reg(0x6d, 0x55);
	wrOV7670Reg(0x6e, 0x11);
	wrOV7670Reg(0x6f, 0x9f); 

	wrOV7670Reg(0x6a, 0x40);
	wrOV7670Reg(0x01, 0x40);
	wrOV7670Reg(0x02, 0x40);
	
	//wrOV7670Reg(0x13, 0xe7);
	wrOV7670Reg(0x15, 0x00);
	wrOV7670Reg(0x4f, 0x80);
	wrOV7670Reg(0x50, 0x80);
	wrOV7670Reg(0x51, 0x00);
	wrOV7670Reg(0x52, 0x22);
	wrOV7670Reg(0x53, 0x5e);
	wrOV7670Reg(0x54, 0x80);
	wrOV7670Reg(0x58, 0x9e);

	wrOV7670Reg(0x41, 0x08);
	wrOV7670Reg(0x3f, 0x00);
	wrOV7670Reg(0x75, 0x05);
	wrOV7670Reg(0x76, 0xe1);

	wrOV7670Reg(0x4c, 0x00);
	wrOV7670Reg(0x77, 0x01);
	
	wrOV7670Reg(0x3d, 0xc1);
	wrOV7670Reg(0x4b, 0x09);
	wrOV7670Reg(0xc9, 0x60);
	//wrOV7670Reg(0x41, 0x38);	
	wrOV7670Reg(0x56, 0x40);
	wrOV7670Reg(0x34, 0x11);
	wrOV7670Reg(0x3b, 0x02);
	wrOV7670Reg(0xa4, 0x89);
	
	wrOV7670Reg(0x96, 0x00);
	wrOV7670Reg(0x97, 0x30);
	wrOV7670Reg(0x98, 0x20);
	wrOV7670Reg(0x99, 0x30);
	wrOV7670Reg(0x9a, 0x84);
	wrOV7670Reg(0x9b, 0x29);
	wrOV7670Reg(0x9c, 0x03);
	wrOV7670Reg(0x9d, 0x4c);
	wrOV7670Reg(0x9e, 0x3f);	

	wrOV7670Reg(0x09, 0x00);
	wrOV7670Reg(0x3b, 0xc2);

}

/* OV7670_init() */
//返回1成功，返回0失败
unsigned char OV7670_init(void)
{
	unsigned char temp = 0x80;	
	uint8_t OV7670_verh, OV7670_verl ;
	
	HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_RESET);  //0：工作 1：POWER DOWN
	HAL_GPIO_WritePin(DCMI_REST_GPIO_Port, DCMI_REST_Pin, GPIO_PIN_SET);  //0：RESET 模式 1：一般模式
	
	rdOV7670Reg(0x0a, &OV7670_verh);
	rdOV7670Reg(0x0b, &OV7670_verl);
	
	printf("OV7670_verh=%#x, OV7670_verl=%#x\n", OV7670_verh, OV7670_verl);
	
//	if(0==wrOV7670Reg(0x12, temp)) //Reset SCCB
//	{
//        return 0 ;
//	}
	
	HAL_Delay(100);
//  set_OV7670reg();
//	OV7670_config_window(272,12,320,240);// set 240*320
	
	return 0x01; //ok			  
} 

void ov7670_work()
{
	
	
}

void  HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	
}

void  HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	
}



uint8_t *JpegBuffer;
 void StartOV2640()
 {
     __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);                    // 使用帧中断
     HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)JpegBuffer, 65536); // 启动拍照 
 }

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
//	 HAL_DCMI_Suspend(hdcmi); // 拍照完成，挂起 DCMI
//	 HAL_DCMI_Stop(hdcmi);    // 拍照完成，停止 DMA传输	
//	
//	 uint16_t i, jpgstart, jpglen = 0;
//	 uint8_t *jpgp = (uint8_t *)JpegBuffer;
//	 uint8_t headok = 0;
//	 for (i = 0; i < 65535; i++) // 查找 0xFF, 0xD8, 0xFF 和 0xFF, 0xD9, 获取 jpg 文件大小
//	 {
//			 if ((jpgp[i] == 0XFF) && (jpgp[i + 1] == 0XD8) && (jpgp[i + 2] == 0xFF) && (!headok)) // 找到 FF D8 FF
//			 {
//					 jpgstart = i;
//					 headok = 1; // 标记找到 jpg 头
//			 }
//			 if ((jpgp[i] == 0XFF) && (jpgp[i + 1] == 0XD9) && headok) // 找到头以后,再找 FF D9
//			 {
//					 jpglen = i - jpgstart + 2;
//					 break;
//			 }
//	 }	
//	 jpgp += jpgstart; 
}
