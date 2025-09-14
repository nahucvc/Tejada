#include "Control.h"
#define APAGADO_ACELERACION_MS 250

float Aceleracion = 0;
bool AC_Recibido = 0;

void Direccion ()
{

} 

void Control()
{

    static uint16_t milisegundos = 0;
    if (AC_Recibido)
    {
        milisegundos = 0;
    }
    else
    {
        milisegundos++;
    }

#ifdef METRICA
    Serial.printf(">valorADC:%d\n", ValorADC);
    Serial.printf(">kalman:%d\n", ADC_kalman);
    Serial.printf(">aceleracion:%d\n", Aceleracion);
#endif
    if (milisegundos >= APAGADO_ACELERACION_MS)
    {
        milisegundos = 0;
    }

    Direccion();
    

    update_Aceleracion(Aceleracion);
}



