/*
 * usb_com.cpp
 *
 *  Created on: Nov 25, 2024
 *      Author: Nahuel
 */

#include "usb_com.h"
#include "controlAuto.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern Direccion dircontrol;
extern TIM_HandleTypeDef htim2;
uint8_t  USB_buffer_rx[50];
SerialUsb serialUsb;
uint8_t banderaUSBRX=0;
void SerialUsb::enviar( const  uint8_t *datos,...)
{
    va_list args; // Lista de argumentos variádicos

    // Inicializar args con los argumentos adicionales
    va_start(args, datos);

    // Usar sprintf con los argumentos variádicos
    vsprintf((char *) buffer, (char *) datos, args);

    va_end(args);
    CDC_Transmit_FS(buffer,sizeof(buffer));

}


void SerialUsb::recibirDatos()
{
   if (banderaUSBRX)
   {

	   	  uint16_t i=0;
	   	  banderaUSBRX=0;
	   	  bool bandera=1;
	   	  for ( ;i<49;i++)
	   	  {
	   		  if( ((char) USB_buffer_rx[i])==';')
	   		  {
	   			  bandera=0;
	   			  break;
	   		  }
	   	  }

	   	  if(bandera)
	   	  {
	   		  return;
	   	  }
	   	  htim2.Instance->CNT=0;
	   	  comando=USB_buffer_rx[i-5];
	   	  float * aux = (float *) &(USB_buffer_rx[i-4]);
	   	  valor=*aux;
	   	  //valor= (USB_buffer_rx[i-4] << 24) | (USB_buffer_rx[i -3] << 16) | (USB_buffer_rx[i -2] << 8) | USB_buffer_rx[i-1];
	   	  switch (comando) {
			case 'a':

				if(valor>100)
				{
                 valor=100;
				}
				if(valor<0)
				{
					valor=0;
				}
				valor= (valor*4094)/100.0;
				dircontrol.Aceleracion=(uint32_t) valor;
				break;

			case 'g':

				dircontrol.angulo= valor;
				serialUsb.enviar((uint8_t *)"angulo:%f\n",dircontrol.angulo );
				break;
			 case 'r':

				if (valor == 1)
				{
					HAL_GPIO_WritePin(adelante_GPIO_Port, adelante_Pin,GPIO_PIN_RESET );
					HAL_GPIO_WritePin(atraz_GPIO_Port, atraz_Pin, GPIO_PIN_SET);

				}
				else
				{
					HAL_GPIO_WritePin(atraz_GPIO_Port, atraz_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(adelante_GPIO_Port, adelante_Pin,GPIO_PIN_SET );

				}

				break;
			 case 'C':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(C_GPIO_Port,C_Pin, GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(C_GPIO_Port,C_Pin, GPIO_PIN_RESET);
				 }
				 break;
			 case 'D':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(D_GPIO_Port,D_Pin, GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(D_GPIO_Port,D_Pin, GPIO_PIN_RESET);
				 }
				 break;
			 case 'E':
				 if(valor ==1)
				 {
					 HAL_GPIO_WritePin(E_GPIO_Port,E_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(E_GPIO_Port,E_Pin,GPIO_PIN_RESET);

				}
				 break;
			 case 'F':
				 if (valor==1)
				 {
					 HAL_GPIO_WritePin(F_GPIO_Port,F_Pin,GPIO_PIN_SET);
				}else
				{
					HAL_GPIO_WritePin(F_GPIO_Port,F_Pin,GPIO_PIN_RESET);
				}
				 break;
			 case 'G':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(G_GPIO_Port,G_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(G_GPIO_Port,G_Pin,GPIO_PIN_RESET);
				 }
				 break;
			 case 'H':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(H_GPIO_Port,H_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(H_GPIO_Port,H_Pin,GPIO_PIN_RESET);
				 }
				 break;
			 case 'I':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(I_GPIO_Port,I_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(I_GPIO_Port,I_Pin,GPIO_PIN_RESET);
				 }
				 break;
			 case 'J':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(J_GPIO_Port,J_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(J_GPIO_Port,J_Pin,GPIO_PIN_RESET);
				 }
				 break;
			 case 'K':
				 if(valor==1)
				 {
					 HAL_GPIO_WritePin(K_GPIO_Port,K_Pin,GPIO_PIN_SET);
				 }else
				 {
					 HAL_GPIO_WritePin(K_GPIO_Port,K_Pin,GPIO_PIN_RESET);
				 }
				 break;
			default:
				break;

		}



   }





}


