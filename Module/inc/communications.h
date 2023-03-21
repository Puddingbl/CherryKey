/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMUNICATIONS_H
#define __COMMUNICATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f421.h"
#include "my_structure.h"
#include "public.h"

/* define---------------------------------------------------------------------*/

/******************************************************************************/

typedef struct
{
	void (*protocol_parse)(Queue_t *q, uint8_t);	
	
	Queue_t rx_buff;
	Queue_t info;
} Com_General_Queue_t;


/* exported functions ------------------------------------------------------- */

extern Com_General_Queue_t com_usart1;
extern uint8_t test_buff[50];
extern float id_vaul;

void com_module_init(void);
void com_general_handle(Com_General_Queue_t *q);
void usart1_dmaTransmit(uint16_t *buff, uint8_t lenght);

#ifdef __cplusplus
}
#endif

#endif
