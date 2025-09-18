#include "Comunicacion.h"
#include <ctype.h>

// --- Estructura y helpers ---
struct Command {
  char cmd;    // primer carácter (comando)
  float value; // valor convertido a float
};

// Buffers de recepción independientes por puerto
static char rx0[RX_BUF_LEN];
static size_t rx0_len = 0;

static char rx1[RX_BUF_LEN];
static size_t rx1_len = 0;

// Parseo rápido: "<cmd><float>\n"  ej:  A12.34\n  ó  C1\n
static bool parse_line(const char* line, Command& out) {
  // línea esperada: al menos 2 caracteres (comando + algo de número) o 1 si booleano/0-1
  if (!line || !line[0]) return false;

  out.cmd = line[0];

  // saltar espacios
  const char* p = line + 1;
  while (*p == ' ' || *p == '\t') ++p;

  // permitir valor vacío => 0.0
  if (*p == '\0') { out.value = 0.0f; return true; }

  // strtof maneja +/-, decimales, exponenciales. Si no es número, permitir 0/1 como bool.
  char* endp = nullptr;
  float v = strtof(p, &endp);
  if (endp == p) {
    // No parseó float: admitir '0' o '1' / 'T'/'F'
    if (*p == '1' || *p == 'T' || *p == 't') v = 1.0f;
    else v = 0.0f;
  }
  out.value = v;
  return true;
}

static void update_Variables(const Command& dato) {
  switch (dato.cmd) {
    case 'C': digitalWrite(PIN_C, (bool)dato.value); break;
    case 'D': digitalWrite(PIN_D, (bool)dato.value); break;
    case 'E': digitalWrite(PIN_E, (bool)dato.value); break;
    case 'F': digitalWrite(PIN_F, (bool)dato.value); break;
    case 'G': digitalWrite(PIN_G, (bool)dato.value); break;
    case 'H': digitalWrite(PIN_H, (bool)dato.value); break;
    case 'I': digitalWrite(PIN_I, (bool)dato.value); break;
    case 'J': digitalWrite(PIN_J, (bool)dato.value); break;
    case 'K': digitalWrite(PIN_K, (bool)dato.value); break;
    case 'A': Aceleracion = dato.value; AC_Recibido = 1; break;
    case 'B': Angulo = dato.value; break;
    default: /* ignorar comando desconocido */ break;
  }
}

void inicializar_pines() {
  pinMode(PIN_C, OUTPUT); digitalWrite(PIN_C, LOW);
  pinMode(PIN_D, OUTPUT); digitalWrite(PIN_D, LOW);
  pinMode(PIN_E, OUTPUT); digitalWrite(PIN_E, LOW);
  pinMode(PIN_F, OUTPUT); digitalWrite(PIN_F, LOW);
  pinMode(PIN_G, OUTPUT); digitalWrite(PIN_G, LOW);
  pinMode(PIN_H, OUTPUT); digitalWrite(PIN_H, LOW);
  pinMode(PIN_I, OUTPUT); digitalWrite(PIN_I, LOW);
  pinMode(PIN_J, OUTPUT); digitalWrite(PIN_J, LOW);
  pinMode(PIN_K, OUTPUT); digitalWrite(PIN_K, LOW);
  pinMode(PA1, OUTPUT); digitalWrite(PA1, LOW);
  pinMode(PA0, OUTPUT); digitalWrite(PA0, LOW);
  
}

// Procesa un puerto serie sin bloquear hasta encontrar '\n'
// Devuelve true si procesó una línea completa
static bool processSerial(HardwareSerial& ser, char* buf, size_t& len) {
  while (ser.available() > 0) {
    int ch = ser.read();
    if (ch < 0) break;

    if (ch == '\r') continue;          // ignorar CR
    if (ch == '\n') {                  // fin de línea -> parsear
      buf[len] = '\0';
      if (len > 0) {
        Command cmd;
        if (parse_line(buf, cmd)) {
          update_Variables(cmd);
        }
      }
      len = 0; // reset para próxima línea
      return true;
    } else {
      if (len < (RX_BUF_LEN - 1)) {
        buf[len++] = (char)ch;
      } else {
        // overflow: reset o consumir hasta '\n'
        len = 0;
      }
    }
  }
  return false;
}

static bool processSerialUSB(USBSerial& ser, char* buf, size_t& len) {
  while (ser.available() > 0) {
    int ch = ser.read();
    if (ch < 0) break;

    if (ch == '\r') continue;          // ignorar CR
    if (ch == '\n') {                  // fin de línea -> parsear
      buf[len] = '\0';
      if (len > 0) {
        Command cmd;
        if (parse_line(buf, cmd)) {
          update_Variables(cmd);
        }
      }
      len = 0; // reset para próxima línea
      return true;
    } else {
      if (len < (RX_BUF_LEN - 1)) {
        buf[len++] = (char)ch;
      } else {
        // overflow: reset o consumir hasta '\n'
        len = 0;
      }
    }
  }
  return false;
}


void Comunicacion_Serial() {
  // Procesar ambos puertos sin bloquear
  (void)processSerialUSB (Serial,  rx0, rx0_len);
  (void)processSerial(Serial1, rx1, rx1_len);
}
