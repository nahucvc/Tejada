#include "RespuestaServidor.h"
#include <ArduinoJson.h>

extern HardwareSerial Myserial;
StaticJsonDocument<256> doc; // ajustá tamaño a tus mensajes

void Recibirdatos(void *art, uint8_t *datos, size_t len)
{
    // ❌ NO hagas puntero[len] = 0;  // puede corromper memoria

    // Usar la versión con longitud: no requiere terminador
    DeserializationError err = deserializeJson(doc, datos, len);
    if (err) {
        Serial.printf("JSON error: %s\n", err.c_str());
        return;
    }

    JsonObject obj = doc.as<JsonObject>();

    if (obj.containsKey("angle")) {
        angulo = obj["angle"].as<float>();
        if (angulo > 180)      angulo = 180;
        else if (angulo < -180) angulo = -180;

        //Serial.printf(">angulo: %.2f\n", angulo);
        if (Myserial.availableForWrite() > 10) {
            Myserial.printf("\nB %d\n", (int) angulo);
        }
    }

    if (obj.containsKey("ac")) {
        aceleracion = obj["ac"].as<float>();
        if (aceleracion > 100) aceleracion = 100;

       // Serial.printf(">aceleracion: %.2f\n", aceleracion);
        if (Myserial.availableForWrite() > 10) {
            Myserial.printf("\nA%.2f\n", aceleracion);
        }
    }
}
