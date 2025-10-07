#include <Arduino.h>
#include <Inicio.h>
#include <Comunicacion.h>

HardwareSerial Serial1(PA10,PC4);

void setup()
{
  Serial1.begin(115200);
  inicializar_pines();
  InicioConfig();
}

void loop()
{
  Comunicacion_Serial(); // no bloqueante
}
