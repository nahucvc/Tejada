#include "Comunicacion.h"

struct Command
{
    char cmd;    // primer carácter (comando)
    float value; // valor convertido a float
};

Command parseCommand(String input)
{
    Command result;

    if (input.length() == 0)
    {
        result.cmd = '\0';
        result.value = 0.0;
        return result;
    }

    // Primer caracter como comando
    result.cmd = input.charAt(0);

    // El resto como número
    if (input.length() > 1)
    {
        String valueStr = input.substring(1);
        result.value = valueStr.toFloat(); // convierte String -> float
    }
    else
    {
        result.value = 0.0;
    }
   #ifdef COMUNICACION
        Serial.printf("Comando : %c | Valor : %f\n", result.cmd, result.value);
   #endif 

    return result;
}

void update_Variables(Command dato)
{
    switch (dato.cmd)
    {
    case 'C':
        digitalWrite(PIN_C, (bool)dato.value);
        break;
    case 'D':
        digitalWrite(PIN_D, (bool)dato.value);
        break;
    case 'E':
        digitalWrite(PIN_E, (bool)dato.value);
        break;
    case 'F':
        digitalWrite(PIN_F, (bool)dato.value);
        break;
    case 'G':
        digitalWrite(PIN_G, (bool)dato.value);
        break;
    case 'H':
        digitalWrite(PIN_H, (bool)dato.value);
        break;
    case 'I':
        digitalWrite(PIN_I, (bool)dato.value);
        break;
    case 'J':
        digitalWrite(PIN_J, (bool)dato.value);
        break;
    case 'K':
        digitalWrite(PIN_K, (bool)dato.value);
        break;
    case 'A':
        Aceleracion = dato.value;
        AC_Recibido = 1;
        break;
    case 'B':
        Angulo = dato.value;
        break;

    default:
        break;
    }
}
  
void inicializar_pines()
{
    pinMode(PIN_C,OUTPUT);
    digitalWrite(PIN_C,LOW);
    pinMode(PIN_D,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_E,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_F,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_G,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_H,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_I,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_J,OUTPUT);
     digitalWrite(PIN_C,LOW);
    pinMode(PIN_K,OUTPUT);
     digitalWrite(PIN_C,LOW);
}

void Comunicacion_Serial()
{

    if (Serial.available() > 0)
    {
        update_Variables(parseCommand(Serial.readString()));
    }
    if (Serial1.available() > 0)
    {
        update_Variables(parseCommand(Serial.readString()));
    }
}
