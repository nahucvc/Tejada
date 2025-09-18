#include <Arduino.h>
#include <Inicio.h>
#include <Comunicacion.h>

HardwareSerial Serial1(PA10, PC4);

void setup() {
  inicializar_pines();
  Serial.begin(115200);
  Serial.setTimeout(2);
  Serial1.begin(115200);
  Serial1.setTimeout(2);
  InicioConfig();
  
}

void loop() {
  Comunicacion_Serial();  // no bloqueante
}
