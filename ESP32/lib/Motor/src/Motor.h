#ifndef MOTOR_H
#define MOTOR_h
#include <Arduino.h>
#include "FS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <esp_task_wdt.h>
#include <PinSalidas.h>

extern float angulo;
extern float aceleracion;
extern AsyncWebSocket ws;
extern TaskHandle_t _Direccion;
void ControlMotor();
void Active_ADC();

#endif