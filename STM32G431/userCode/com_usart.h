/*
 * com_usart.h
 *
 *  Created on: Nov 24, 2024
 *      Author: Nahuel
 */

#ifndef COM_USART_H_
#define COM_USART_H_
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void IrSerial();


#ifdef __cplusplus
}
#endif

class Serial {
public:

void begin();
void decodificar(uint8_t indice);
float aceleracion=0;
float angle=0;
bool adelante=0;
bool atraz=0;

private:

uint8_t datosRx[40];
uint8_t comando;
uint32_t valor;

};



#endif /* COM_USART_H_ */
