#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define KEY_DEBUG  0
#if KEY_DEBUG
	#define KEY_TRACE(format, ...)  DEBUG_TRACE(format, __VA_ARGS__)  //ͬʱ��ӡ�ļ���������������
	#define KEY_LOG(format, ...)    DEBUG_LOG(format, ##__VA_ARGS__)		//��ͨprintf
	#else
	#define KEY_TRACE(format, ...)
	#define KEY_LOG(format, arg...)
#endif

#define key_port    													 void                               //�ڼ�������
#define key_pin 															 uint16_t														//�ڼ�������

/* ����ִ��ʱ�� */
#define KEY_TIME_BASE     1   //ms  

#define 	KEY_NUMBER    	  6    // ��������
#define   KEY_ADC_NUM				4		 // ADC��������
#define   KEY_CB_NUM			  6    // ÿ��������ע��Ļص���������
#define   KEY_QUEUE_LEN     50	 // ���г���


#ifdef __cplusplus
}
#endif

#endif
