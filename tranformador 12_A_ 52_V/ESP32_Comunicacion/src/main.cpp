
#include <Arduino.h>
#include <servidor.h>
#include <WiFiUdp.h>
#include "esp32s2/rom/crc.h"
#include "Recibir_datos.h"
#include <queue>
#include <ArduinoJson.h>
HardwareSerial Myserial(1);

const char* ssid = "MICRO";
const char* password = "123456789";

std::queue<Datos_Control> cola;
constexpr size_t BUFFER_JSON_SIZE = 1500;
char bufferJson[BUFFER_JSON_SIZE];
size_t bufferJsonLen = 0;

void enviarDatos()
{
  if (cola.empty())
    return;

  DynamicJsonDocument doc(1500);
  JsonArray arrayDatos = doc.createNestedArray("datos");

  while (!cola.empty())
  {
    Datos_Control d = cola.front();
    cola.pop();

    JsonObject obj = arrayDatos.createNestedObject();
    obj["vref"]  = d.voltaje_referencia;
    obj["vadc"]  = d.voltaje_ADC;
    obj["imax"]  = d.corriente_maxima;
    obj["iadc"]  = d.coriente_ADC;
    obj["vbat"]  = d.voltaje_bateria;
    obj["error"] = d.error;
    obj["duty"]  = d.duty;
  }

  // Serializar en buffer de caracteres
  bufferJsonLen = serializeJson(doc, bufferJson, BUFFER_JSON_SIZE);

  // Seguridad: terminador nulo por si se usa como string C
  if (bufferJsonLen < BUFFER_JSON_SIZE)
    bufferJson[bufferJsonLen] = '\0';
    ws.textAll(bufferJson);
}


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
  IniciarServidor();
 
}



void loop()
{
  if (recibirDatosControl(datosRx))

  {
    
    try
    {
      cola.push(datosRx);
    }
    catch(const std::exception& e)
    {
      Serial.printf("error");
    }
    
      
    
  }
  enviarDatos();
}
