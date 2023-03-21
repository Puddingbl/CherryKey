/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __PUBLIC_H
#define __PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "usart.h"
#include "stdio.h"

/* define---------------------------------------------------------------------*/

/* DEBUG专用------------------------------------------------------*/
#define DEBUG 1
#if DEBUG
#define DEBUG_TRACE(format, ...) printf("Info:[%s:%s(%d)]:" format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)  //同时打印文件名、函数名、行
#define DEBUG_LOG(format, ...) printf(format, ##__VA_ARGS__)																															 //普通printf
#else
#define DEBUG_TRACE(format, ...)
#define DEBUG_LOG(format, arg...)
#endif

//定义枚举类型 -> TRUE/FALSE位
typedef enum 
{
  FALSE = 0U, 
  TRUE = !FALSE
} FlagStatus_t;


//定义枚举类型 -> BIT位
typedef enum
{
	BIT0 = (uint8_t)(0x01 << 0),  
	BIT1 = (uint8_t)(0x01 << 1),  
	BIT2 = (uint8_t)(0x01 << 2),  
	BIT3 = (uint8_t)(0x01 << 3),  
	BIT4 = (uint8_t)(0x01 << 4),
	BIT5 = (uint8_t)(0x01 << 5),
	BIT6 = (uint8_t)(0x01 << 6),
	BIT7 = (uint8_t)(0x01 << 7),
}BIT_t;


/* exported functions ------------------------------------------------------- */
void my_tick_inc(uint32_t tick_period);
uint32_t my_tick_get(void);
uint32_t my_tick_elaps(uint32_t prev_tick);

/* extern variables-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
