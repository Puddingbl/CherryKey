/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_NORFLASH_H
#define __SPI_NORFLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "usart.h"


/* define---------------------------------------------------------------------*/
////宏定义
//CS管脚控制
#define  	CLR_SPI_Flash_CS	 HAL_GPIO_WritePin(spi2_cs_GPIO_Port,spi2_cs_Pin,GPIO_PIN_RESET)
#define  	SET_SPI_Flash_CS	 HAL_GPIO_WritePin(spi2_cs_GPIO_Port,spi2_cs_Pin,GPIO_PIN_SET)

//Flash命令
#define  	W25X_WriteEnable		    0x06 
#define  	W25X_WriteDisable				0x04 
#define  	W25X_ReadStatusReg1  		0x05 
#define  	W25X_ReadData			  		0x03 
#define  	W25X_PageProgram		  	0x02 
#define  	W25X_SectorErase		  	0x20 
#define  	W25X_ChipErase			  	0xC7 
#define  	W25X_ReadJedecID     		0x9F 

#define  	SPI_FLASH_PageSize   		256
#define  	Flash_Status1_BUSY      0x01  //忙碌标志位
#define  	Dummy_Byte           		0xFF  //假数据

//定义枚举类型

//定义结构体类型
typedef struct
{
  uint32_t  JedecID;	                                //设备标识符 -> 制造商+内存类型+容量
	
	void (*ReadJedecID)(void);                          //读取设备标识符
	void (*EraseSector)(uint32_t);                      //擦除扇区(4kB)
	void (*EraseTotal)(void);                           //擦除全部
  void (*WriteUnfixed)(uint8_t*,uint32_t,uint32_t);   //写入不固定长度数据
	void (*ReadUnfixed)(uint8_t*,uint32_t,uint32_t);    //读取不固定长度数据
} SPI_Flash_t;


/* extern variables-----------------------------------------------------------*/
extern SPI_Flash_t  SPI_Flash ;

/* exported functions ------------------------------------------------------- */
void SPI_Flash_writeReg(void);

#ifdef __cplusplus
}
#endif

#endif
