/*
 * UserCode.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */

#ifndef CODE_USERCODE_USERCODE_H_
#define CODE_USERCODE_USERCODE_H_
#include "main.h"


#ifdef __cplusplus
 extern "C" {
#endif
extern uint8_t buffer[60];
void setup();
void loop();
extern UART_HandleTypeDef huart1;
extern uint32_t ValorADC;
#ifdef __cplusplus
 }
#endif
#endif /* CODE_USERCODE_USERCODE_H_ */
