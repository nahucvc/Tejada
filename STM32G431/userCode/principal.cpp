/*
 * principal.cpp
 *
 *  Created on: Nov 24, 2024
 *      Author: Nahuel
 */
#include "principal.h"
#include "main.h"
#include "com_usart.h"
#include "controlAuto.h"
extern Serial serial;
extern Direccion dircontrol;
void setup()
{
serial.begin();
dircontrol.iniciarADC();
}
void loop()
{

}



