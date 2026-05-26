#include "Eth_code.h"
#define _ASYNC_WEBSERVER_LOGLEVEL_       2
#include <AsyncTCP.h>
#include <AsyncWebServer_ESP32_W5500.h>
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

// Select the IP address according to your local network
IPAddress myIP(192, 168, 0, 20);
IPAddress myGW(192, 168, 0, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);
#define ETH_SPI_HOST        SPI2_HOST
#define SPI_CLOCK_MHZ       25
#define INT_GPIO            35
#define MISO_GPIO           9
#define MOSI_GPIO           11
#define SCK_GPIO            7
#define CS_GPIO             1


void init_W5500()
{
  Serial.begin(115200);
  
  // --- DEPURACIÓN: Verificación de inicialización de serial ---
  delay(100);
  if (!Serial) {
    Serial.println(F("[ERROR] Falló la inicialización del puerto Serial"));
  } else {
    Serial.println(F("[OK] Puerto Serial inicializado correctamente"));
  }

   delay(200);

  Serial.print(F("\nStart Async_HelloServer on "));
  Serial.print(ARDUINO_BOARD);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(ASYNC_WEBSERVER_ESP32_W5500_VERSION);

  AWS_LOGWARN(F("Default SPI pinout:"));
  AWS_LOGWARN1(F("SPI_HOST:"), ETH_SPI_HOST);
  AWS_LOGWARN1(F("MOSI:"), MOSI_GPIO);
  AWS_LOGWARN1(F("MISO:"), MISO_GPIO);
  AWS_LOGWARN1(F("SCK:"),  SCK_GPIO);
  AWS_LOGWARN1(F("CS:"),   CS_GPIO);
  AWS_LOGWARN1(F("INT:"),  INT_GPIO);
  AWS_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
  AWS_LOGWARN(F("========================="));

  // --- DEPURACIÓN: Verificación de pines SPI ---
  Serial.print(F("[DEBUG] Verificando configuración de pines SPI..."));
  
  // Verificar que los pines son válidos (no configurados como GPIOs conflictivos)
  #ifdef CONFIG_IDF_TARGET_ESP32S3
    if (MOSI_GPIO == 11 && MISO_GPIO == 9 && SCK_GPIO == 7) {
      Serial.println(F("[OK] Pines SPI estándar ESP32-S3 detectados"));
    } else {
      Serial.print(F("[INFO] Usando pines personalizados: MOSI="));
      Serial.print(MOSI_GPIO); Serial.print(F(", MISO="));
      Serial.print(MISO_GPIO); Serial.print(F(", SCK="));
      Serial.print(SCK_GPIO); Serial.println(F(", CS="));
      Serial.println(CS_GPIO);
    }
  #endif

  uint16_t index =2;
  
  // --- DEPURACIÓN: Verificación de MAC address antes de iniciar ---
  Serial.print(F("[DEBUG] MAC Address para W5500: "));
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(F(":"));
    Serial.printf("%02X", mac[index][i]);
  }
  Serial.println();

  // --- INICIALIZACIÓN DEL W5500 CON VERIFICACIÓN ---
  Serial.println(F("[DEBUG] Iniciando W5500 Ethernet..."));
  
  bool ethInitSuccess = false;
  int retryCount = 0;
  const int maxRetries = 3;
  
  while (!ethInitSuccess && retryCount < maxRetries) {
    ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[index] );
    
    // Verificar estado después de cada intento usando linkUp()
    delay(100); // Esperar un poco para que el módulo se inicialice
    
    if (ETH.linkUp()) {
      ethInitSuccess = true;
      Serial.println(F("[OK] W5500 inicializado correctamente - Link UP"));
      
      // Obtener velocidad del link
      uint8_t speed = ETH.linkSpeed();
      Serial.print(F("Velocidad del link: "));
      Serial.print(speed);
      Serial.println(F(" Mbps"));
    } else {
      retryCount++;
      const char* errorMsg = "[ERROR]";
      
      if (retryCount == 1) {
        Serial.println(F("[INFO] Esperando conexión física..."));
      } else if (retryCount == 2) {
        Serial.println(F("[WARNING] Conexión lenta - verificando estado..."));
      } else {
        errorMsg = "[FATAL]";
      }
      
      Serial.print(errorMsg);
      Serial.print(F(" Intento de inicialización falló ("));
      Serial.print(retryCount);
      Serial.println(F(")"));
      
      if (retryCount < maxRetries) {
        delay(500); // Esperar antes de reintentar
      }
    }
  }
  
  if (!ethInitSuccess) {
    Serial.println(F("[FATAL] No se pudo inicializar W5500 después de múltiples intentos"));
    Serial.println(F("[SUGERENCIA] Verifique:"));
    Serial.println(F("  - Conexión física del módulo W5500 (cables SPI)"));
    Serial.println(F("  - Pines SPI configurados correctamente en el esquema"));
    Serial.println(F("  - Módulo W5500 conectado a VCC (3.3V) y GND"));
    Serial.println(F("  - Chip select (CS) pin libre del ESP32"));
    return; // Salir si falla después de todos los intentos
  }

  
  // --- DEPURACIÓN: Configuración de IP con verificación ---
  Serial.print(F("[DEBUG] Configurando dirección IP: "));
  Serial.println(myIP);
  
  bool configSuccess = ETH.config(myIP, myGW, mySN, myDNS);
  
  if (configSuccess) {
    Serial.println(F("[OK] Dirección IP configurada correctamente"));
  } else {
    Serial.println(F("[ERROR] Falló la configuración de dirección IP"));
    return;
  }

  // --- ESPERAR CONEXIÓN W5500 ---
  Serial.println(F("[DEBUG] Esperando conexión establecida con W5500..."));
  
  // Esperar un tiempo para que DHCP complete su trabajo
  delay(2000);
  
  if (ETH.linkUp()) {
    Serial.println(F("[OK] Conexión W5500 establecida correctamente"));
    
    // Información adicional del estado
    uint8_t speed = ETH.linkSpeed();
    Serial.print(F("Velocidad de conexión: "));
    Serial.print(speed);
    Serial.println(F(" Mbps"));
  } else {
    Serial.println(F("[ERROR] No se pudo establecer conexión con W5500"));
    Serial.println(F("[SUGERENCIA] Verifique:"));
    Serial.println(F("  - Módulo W5500 conectado correctamente al ESP32"));
    Serial.println(F("  - Tensión de alimentación adecuada (3.3V, no 5V)"));
    Serial.println(F("  - No hay interferencias en el bus SPI"));
    return;
  }

  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(ETH.localIP());
  
  // --- DEPURACIÓN: Información de estado final ---
  Serial.println(F("\n[DEBUG] === ESTADO FINAL DEL SISTEMA ==="));
  Serial.print(F("Estado W5500: "));
  Serial.println(ETH.linkUp() ? "OK (Link UP)" : "ERROR (Link DOWN)");
  
  // Mostrar información de red completa
  Serial.print(F("Dirección MAC: "));
  uint8_t macAddr[6];
  ETH.macAddress(macAddr);
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(F(":"));
    Serial.printf("%02X", macAddr[i]);
  }
  Serial.println();
  
  Serial.print(F("Subnet Mask: "));
  Serial.println(ETH.subnetMask());
  
  Serial.print(F("Gateway IP: "));
  Serial.println(ETH.gatewayIP());
  
  Serial.print(F("DNS Server: "));
  Serial.println(ETH.dnsIP(0));
  
  Serial.println(F("==============================="));
}
