/*
 * Direccion.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */

#ifndef CODE_DIRECCION_DIRECCION_H_
#define CODE_DIRECCION_DIRECCION_H_
#include "UserCode.h"
#include <iostream>
#include "cmath"
#define PI 3.14159265358979323846f
extern float Angulo;
extern bool AC_Recibido;
extern float Aceleracion;
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

    const float fc = 10.0f;   // corta  (pasa continua y bajas)

public:
    const float Fs = 5000.0f; // 1000 Hz -> T = 1 ms
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










#ifdef __cplusplus
 extern "C" {
#endif

void InterrupcionFiltro();
void InterrupcionADC();

#ifdef __cplusplus
 }
#endif





#endif /* CODE_DIRECCION_DIRECCION_H_ */
