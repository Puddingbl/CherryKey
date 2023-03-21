
/* Includes ------------------------------------------------------------------*/
#include "run.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "public.h"
#include "key.h"
#include "adc.h"

uint16_t cont1 = 0;
uint32_t adcValue = 0;
	
void run(void) {
	if (key_get_event(&key2) == CLICK) {
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_SET);  // rgb灯关闭绿色
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_SET);  // rgb灯关闭红色
	}
	
	if ((key_get_event(&key2) == LONG_PRESS) && (key_get_event(&key3) == LONG_PRESS)) {
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_RESET);  // rgb灯绿色
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_RESET);  // rgb灯红色
	}	
}

/*
	* @name   HAL_TIM_PeriodElapsedCallback
	* @brief  定时器中断回调函数
	* @param  *htim -> 处理定时器的结构体指针
	* @retval None    
	* @ ElapsedCallback是触发回调，用这个
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == htim6.Instance) {	
		if (cont1%10 == 0) {
			key_callback_handle();
		}			
		if (cont1 == 1000) {
//			printf("U=0x%x\n", adcValue);				
			cont1 = 0;
		}	
		my_tick_inc(1);  // 滴答
		key_detect_scan();
		HAL_ADC_Start_IT(&hadc1);			 // 启动ADC中断	
	}
	cont1++;
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	HAL_ADC_Stop_IT(hadc);
	adcValue = HAL_ADC_GetValue(hadc);
	key_adc_update(&KeyADCPort, adcValue); 
}