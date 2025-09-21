
#include <Arduino.h>
#include <servidor.h>

HardwareSerial Myserial(1);
const char* SSID_AP     = "ESP32_AP";
const char* PASS_AP     = "123456789";   // mínimo 8 caracteres (WPA2)

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway (192, 168, 4, 1);
IPAddress subnet  (255, 255, 255, 0);
float angulo , aceleracion;

void setup()
{
  
  Serial.begin(115200);
  Myserial.begin(115200, SERIAL_8N1, 37, 38);
  WiFi.mode(WIFI_AP);

  // IP del AP (opcional: si no lo ponés, usa 192.168.4.1 por defecto)
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // softAP(ssid, pass, canal, oculto, max_conexiones)
  bool ok = WiFi.softAP(SSID_AP, PASS_AP, 6, false, 4);
  Serial.println(ok ? "AP levantado OK" : "Error al iniciar AP");
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP()); 
  IniciarServidor();
 
}


void loop()
{
 

  
}
