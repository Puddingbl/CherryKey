#ifndef __FILTER_H__
#define __FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "main.h"


/* define---------------------------------------------------------------------*/
typedef int filter_type;  





/* extern variables-----------------------------------------------------------*/


/* exported functions ------------------------------------------------------- */
filter_type MedianFilter(filter_type* data,int size);

#ifdef __cplusplus
}
#endif

#endif