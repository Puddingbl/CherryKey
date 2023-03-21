/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_QUEUE_H
#define __MY_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"

/* define---------------------------------------------------------------------*/

#define Queue_Size     30

typedef enum 
{
	Q_ERROR = -1,
	Q_OK    = 0
}	Q_Status;

typedef  void*     QElemType;

typedef struct
{
	uint8_t front;         // 已读数据指向
	uint8_t rear;    			 // 已收数据指向
	uint8_t count;     		 // 总共有多少数据
	uint8_t size;
	QElemType *data;    
} Queue_t;

/* exported functions ------------------------------------------------------- */
Q_Status queue_init(Queue_t *q, QElemType *pArray, uint8_t size);
uint8_t  queue_length(Queue_t q);
void queue_clear(Queue_t *q);
Q_Status queue_insert(Queue_t *q, QElemType e);
Q_Status queue_extract(Queue_t *q, QElemType *e);


#ifdef __cplusplus
}
#endif

#endif
