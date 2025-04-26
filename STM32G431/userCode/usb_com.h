/*
 * usb_com.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Nahuel
 */

#ifndef USB_COM_H_
#define USB_COM_H_
#include "main.h"
#include "usbd_cdc_if.h"


class SerialUsb {
public:
void enviar( const uint8_t *,...);
void recibirDatos();


private:
uint8_t buffer[50];

char comando;
float valor;


};


#endif /* USB_COM_H_ */
