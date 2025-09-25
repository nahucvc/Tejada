
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet_Generic.h>
#include <servidor.h>

#define USE_DHCP 0 // 1 = DHCP (con timeouts), 0 = IP estática
HardwareSerial Myserial(1);
float angulo = 90;
float aceleracion = 0;
// Pines W5500 (ajustá si es necesario)
#define W5500_SCK 7
#define W5500_MISO 9
#define W5500_MOSI 11
#define W5500_CS 12
#define W5500_RST 5

// MACs
#define MAC_BYTES 6
byte mac[][MAC_BYTES] =
    {
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05},
};

// IP estática (solo si USE_DHCP == 0)
IPAddress ETH_IP(192, 168, 0, 4);
IPAddress ETH_GW(192, 168, 0, 1);
IPAddress ETH_NETMASK(255, 255, 255, 0);
IPAddress ETH_DNS(8, 8, 8, 8);

const char* SSID_AP     = "Auto";
const char* PASS_AP     = "123456789";   // mínimo 8 caracteres (WPA2)

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway (192, 168, 4, 1);
IPAddress subnet  (255, 255, 255, 0);

// -------------------- Utilidad: arranque bloqueante del W5500 --------------------
static bool waitHardware(uint32_t hwTimeoutMs)
{
  uint32_t t0 = millis();
  while (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    if (millis() - t0 > hwTimeoutMs)
    {
      Serial.println(F("[ETH] ERROR: No se detecta hardware W5500. Revisar cableado/CS/RST/Alimentación."));
      return false;
    }
    delay(50);
  }
  Serial.println(F("[ETH] Hardware W5500 detectado."));
  return true;
}

static bool waitLink(uint32_t linkTimeoutMs)
{
  uint32_t t0 = millis();
  while (Ethernet.linkStatus() != LinkON)
  {
    if (millis() - t0 > linkTimeoutMs)
    {
      Serial.println(F("[ETH] ERROR: Link DOWN. Verifique cable UTP/puerto de switch/MDI-X."));
      return false;
    }
    Serial.println(F("[ETH] Link esperando..."));
    delay(250);
  }
  return true;
}

bool startW5500_Blocking(uint8_t macIndex, uint32_t hwTimeoutMs = 3000, uint32_t linkTimeoutMs = 15000)
{
  // 1) Reset duro del W5500
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW);
  delay(20); // 10–50 ms
  digitalWrite(W5500_RST, HIGH);
  delay(200); // 150–200 ms

  // 2) Inicializar SPI con tus pines remapeados
  SPI.begin(W5500_SCK, W5500_MISO, W5500_MOSI, W5500_CS);
  SPI.setFrequency(20000000); // hasta 25 MHz; bajá si hay ruido
  Ethernet.init(W5500_CS);

  Serial.println(F("[ETH] Iniciando W5500..."));

  // 3) Elegir DHCP o IP estática usando las SOBRECARGAS válidas
#if USE_DHCP
  {
    const unsigned long dhcpTimeoutMs = 8000;
    const unsigned long dhcpResponseTimeoutMs = 4000;

    // Sobrecarga válida: begin(mac, SPIClass*, timeout, responseTimeout) -> DHCP
    int ret = Ethernet.begin((uint8_t *)mac[macIndex], &SPI, dhcpTimeoutMs, dhcpResponseTimeoutMs);
    if (ret != 1)
    {
      Serial.println(F("[ETH] ERROR: DHCP falló."));
      return false;
    }
  }
#else
  {
    // Sobrecarga válida para estática: begin(mac, ip, dns, gateway, subnet)
    Ethernet.begin((uint8_t *)mac[macIndex], ETH_IP, ETH_DNS, ETH_GW, ETH_NETMASK);
  }
#endif

  // 4) Verificar hardware y link
  if (!waitHardware(hwTimeoutMs))
    return false;
  if (!waitLink(linkTimeoutMs))
    return false;

  // 5) Confirmación
  Serial.print(F("[ETH] OK. IP: "));
  Serial.println(Ethernet.localIP());
  Serial.print(F("[ETH] GW: "));
  Serial.println(Ethernet.gatewayIP());
  Serial.print(F("[ETH] DNS: "));
  Serial.println(Ethernet.dnsServerIP());
  Serial.print(F("[ETH] Subnet: "));
  Serial.println(Ethernet.subnetMask());
  return true;
}

void setup()
{
  Serial.begin(115200);
  Myserial.begin(115200, SERIAL_8N1, 37, 38);
  Serial.println(F("\n[BOOT] Arrancando..."));
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // softAP(ssid, pass, canal, oculto, max_conexiones)
  bool ok = WiFi.softAP(SSID_AP, PASS_AP, 6, false, 4);
  Serial.println(ok ? "AP levantado OK" : "Error al iniciar AP");
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP());
  IniciarServidor();
  if (!startW5500_Blocking(0))
  {
    Serial.println(F("[BOOT] Falla en W5500. Sistema detenido."));
    while (true)
    {
      delay(1000);
    }
  }
  

  // IP del AP (opcional: si no lo ponés, usa 192.168.4.1 por defecto)
  
  Ethernet.begin(mac[5],&SPI, (int) 200,(int) 200);
  Ethernet.setLocalIP(ETH_IP);
  Ethernet.setSubnetMask(subnet);
  
  Serial.println(F("[BOOT] W5500 listo. Iniciando servidor..."));

  
}

void loop()
{
  
}
