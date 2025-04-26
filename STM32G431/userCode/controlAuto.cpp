/*
 * controlAuto.cpp
 *
 *  Created on: Nov 25, 2024
 *      Author: Nahuel
 */

#include "controlAuto.h"


extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim7;
extern SerialUsb serialUsb;
extern TIM_HandleTypeDef htim3;
extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim2;
Direccion dircontrol;

void Direccion::iniciarADC()
{
	HAL_ADC_Start_DMA(&hadc1, &this->ADC, 1);
	HAL_TIM_Base_Start_IT(&htim15);

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t *) &this->duty1, 1);
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2,(uint32_t *) &this->duty2, 1);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, &(this->Aceleracion), 1, DAC_ALIGN_12B_R);

}

void Direccion::MoverDireccion(float lectura)
{
  float grados= lectura*355.0913 - 127.1227;
  if (this->angulo > 34.0)
  {
	  this->angulo=34;
  }
  if(this->angulo<-34)
  {
	  this->angulo=-34;
  }
   float error = angulo - grados;
  VE[2] = VE[1];
  VE[1] = VE[0];
  VE[0] = error/34.0;

  VS[2] = VS[1];
  VS[1] = VS[0];
  VS[0] = (VE[0] * CE[0])+   (VE[1] * CE[1])  +   (VE[2] * CE[2])  + (VS[1] * CS[1]) +  (VS[2] * CS[2]) ;

  if (VS[0] > 1)
  {
	  VS[0] = 1;

  }
  if (VS[0] < -1)
    {
  	  VS[0] = - 1;
    }
  uint16_t duty = (uint16_t)(abs(VS[0]) * 16499 +500);
  if (VS[0]>0)
  {
	  this->duty2=0;
	  this->duty1=duty;
  }else
  {
	  this->duty1=0;
	  this->duty2=duty;
  }

}


KalmanFilter filtro(0,1,0.1e-9,2.388e-07);
 void irTim15()
{
static int bandera =0;
if (bandera)
{

	float adc = (dircontrol.ADC)/4095.0;
	filtro.update(adc);
	dircontrol.MoverDireccion(filtro.getState());

	serialUsb.enviar((uint8_t*)"\n>adc:%f\n>kalman:%f\n", adc ,filtro.getState());


}
	bandera=1;

}


 void irTim7()
 {
	 serialUsb.recibirDatos();
 }

 void irTim2()
 {
	 dircontrol.Aceleracion=0;
 }

