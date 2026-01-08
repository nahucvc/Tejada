/*
 * UserMain.cpp
 *
 *  Created on: 18 dic 2025
 *      Author: Nahuel
 */

#include "UserMain.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim8;
extern DAC_HandleTypeDef hdac1;
extern COMP_HandleTypeDef hcomp1;
volatile uint32_t adcBuffer[3];
volatile uint8_t BufferTX[33] = { 0 };

void setup(void) {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcBuffer, 3);
	//HAL_UART_Transmit_DMA(&huart1, (uint8_t*) BufferTX, 33);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 868);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_TIM_Base_Start(&htim8);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1);
	HAL_COMP_Start(&hcomp1);
	__HAL_TIM_MOE_ENABLE(&htim8);
	__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_BREAK);
	HAL_TIM_Base_Start(&htim8);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1);

}

void loop(void) {
	loop_break_control();
}

#ifdef __cplusplus
}
#endif
