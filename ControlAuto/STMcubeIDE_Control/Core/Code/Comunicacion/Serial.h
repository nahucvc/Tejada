/*
 * Serial.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */

#ifndef CODE_COMUNICACION_SERIAL_H_
#define CODE_COMUNICACION_SERIAL_H_
#include "main.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
 extern "C" {
#endif


 bool parseCmdVal(uint8_t *buf, size_t len, char &outCmd, float &outVal);
 void loopCom();


#ifdef __cplusplus
 }
#endif

#endif /* CODE_COMUNICACION_SERIAL_H_ */
