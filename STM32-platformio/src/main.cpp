#include <Arduino.h>
#include <Inicio.h>
#include <Comunicacion.h>

HardwareSerial Serial1(PA10, PC4);

void setup() {
  inicializar_pines();
  Serial.begin(115200);
  
  Serial1.begin(230400, SERIAL_8E2);
 
  InicioConfig();
  
}

void loop() {
  Comunicacion_Serial();  // no bloqueante
}
