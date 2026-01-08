/*
 * UserMain.h
 *
 *  Created on: 18 dic 2025
 *      Author: Nahuel
 */

#ifndef USERMAIN_H_
#define USERMAIN_H_
#include "main.h"
#include "control.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DATOS_CONTROL_SIZE sizeof(struct Datos_Control)
struct __attribute__((packed)) Datos_Control
{
	float voltaje_referencia;
	float voltaje_ADC;
	float corriente_maxima;
	float coriente_ADC;
	float voltaje_bateria;
	float error;
	uint32_t duty;
	uint32_t CRCdata;
};



void setup(void);

void loop(void);

extern volatile uint32_t adcBuffer[3];
extern volatile uint8_t BufferTX[33];
extern TIM_HandleTypeDef htim8;
#ifdef __cplusplus
}
#endif


#endif /* USERMAIN_H_ */
