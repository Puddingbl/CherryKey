

/* Includes ------------------------------------------------------------------*/
#include "init.h"

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "adc.h"
#include "stdio.h"
#include <string.h>
#include "public.h"
#include "stdio.h"
#include "key.h"
#include "key_demo.h"


void init(void) {
	/*key初始化*/
	key_init();
	key_demo_init();
		
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_RESET);  //rgb灯显示绿色
	HAL_TIM_Base_Start_IT(&htim6); //启动定时器6
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);  // adc校准
	

	printf("start\n");
}

