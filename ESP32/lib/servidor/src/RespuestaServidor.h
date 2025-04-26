#ifndef RESPUESTA_SERVIDOR_H
#define RESPUESTA_SERVIDOR_H
#include <Arduino.h>
#include "FS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>


DynamicJsonDocument doc(1024);
void Recibirdatos(void *art, uint8_t *datos, size_t len);
extern float angulo;
extern float aceleracion;
extern short _joystickAvalible;
 



#endif