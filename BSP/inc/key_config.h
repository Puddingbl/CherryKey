#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define KEY_DEBUG  0
#if KEY_DEBUG
	#define KEY_TRACE(format, ...)  DEBUG_TRACE(format, __VA_ARGS__)  //同时打印文件名、函数名、行
	#define KEY_LOG(format, ...)    DEBUG_LOG(format, ##__VA_ARGS__)		//普通printf
	#else
	#define KEY_TRACE(format, ...)
	#define KEY_LOG(format, arg...)
#endif

#define key_port    													 void                               //第几组引脚
#define key_pin 															 uint16_t														//第几个引脚

/* 定期执行时间 */
#define KEY_TIME_BASE     1   //ms  

#define 	KEY_NUMBER    	  6    // 按键数量
#define   KEY_ADC_NUM				4		 // ADC按键数量
#define   KEY_CB_NUM			  6    // 每个按键可注册的回调函数数量
#define   KEY_QUEUE_LEN     50	 // 队列长度


#ifdef __cplusplus
}
#endif

#endif
