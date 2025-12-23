#include "Control.h"

float Aceleracion = 0;
bool AC_Recibido = 0;
float Angulo = 90;
double ceE[3] = {6,0, 0};
double ceS[3] = {1, 0, 0};
double Vs[3] = {0};
double Ve[3] = {0};
float anguloRuedas;
float Error;

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
    Serial.printf(">ADC:%d\n", ValorADC);
    Serial.printf(">ADC_K:%d\n", ADC_kalman);
    Serial.printf(">AC:%.4f\n", Aceleracion);
    Serial.printf(">AngleRef:%.4f\n", Angulo);
    Serial.printf(">Error:%.4f\n", Error);
    Serial.printf(">Vs:%.4f\n", Vs[0]);

#endif
    if (milisegundos >= APAGADO_ACELERACION_MS)
    {
        Aceleracion = 0;
        milisegundos = 0;
    }

    Direccion();

    update_Aceleracion(Aceleracion);
}
