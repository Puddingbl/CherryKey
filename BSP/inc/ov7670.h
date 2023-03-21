/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __OV7670_H
#define __OV7670_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "public.h"
#include "i2c.h"

/* define---------------------------------------------------------------------*/
#define OV7670_RRST_H   HAL_GPIO_WritePin(DM_RESET_GPIO_Port, DM_RESET_Pin, GPIO_PIN_SET)
#define OV7670_RRST_L   HAL_GPIO_WritePin(DM_RESET_GPIO_Port, DM_RESET_Pin, GPIO_PIN_RESET)


#define OV7670_WRST_H   GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define OV7670_WRST_L   GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define OV7670_WEN_H   GPIO_SetBits(GPIOG,GPIO_Pin_5)
#define OV7670_WEN_L   GPIO_ResetBits(GPIOG,GPIO_Pin_5)



/* extern variables-----------------------------------------------------------*/

/* exported functions ------------------------------------------------------- */
unsigned char OV7670_init(void);


#ifdef __cplusplus
}
#endif

#endif
