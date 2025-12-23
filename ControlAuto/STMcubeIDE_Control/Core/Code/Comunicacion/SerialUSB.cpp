/*
 * SerialUSB.cpp
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */
#include "SerialUSB.h"
#include "usbd_cdc_if.h"
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);


// -------------------- Métodos privados (cola) --------------------

size_t SerialUSB::queueFree() const {
    // Deja 1 byte de margen para diferenciar lleno vs vacío
    if (qHead >= qTail) return (TX_QUEUE_SIZE - (qHead - qTail) - 1);
    return (qTail - qHead - 1);
}

size_t SerialUSB::queueCount() const {
    if (qHead >= qTail) return (qHead - qTail);
    return (TX_QUEUE_SIZE - (qTail - qHead));
}

void SerialUSB::queuePushBytes(const uint8_t* data, size_t len) {
    // Encola hasta que se acabe el espacio
    for (size_t i = 0; i < len; ++i) {
        size_t nextHead = (qHead + 1) % TX_QUEUE_SIZE;
        if (nextHead == qTail) {
            // Cola llena: descartamos lo que no entra (o podrías bloquear/esperar)
            break;
        }
        txQueue[qHead] = data[i];
        qHead = nextHead;
    }
}

size_t SerialUSB::queuePopBytes(uint8_t* out, size_t maxlen) {
    size_t cnt = 0;
    while (cnt < maxlen && qTail != qHead) {
        out[cnt++] = txQueue[qTail];
        qTail = (qTail + 1) % TX_QUEUE_SIZE;
    }
    return cnt;
}

// -------------------- Transmisión USB no bloqueante --------------------

void SerialUSB::tryKickTx() {
    // Mientras haya datos y el USB acepte, mandamos paquetes de hasta 64 bytes
    // CDC_Transmit_FS devuelve USBD_BUSY si aún hay uno en vuelo.
    while (queueCount() > 0) {
        // Armar un paquete
        size_t n = queuePopBytes(txWorkBuf, USB_PKT_MAX);
        if (n == 0) break;

        uint8_t st = CDC_Transmit_FS(txWorkBuf, (uint16_t)n);
        if (st == USBD_BUSY) {
            // No se pudo enviar ahora: reinsertar el paquete al frente de la cola.
            // (Estrategia simple: encolarlo al final de nuevo)
            queuePushBytes(txWorkBuf, n);
            break;
        } else if (st != USBD_OK) {
            // Error no esperado: descartar este paquete para no trabar la cola.
            // (Alternativa: reintentar/enviar a log)
            break;
        } else {
            // USBD_OK => paquete aceptado; seguimos intentando enviar más.
            // Importante: txWorkBuf no se toca hasta que el stack haga copia interna.
            // En la implementación de Cube, Transmit_FS programa el envío de inmediato.
            // En la práctica, mandar paquetes consecutivos suele devolver BUSY en el siguiente ciclo.
        }
    }
}

// -------------------- API pública --------------------

void SerialUSB::printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(fmtBuffer, sizeof(fmtBuffer), fmt, args);
    va_end(args);

    if (len <= 0) return;

    // Asegurar que no exceda el buffer temporal
    size_t toCopy = (len > (int)sizeof(fmtBuffer)) ? sizeof(fmtBuffer) : (size_t)len;

    // Encolar los datos (si no entra todo, lo que no entra se descarta)
    queuePushBytes(reinterpret_cast<const uint8_t*>(fmtBuffer), toCopy);

    // Intentar enviar inmediatamente lo que se pueda (no bloqueante)
    tryKickTx();
}

void SerialUSB::service() {
    // Llamar periódicamente para drenar la cola si antes estaba ocupado
    tryKickTx();
}


SerialUSB Serial;
