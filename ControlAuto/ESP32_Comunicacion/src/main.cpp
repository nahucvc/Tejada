
#include <Arduino.h>
#include <SPI.h>
#include <servidor.h>
#include "Eth_code.h"

HardwareSerial Myserial(1);
float angulo = 90;
float aceleracion = 0;

const char *SSID_AP = "Auto";
const char *PASS_AP = "123456789"; // mínimo 8 caracteres (WPA2)

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(115200);
  Myserial.begin(115200, SERIAL_8N1, 37, 38);
  
  Serial.println(F("\n[BOOT] Arrancando..."));
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  bool ok = WiFi.softAP(SSID_AP, PASS_AP, 6, false, 4);
  Serial.println(ok ? "AP levantado OK" : "Error al iniciar AP");
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP());

  Serial.println(F("[BOOT] W5500 listo. Iniciando servidor..."));
  IniciarServidor();
  init_W5500();
}

void loop()
{

}
