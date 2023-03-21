/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATICLINKLIST_H
#define __STATICLINKLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"

/* define---------------------------------------------------------------------*/
typedef     void*     sLinkType; 

typedef struct
{
	sLinkType 	data;
	uint16_t 		cur;
} sLink_t;

typedef struct
{
	sLink_t	  *sll;
	uint16_t  max_size;
} staticll_t;

/* extern variables-----------------------------------------------------------*/

/* exported functions ------------------------------------------------------- */
uint8_t slink_init(staticll_t *slink, sLink_t *sll, uint16_t size);
uint16_t slink_malloc(staticll_t *slink);
void slink_free(staticll_t *slink, uint16_t k);
uint16_t slink_length(staticll_t *slink);
uint8_t slink_insert(staticll_t *slink, uint16_t i, sLinkType e);
uint8_t slink_delete(staticll_t *slink, uint16_t i);
sLinkType slink_getData(staticll_t *slink, uint16_t k);

#ifdef __cplusplus
}
#endif

#endif
