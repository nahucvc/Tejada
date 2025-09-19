#include <Arduino.h>
#include <Inicio.h>
#include <Comunicacion.h>

void setup()
{
  inicializar_pines();
  InicioConfig();
}

void loop()
{
  Comunicacion_Serial(); // no bloqueante
}
