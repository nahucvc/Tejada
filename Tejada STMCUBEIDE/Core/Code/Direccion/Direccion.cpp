/*
 * Direccion.cpp
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */
#include "Direccion.h"
#include "SerialUSB.h"
#include <string.h>
#define METRICA
#define APAGADO_ACELERACION_MS 4000
extern TIM_HandleTypeDef htim3;
extern DAC_HandleTypeDef hdac1;

float Aceleracion = 0;
bool AC_Recibido = 0;
float Angulo = 90;
double ceE[3] = {6,0, 0};
double ceS[3] = {1, 0, 0};
double Vs[3] = {0};
double Ve[3] = {0};
float anguloRuedas;
float Error;
KalmanFilter k1(0/*valor inicial*/,1 /*incertidumbre inicial*/,0.2 /*Ganacia de Kalman inicial*/ ,1/*Varianza de la medición*/);

float ADC_kalman=0;


void InterrupcionFiltro()
{
	k1.update(ValorADC);
	ADC_kalman = k1.getState();
}

void GiraDerecha(float duty)
{
    htim3.Instance->CCR1 = 0;
    htim3.Instance->CCR2 = (uint16_t) (duty * 17000);
}
void GiraIquierda(float duty)
{
    htim3.Instance->CCR2 = 0;
    htim3.Instance->CCR1 = (uint16_t) (duty * 17000);
}

void Direccion()
{
    if (ADC_kalman >= 1857)
    {
        anguloRuedas= -0.159*ADC_kalman+385.2;
    }else
    {
        anguloRuedas= -0.1737*ADC_kalman+412.59;
    }


    Error = (abs(Angulo) - anguloRuedas) / 90;
    Ve[2] = Ve[1];
    Ve[1] = Ve[0];
    Ve[0] = Error;

    Vs[2] = Vs[1];
    Vs[1] = Vs[0];
    Vs[0] = (Ve[0] * ceE[0]) + (Ve[1] * ceE[1]) + (Ve[2] * ceE[2]) + (Vs[1] * ceS[1]) + (Vs[2] * ceS[2]);

    (Vs[0] > 1) ? Vs[0] = 1 : ((Vs[0] < -1) ? Vs[0] = -1 : Vs[0] = Vs[0]);

    if (Vs[0] > 0)
    {
        GiraDerecha(abs(Vs[0]));
    }
    else
    {
        GiraIquierda(abs(Vs[0]));
    }

}

void update_Aceleracion(float ac)
{
uint32_t valorDAC = (uint32_t) ((ac/100.0)*4093.0);
hdac1.Instance->DHR12R1=(valorDAC & 0x0FFFu);

if (Angulo<0)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

}else
{
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
}
}





void Control()
{

    static uint16_t milisegundos = 0;
    if (AC_Recibido)
    {
        milisegundos = 0;
        AC_Recibido = 0;
    }
    else
    {
        milisegundos++;
    }

#ifdef METRICA
    static char datos[100];
    memset(datos, 0, 100);
    sprintf(datos,"\n>ADC:%d\n>ADC_K:%f\n>AC:%.4f\n>AngleRef:%f\n>Error:%f\n>Vs:%f\n",(int) ValorADC,ADC_kalman,Aceleracion,Angulo,Error,Vs[0] );
    uint8_t * puntero = (uint8_t *) datos;
    CDC_Transmit_FS( puntero, sizeof(datos));


#endif
    if (milisegundos >= APAGADO_ACELERACION_MS)
    {
        Aceleracion = 0;
        milisegundos = 0;
    }

    Direccion();
    update_Aceleracion(Aceleracion);

}

void InterrupcionADC()
{

	Control();
}




