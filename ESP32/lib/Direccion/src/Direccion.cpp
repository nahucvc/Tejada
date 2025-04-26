#include "Direccion.h"
#include "PinSalidas.h"

#include "driver/timer.h"
#include "driver/adc.h"

#define TIMER_INTERVAL_US 1000
#define ADC_CHANNEL ADC1_CHANNEL_8
// variables globales
float Lectura, referencia;
hw_timer_t *timer = NULL;
TaskHandle_t _Direccion;
void ADClectura(void *parametros);
void _direccion(void *parametro);

void IRAM_ATTR onTimer()
{

    static float datos[10];
    datos[9] = datos[8];
    datos[8] = datos[7];
    datos[7] = datos[6];
    datos[6] = datos[5];
    datos[5] = datos[4];
    datos[4] = datos[3];
    datos[3] = datos[2];
    datos[2] = datos[1];
    datos[1] = datos[0];
    datos[0] = (float)adc1_get_raw(ADC_CHANNEL);

    Lectura = ((datos[0]) + datos[1] + datos[2] + datos[3] + datos[4] + datos[5] + datos[6] + datos[7] + datos[8] + datos[9]) / 10.0;
    Lectura = -0.087804 * Lectura + 446.7;

    static const double ceE[3] = {0.179104477611940	,-0.139303482587065,0};
    static const double ceS[3] = {1	,0.990049751243781,0};
    static double Vs[3] = {0};
    static double Ve[3] = {0};
    uint16_t duty;

    referencia = abs(angulo);
    float Error = (Lectura - referencia) / 180;
    Ve[2] = Ve[1];
    Ve[1] = Ve[0];
    Ve[0] = Error;

    Vs[2] = Vs[1];
    Vs[1] = Vs[0];
    Vs[0] = (Ve[0] * ceE[0])+   (Ve[1] * ceE[1])  +   (Ve[2] * ceE[2])  + (Vs[1] * ceS[1]) +  (Vs[2] * ceS[2])  ;  

    if (Vs[0] > 1)
    {
        Vs[0] = 1;
    }
    else
    {
        if (Vs[0] < -1)
        {
            Vs[0] = -1;
        }
    }
    duty = (uint32_t)((abs(Vs[0]) * 1023) + 100);
    if (duty > 1023)
        duty = 1023;
    if (Vs[0] > 0)
    {
        analogWrite(PwmIzquierda, 0);
        analogWrite(PWMDeracha, duty);
    }
    else
    {
        analogWrite(PWMDeracha, 0);
        analogWrite(PwmIzquierda, duty);
    }
}

void inicioADC()
{
    // Inicializar el ADC
    adc1_config_width(ADC_WIDTH_BIT_13);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_12); // Configurar la atenuación del canal

    // Inicializar el puerto serie para ver los valores leídos
    Serial.begin(115200);
    analogWriteFrequency(10000);
    analogWriteResolution(10);

    pinMode(EnableDerecha, OUTPUT);
    pinMode(EnableIzquierda, OUTPUT);
    digitalWrite(EnableDerecha, 1);
    digitalWrite(EnableIzquierda, 1);
    // Configurar el temporizador
    timer = timerBegin(1, 80, true);                 // 80 prescalador para 1 MHz (80 MHz / 80 = 1 MHz)
    timerAttachInterrupt(timer, &onTimer, true);     // Asignar la función de interrupción
    timerAlarmWrite(timer, TIMER_INTERVAL_US, true); // Configurar alarma para 1 kHz
    delay(100);
    timerAlarmEnable(timer); // Activar alarma
}

void Direccion(void)
{

    xTaskCreate(_direccion, "Direccion", 4048, NULL, 1, &_Direccion);
}

void _direccion(void *parametro)
{

   
    while (1)
    {

        Serial.printf(">adc:%f\n>error:%f\n>an:%f\n>duty:%d\n", Lectura);
        delay(4);
    }
}
