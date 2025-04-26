#include "RespuestaServidor.h"

void Recibirdatos(void *art, uint8_t *datos, size_t len)
{

    char *puntero = (char *)datos;
    puntero[len] = 0;
    String inf(puntero);
    deserializeJson(doc, inf);
    JsonObject obj = doc.as<JsonObject>();
    if (obj.containsKey("angle"))
    {
        angulo = obj[String("angle")];
        Serial.printf("angulo: ");
        Serial.println(angulo);
    }

    if (obj.containsKey("ac"))
    {
        aceleracion = obj[String("ac")];
        Serial.printf("aceleracion: ");
        Serial.println(aceleracion);
    }
    if (obj.containsKey("en"))
    { 
        _joystickAvalible=1;
    }
    
}



