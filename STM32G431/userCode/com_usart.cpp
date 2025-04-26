/*
 * com_usart.c
 *
 *  Created on: Nov 24, 2024
 *      Author: Nahuel
 */
#include "com_usart.h"
#include <cstring>
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
Serial serial;

void Serial::begin()
{
 HAL_UART_Receive_DMA(&huart1, this->datosRx, 40);
 __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

}

void Serial::decodificar(uint8_t indice)
{
	this->comando=this->datosRx[indice-5];
	this->valor= (this->datosRx[indice-4] << 24) | (this->datosRx[indice -3] << 16) | (this->datosRx[indice -2] << 8) | this->datosRx[indice-1];
	//memset(this->datosRx, 0, 40);
	switch (this->comando) {
		case 'a':
			float * ac;
			ac=(float *) &valor;
			this->aceleracion= * ac;
			break;
		case 'f':
			float *ag;
			ag=(float *) & valor;
			this->angle= *ac;
			break;
		case 'g':
			this->adelante= (bool) valor;
			break;
		case 'b':
			this->atraz= (bool) valor;
			break;
		default:
			break;
	}
}

void IrSerial()
{
	uint8_t end = (39 - hdma_usart1_rx.Instance->CNDTR);
	  uint8_t * p = huart1.pRxBuffPtr + end;
	     if(*p==';')
	     {
	    	 __HAL_DMA_DISABLE(&hdma_usart1_rx);
	    	 serial.decodificar(end);
	    	 hdma_usart1_rx.Instance->CNDTR=40;
	    	 __HAL_DMA_ENABLE(&hdma_usart1_rx);
	     }
}



