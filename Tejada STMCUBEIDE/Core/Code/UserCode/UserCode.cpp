/*
 * UserCode.cpp
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */
#include "UserCode.h"
#include "SerialUSB.h"
#include "Serial.h"
uint8_t buffer[60];
extern ADC_HandleTypeDef hadc1;
uint32_t ValorADC;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern DAC_HandleTypeDef hdac1;

void setup()
{
 HAL_UART_Receive_DMA(&huart1, buffer, 60);
 HAL_ADC_Start_DMA(&hadc1,&ValorADC, 1);
 HAL_TIM_Base_Start_IT(&htim8); //5k
 HAL_TIM_Base_Start_IT(&htim7);// 1k
 HAL_DAC_Start(&hdac1,DAC_CHANNEL_1 );

}

void loop()
{

loopCom();
Serial.service();

}


