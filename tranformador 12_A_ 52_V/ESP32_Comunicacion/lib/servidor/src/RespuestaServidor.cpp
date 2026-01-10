#include "RespuestaServidor.h"
#include <ArduinoJson.h>

extern HardwareSerial Myserial;
StaticJsonDocument<256> doc; // ajustá tamaño a tus mensajes

void Recibirdatos(void *art, uint8_t *datos, size_t len)
{
   
    DeserializationError err = deserializeJson(doc, datos, len);
    if (err) {
        Serial.printf("JSON error: %s\n", err.c_str());
        return;
    }

    JsonObject obj = doc.as<JsonObject>();
    char datosRecibidos[60];
    serializeJson(doc,datosRecibidos);
    Serial.println(datosRecibidos);

    if (obj.containsKey("angle")) {
       
    }

    if (obj.containsKey("ac")) {
        
            
        }
    
}
