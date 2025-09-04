#include <Arduino.h>
//#include <Inicio.h>



#ifdef ESP32_ogas 
void saludar ()
{
  Serial.printf("Hola soy una esp32\n");
}
#endif

#ifndef ESP32_ogas 
void saludar ()
{
  Serial.printf("Hola soy otra placa \n");
}
#endif


uint32_t ADC_Valor=0;
void setup() {
  Serial.begin(115200);
  
  
}

void loop() {
   saludar();
  delay(100);
}

