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
// filtro de kalman 
class KalmanFilter {
private:
    float x;   // Estado estimado
    float P;   // Covarianza del estado
    float Q;   // Varianza del proceso
    float R;   // Varianza de la medición
    float K;   // Ganancia de Kalman

public:
    // Constructor
    KalmanFilter(float initial_state, float initial_uncertainty, float process_variance, float measurement_variance)
        : x(initial_state), P(initial_uncertainty), Q(process_variance), R(measurement_variance), K(0) {}

    // Actualización del filtro con una nueva medición
    void update(float measurement) {
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
    float getState() const {
        return x;
    }

    // Obtener la incertidumbre actual
    float getUncertainty() const {
        return P;
    }
    float generateUniformNoise(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

};




void InicioConfig();


#endif