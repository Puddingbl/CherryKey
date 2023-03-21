

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
	/*key��ʼ��*/
	key_init();
	key_demo_init();
		
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_RESET);  //rgb����ʾ��ɫ
	HAL_TIM_Base_Start_IT(&htim6); //������ʱ��6
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);  // adcУ׼
	

	printf("start\n");
}

