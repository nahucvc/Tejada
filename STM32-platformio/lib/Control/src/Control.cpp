#include "Control.h"


float Aceleracion = 0;
bool AC_Recibido = 0;
float Angulo =0;
void Direccion ()
{

} 

void Control()
{

    static uint16_t milisegundos = 0;
    if (AC_Recibido)
    {
        milisegundos = 0;
        AC_Recibido=0;
    }
    else
    {
        milisegundos++;
    }

#ifdef METRICA
    Serial.printf(">valorADC:%d\n", ValorADC);
    Serial.printf(">kalman:%d\n", ADC_kalman);
    Serial.printf(">aceleracion:%f\n", Aceleracion);
    Serial.printf(">Angulo:%f\n", Angulo);
#endif
    if (milisegundos >= APAGADO_ACELERACION_MS)
    {
        Aceleracion=0;
        milisegundos = 0;
    }

    Direccion();
    

    update_Aceleracion(Aceleracion);
}



