/*
 * controlAuto.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Nahuel
 */

#ifndef CONTROLAUTO_H_
#define CONTROLAUTO_H_
#include "main.h"
#include "usb_com.h"
#include <cstdlib> // Para rand()
#include <cmath>   // Para sqrt() y log()
class Direccion {
public:
void iniciarADC();
void  MoverDireccion(float lectura);
uint32_t Aceleracion=0;;
float angulo=0;
uint32_t ADC;
private:



float CE[4]={12	,0,0};
float CS[4]={1,0,0};
float VE[4];
float VS[4];
uint16_t duty1=0;
uint16_t duty2=0;

};

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






#ifdef __cplusplus

extern "C" {
#endif

void irTim15();
void irTim7();
void irTim2();

#ifdef __cplusplus
}
#endif


#endif /* CONTROLAUTO_H_ */
