#ifndef COMUNICACION_H
#define COMUNICACION_H
#include <Control.h>
#include <Arduino.h>

#define PIN_C PA15
#define PIN_D PC10
#define PIN_E PC11
#define PIN_F PB3
#define PIN_G PB4
#define PIN_H PB5
#define PIN_I PB6
#define PIN_J PB7
#define PIN_K PB9

// Tamaños de buffer para parser no bloqueante
#define RX_BUF_LEN 64

void Comunicacion_Serial();
void inicializar_pines();

#ifdef __cplusplus
extern "C" {
#endif
bool processSerial(char ch);
#ifdef __cplusplus
}
#endif
#endif
