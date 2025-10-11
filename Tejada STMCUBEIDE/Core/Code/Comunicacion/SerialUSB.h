/*
 * SerialUSB.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */

#ifndef CODE_COMUNICACION_SERIALUSB_H_
#define CODE_COMUNICACION_SERIALUSB_H_
#include <stdio.h>
#include "main.h"
#include <stdarg.h>


class SerialUSB {
public:
    SerialUSB(){}

    // Formatea y encola la salida (no bloquea si el USB está ocupado)
    void printf(const char *fmt, ...);

    // Llamar periódicamente para drenar la cola hacia USB
    void service();

private:
    // Buffer temporal para formatear (vsnprintf)
    char fmtBuffer[500];

    // Cola de transmisión (ajustá el tamaño según tu RAM disponible)
    static constexpr size_t TX_QUEUE_SIZE = 500;
    uint8_t txQueue[TX_QUEUE_SIZE];
    volatile size_t qHead = 0;   // índice de escritura
    volatile size_t qTail = 0;   // índice de lectura

    // Buffer de trabajo para el paquete USB en vuelo
    // (Debe mantenerse válido hasta que el stack USB lo transmita)
    static constexpr size_t USB_PKT_MAX = 64; // CDC FS suele ser 64
    uint8_t txWorkBuf[USB_PKT_MAX];

    // Helpers de cola
    size_t queueFree() const;
    size_t queueCount() const;
    void   queuePushBytes(const uint8_t* data, size_t len);
    size_t queuePopBytes(uint8_t* out, size_t maxlen);

    // Intenta enviar un paquete desde la cola (no bloquea)
    void tryKickTx();
};

extern SerialUSB Serial;

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __cplusplus
 }
#endif


#endif /* CODE_COMUNICACION_SERIALUSB_H_ */
