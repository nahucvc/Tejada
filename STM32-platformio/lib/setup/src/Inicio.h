#ifndef INICIO_H
#define INICIO_H
#include <Arduino.h>
#include "adc.h"
#include "gpio.h"
#include "stm32g4xx_it.h"
#include "tim.h"
#include "dac.h"
#include <Control.h>
extern uint32_t ValorADC;
extern uint32_t ADC_kalman;

// funcion para actualizar el acelerador
void update_Aceleracion(float ac);

struct LPF1
{
    float y;     // estado (salida previa)
    float alpha; // coeficiente
};

// filtro de kalman
class KalmanFilter
{
private:
    float x; // Estado estimado
    float P; // Covarianza del estado
    float Q; // Varianza del proceso
    float R; // Varianza de la medición
    float K; // Ganancia de Kalman
    LPF1 filt;
    const float Fs = 1000.0f; // 1000 Hz -> T = 1 ms
    const float fc = 10.0f;   // corta  (pasa continua y bajas)

public:
    // Actualización del filtro con una nueva medición
    void update(float measurement)
    {
        measurement = lpf1_update(filt,measurement);
        // Predicción
        P += Q;

        // Cálculo de la ganancia de Kalman
        K = P / (P + R);

        // Corrección del estado
        x = x + K * (measurement - x);

        // Actualización de la covarianza
        P = (1 - K) * P;
    }

    // Obtener el estado estimado
    float getState() const
    {
        return x;
    }

    // Obtener la incertidumbre actual
    float getUncertainty() const
    {
        return P;
    }
    float generateUniformNoise(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    float lpf1_update(LPF1 &f, float x)
    {
        f.y = f.y + f.alpha * (x - f.y);
        return f.y;
    }

    void lpf1_init(LPF1 &f, float fc, float Fs)
    {
        float T = 1.0f / Fs;
        float tau = 1.0f / (2.0f * PI * fc);
        f.alpha = T / (tau + T); // equivalente a (2πfcT)/(1+2πfcT)
        f.y = 0.0f;
    }

    // Constructor
    KalmanFilter(float initial_state, float initial_uncertainty, float process_variance, float measurement_variance)
        : x(initial_state), P(initial_uncertainty), Q(process_variance), R(measurement_variance), K(0) { lpf1_init(filt, fc, Fs);}
};

void InicioConfig();

#endif