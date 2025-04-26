
#include <Arduino.h>
#include <servidor.h>
#include <Motor.h>
#include <Direccion.h>
#include <esp_task_wdt.h>

const char *ssid = "Auto"; // Nombre del punto de acceso Wi-Fi
const char *password = "123456789";

float angulo = 90;
float aceleracion = 0;

void setup()
{
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  Serial.begin(115200);
  // esp_task_wdt_init(40, 1);
  WiFi.softAP(ssid, password);
  
  Serial.println();
  Serial.print("Punto de acceso WiFi creado con SSID: ");
  Serial.println(ssid);
  IniciarServidor();
  inicioADC();
  Direccion();
  ControlMotor();
}

void RecibirSerial(void)
{

  String inf = Serial.readString();
  //Serial.println(inf);
 
  if (inf.compareTo(""))
  {
    deserializeJson(doc, inf);
    JsonObject obj = doc.as<JsonObject>();
    if (obj.containsKey("angle"))
    {
      angulo = obj[String("angle")];
      Serial.printf("angulo: ");
      Serial.println(angulo);
    }

    if (obj.containsKey("ac"))
    {
      aceleracion = obj[String("ac")];
      Serial.printf("aceleracion: ");
      Serial.println(aceleracion);
    }
    if (obj.containsKey("en"))
    {
      _joystickAvalible = 1;
    }
  }
}

void loop()
{
 

  if (Serial.available()>0)
  {
    RecibirSerial();
  }
  
  
  
}
