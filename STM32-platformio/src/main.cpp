#include <Arduino.h>
#include <Inicio.h>
#include <Comunicacion.h>

void setup()
{
  inicializar_pines();
  Serial1.begin(115200);
  InicioConfig();
}

void loop()
{
  Comunicacion_Serial();
}
