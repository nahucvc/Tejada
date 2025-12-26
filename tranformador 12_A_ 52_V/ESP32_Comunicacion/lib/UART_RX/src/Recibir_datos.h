#ifndef RECIBIR_DATOS_H
#define RECIBIR_DATOS_H
#include "Arduino.h"


struct __attribute__((packed)) Datos_Control
{
  uint32_t voltaje_referencia;
  uint32_t voltaje_ADC;
  uint32_t corriente_maxima;
  uint32_t coriente_ADC;
  uint32_t voltaje_bateria;
  float error;
  uint32_t duty;
  uint32_t CRCdata;
};

Datos_Control datosRx;


uint32_t calcularCRC32(const uint8_t *data, size_t length)
{
  uint32_t crc = 0xFFFFFFFF;        // Valor inicial
  const uint32_t poly = 0xEDB88320; // Polinomio reflejado CRC-32

  for (size_t i = 0; i < length; i++)
  {
    crc ^= data[i];

    for (uint8_t bit = 0; bit < 8; bit++)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ poly;
      else
        crc >>= 1;
    }
  }

  return ~crc; // Inversión final
}

bool validarCRC(const Datos_Control &d)
{
  return d.CRCdata == calcularCRC32((const uint8_t *)&d, sizeof(Datos_Control) - 4);
}




extern HardwareSerial Myserial;;

#define START_BYTE '$'
#define FRAME_SIZE sizeof(Datos_Control)
bool recibirDatosControl(Datos_Control &datos)
{
  static uint8_t buffer[FRAME_SIZE];
  static uint8_t index = 0;
  static bool recibiendo = false;

  while (Myserial.available())
  {
    uint8_t byteRecibido = Myserial.read();

    // Detectar inicio de trama
    if (byteRecibido == START_BYTE)
    {
      recibiendo = true;
      index = 0;
      continue;
    }

    // Si no se detectó inicio, ignorar
    if (!recibiendo)
      continue;

    buffer[index++] = byteRecibido;

    // Trama completa
    if (index >= FRAME_SIZE)
    {
      memcpy(&datos, buffer, FRAME_SIZE);
      recibiendo = false;
      index = 0;
      if ( uint32_t crc_calc = calcularCRC32((const uint8_t *)&datosRx, sizeof(Datos_Control) - 4)==datos.CRCdata)
      {
        return true; // ✔ Datos válidos recibidos
      }else
      {
        return false;
      }
      
      
      
    }
  }

  return false; // ❌ Todavía no llegó el paquete completo
}





#endif

