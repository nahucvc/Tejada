/*
 * control.cpp
 *
 *  Created on: 18 dic 2025
 *      Author: Nahuel
 */

#include "control.h"
#include "UserMain.h"

class KalmanFilter {
private:
	float x; // Estado estimado
	float P; // Covarianza del estado
	float Q; // Varianza del proceso
	float R; // Varianza de la medición
	float K; // Ganancia de Kalman

public:

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
		return min
				+ static_cast<float>(rand())
						/ (static_cast<float>(RAND_MAX / (max - min)));
	}

	/**
	 * @brief Constructor del filtro de Kalman.
	 *
	 * Inicializa el estado interno y los parámetros principales del filtro.
	 *
	 * @param initial_state Estado inicial estimado del sistema.
	 * @param initial_uncertainty Incertidumbre inicial asociada al estado.
	 * @param process_variance Varianza del proceso (ruido del modelo).
	 * @param measurement_variance Varianza de la medición (ruido del sensor).
	 */

	KalmanFilter(float initial_state /** Estado Inicial **/,
			float initial_uncertainty, float process_variance,
			float measurement_variance) :
			x(initial_state), P(initial_uncertainty), Q(process_variance), R(
					measurement_variance), K(0) {

	}
};

#define  VOLTAJE_MEDIDO_TRANSFORMADOR  adcBuffer[0]
#define  CORRIENTE_TRANSFORMADOR       adcBuffer[1]

Datos_Control datos_control = { 2000, 0, 1000, 0, 0, 0, 0, 0 };
Datos_Control datos_tx;
KalmanFilter k1(0, 1, 2, 1);
extern CRC_HandleTypeDef hcrc;


#include <cstdint>
#include <cstddef>

uint32_t calcularCRC32(const uint8_t* data, size_t length)
{
    uint32_t crc = 0xFFFFFFFF;        // Valor inicial
    const uint32_t poly = 0xEDB88320; // Polinomio reflejado CRC-32

    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
    }

    return ~crc; // Inversión final
}



void iniciar_filtro1() {
	//memset(estado_filtro, 0, sizeof(estado_filtro));

	//arm_fir_init_f32( &filtro,NUMERO_ESTADOS,coeficientes,estado_filtro,1);

}

extern UART_HandleTypeDef huart1;

void trasmitir_datos(void) {
	uint8_t *puntero = (uint8_t*) BufferTX;
	*puntero = 36; // inicio de trama
	puntero++;

	datos_tx=datos_control;

	datos_tx.CRCdata= calcularCRC32((uint8_t *) &datos_control,DATOS_CONTROL_SIZE -4 );
	HAL_UART_Transmit(&huart1, (uint8_t*) BufferTX, 33,10);
	memcpy(puntero, &datos_tx, DATOS_CONTROL_SIZE);
}

bool flag_break_tim8 = false;
uint32_t tiempo_interrupcion;
void control_break_tim_8() {
	flag_break_tim8 = true;
	tiempo_interrupcion = HAL_GetTick();
	__HAL_TIM_CLEAR_FLAG(&htim8, TIM_IT_BREAK);

}

#define TIEMPO_INTERRUPCION_MS 500
void loop_break_control() {
	if (flag_break_tim8) {
		if (HAL_GetTick() - tiempo_interrupcion > TIEMPO_INTERRUPCION_MS) {
			flag_break_tim8 = false;
			__HAL_TIM_CLEAR_FLAG(&htim8, TIM_FLAG_BREAK);
			__HAL_TIM_MOE_ENABLE(&htim8);
			__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_BREAK);
		}

	}
}

void control_loop() {
	static uint32_t duty_max = htim8.Instance->ARR;
	datos_control.voltaje_ADC = (adcBuffer[0] - 1994.0) * 1.61731844f;
	datos_control.coriente_ADC = (adcBuffer[1] - 1994.0) * 1.61731844f;
	datos_control.voltaje_bateria = adcBuffer[2]* 0.0008056f;
	datos_control.error = datos_control.voltaje_referencia
			- (float32_t) datos_control.voltaje_ADC;
	if (datos_control.error > 0) {
		datos_control.duty = (uint32_t) ((datos_control.error / 3200.0f) * 0.3f
				* duty_max);
		htim8.Instance->CCR1 = datos_control.duty;
	} else {
		htim8.Instance->CCR1 = 0;
		datos_control.duty = 0;
	}

}
