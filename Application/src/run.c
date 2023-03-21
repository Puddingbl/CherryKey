
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
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_SET);  // rgb�ƹر���ɫ
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_SET);  // rgb�ƹرպ�ɫ
	}
	
	if ((key_get_event(&key2) == LONG_PRESS) && (key_get_event(&key3) == LONG_PRESS)) {
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_RESET);  // rgb����ɫ
		HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_RESET);  // rgb�ƺ�ɫ
	}	
}

/*
	* @name   HAL_TIM_PeriodElapsedCallback
	* @brief  ��ʱ���жϻص�����
	* @param  *htim -> ����ʱ���Ľṹ��ָ��
	* @retval None    
	* @ ElapsedCallback�Ǵ����ص��������
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
		my_tick_inc(1);  // �δ�
		key_detect_scan();
		HAL_ADC_Start_IT(&hadc1);			 // ����ADC�ж�	
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