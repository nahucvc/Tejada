#include "Motor.h"

TaskHandle_t _Motor;

void _ControlMotor(void *parametros);
TaskHandle_t _Encendido;
short _joystickAvalible = 0;
void Encendido(void *parametro)
{

  
  while (1)
  {
    
    ws.textAll( "{\"encendido\":true}");
    delay(900);
   
    if (_joystickAvalible == 0)
    {
      aceleracion=0;

     }
        
    else
    {
      
      _joystickAvalible = 0;
    }
  }
}




void _ControlMotor(void *parametros)
{
    
    Serial.print("codigo del motor funcionando\n");
    
    pinMode(_Adelante, OUTPUT);
    pinMode(_Atraz, OUTPUT);
    pinMode(_Acelerar, OUTPUT);
    int valor_acelerador=0;
    while(1)
    {

        if (angulo > 0)
        {
            digitalWrite(_Atraz, 0);
            digitalWrite(_Adelante, 1);
        }
        else
        {
            digitalWrite(_Adelante, 0);
            digitalWrite(_Atraz, 1);
        }
        valor_acelerador=(aceleracion*255)/100;
        dacWrite(18,valor_acelerador);
        //Serial.println(valor_acelerador);
        delay(1);
    }
}

void ControlMotor()
{
    
    
    xTaskCreateUniversal(_ControlMotor,"controlMotor", 2024,NULL,1,&_Motor,0);
    xTaskCreate(Encendido, "encendido", 2024, NULL, 1, &_Encendido);
  // 
}
