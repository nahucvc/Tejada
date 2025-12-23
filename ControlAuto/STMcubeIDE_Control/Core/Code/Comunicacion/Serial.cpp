/*
 * Serial.cpp
 *
 *  Created on: Oct 10, 2025
 *      Author: Nahuel
 */

#include "Serial.h"
#include "SerialUSB.h"
#include "UserCode.h"
#include "Direccion.h"

struct Command
{
  char cmd;    // primer carácter (comando)
  float value; // valor convertido a float
};


static void update_Variables(const Command &dato)
{
  switch (dato.cmd)
  {
  case 'C':
    HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'D':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'E':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'F':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'G':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'H':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'I':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'J':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET) ;
    break;
  case 'K':
	  HAL_GPIO_WritePin(PIN_C_GPIO_Port, PIN_C_Pin, (dato.value==0)? GPIO_PIN_RESET:GPIO_PIN_SET);
    break;
  case 'A':
    Aceleracion = dato.value;
    AC_Recibido = 1;
    break;
  case 'B':
    Angulo = dato.value;
    break;
  default: /* ignorar comando desconocido */
    break;
  }
}



bool parseCmdVal(uint8_t *buf, size_t len, char &outCmd, float &outVal) {
  if (!buf || len == 0) return false;

  auto isSpace = [](char c){ return c==' '||c=='\t'||c=='\r'||c=='\n'; };
  auto isDigit = [](char c){ return c>='0' && c<='9'; };

  char valBuf[32];          // suficiente para "-123.456e-7"
  size_t valLen = 0;

  for (size_t i = 0; i < len; ++i) {
    char c = (char)buf[i];
    if (c == 0) continue;           // ignorar huecos/vacíos
    if (c != '{') continue;         // buscamos inicio

    size_t j = i + 1;

    // saltar espacios
    while (j < len && isSpace((char)buf[j])) j++;
    if (j >= len) break;

    // un solo carácter de comando
    char cmd = (char)buf[j];
    if (cmd == '{' || cmd == '}' || cmd == ':' || isSpace(cmd) || cmd == 0) {
      continue; // no es válido, seguimos buscando otro '{'
    }
    j++;

    // espacios antes de ':'
    while (j < len && isSpace((char)buf[j])) j++;
    if (j >= len || (char)buf[j] != ':') continue;
    j++;

    // espacios antes del valor
    while (j < len && isSpace((char)buf[j])) j++;
    if (j >= len) break;

    // leer valor hasta '}' (acepta + - . dígitos y e/E)
    valLen = 0;
    bool okChars = true;

    for (; j < len; ++j) {
      char ch = (char)buf[j];
      if (ch == 0) break;
      if (ch == '}') {
        break; // fin de objeto
      }
      if (isSpace(ch)) {
        // ignoramos espacios en el valor
        continue;
      }
      if (!(isDigit(ch) || ch == '+' || ch == '-' || ch == '.' || ch == 'e' || ch == 'E')) {
        okChars = false; // caracter inesperado dentro del valor
        break;
      }
      if (valLen < sizeof(valBuf) - 1) {
        valBuf[valLen++] = ch;
      } else {
        okChars = false; // overflow de valor
        break;
      }
    }

    if (!okChars) continue;

    // Debe terminar en '}'
    if (j >= len || (char)buf[j] != '}') continue;

    // Cerrar cadena de valor
    valBuf[valLen] = '\0';

    // Validar que haya al menos un dígito
    bool hasDigit = false;
    for (size_t k = 0; k < valLen; ++k) {
      if (isDigit(valBuf[k])) { hasDigit = true; break; }
    }
    if (!hasDigit) continue;

    // Convertir a float
    char *endp = nullptr;
    float v = strtof(valBuf, &endp);
    if (endp == valBuf) continue;   // no convirtió nada
    while (*endp == ' ' || *endp == '\t' || *endp == '\r' || *endp == '\n') endp++;
    if (*endp != '\0') continue;    // basura al final

    // ÉXITO
    outCmd = cmd;
    outVal = v;
    HAL_UART_DMAStop(&huart1);                         // detiene el stream RX
    memset(buf, 0, len);            // borrar el array completo
    HAL_UART_Receive_DMA(&huart1, buffer, sizeof buffer);  // rearma desde 0
    return true;
  }

  return false; // no se encontró objeto válido
}


void loopCom()
{
	static char CMD;
	static float var;
	if(parseCmdVal(buffer, 60, CMD, var))
	{
     Serial.printf("Comado: %c | Valor: %f \n",CMD,var);
     update_Variables({CMD, var});
	}

}


