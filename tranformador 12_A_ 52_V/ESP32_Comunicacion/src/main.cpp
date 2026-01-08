
#include <Arduino.h>
#include <servidor.h>
#include <WiFiUdp.h>
#include "esp32s2/rom/crc.h"
#include "Recibir_datos.h"
HardwareSerial Myserial(1);

const char* ssid = "MICRO";
const char* password = "123456789";

IPAddress teleplotIP(192,168,137,1); 
const uint16_t teleplotPort = 47269;

WiFiUDP udp;



void setup()
{
  Serial.begin(115200);
  Myserial.begin(250000, SERIAL_8N1, 37, 38);
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  udp.begin(12345);
}



void enviarDatosTeleplot(const Datos_Control& d)
{
  char buffer[256];

  int len = snprintf(buffer, sizeof(buffer),
    "vref:%f\n"
    "vadc:%f\n"
    "imax:%f\n"
    "iadc:%f\n"
    "vbat:%f\n"
    "error:%f\n"
    "duty:%lu\n",
    d.voltaje_referencia,
    d.voltaje_ADC,
    d.corriente_maxima,
    d.coriente_ADC,
    d.voltaje_bateria,
    d.error,
    d.duty
  );

  udp.beginPacket(teleplotIP, teleplotPort);
  udp.write((uint8_t*)buffer, len);
  udp.endPacket();
}



void loop()
{
  if (recibirDatosControl(datosRx))

  {
    
    try
    {
      enviarDatosTeleplot(datosRx);
    }
    catch(const std::exception& e)
    {
      Serial.printf("error");
    }
    
      
    
  }
}
