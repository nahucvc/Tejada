#ifndef PINSALIDA_H
#define PINSALIDA_H


#ifdef LOLI 
#define ADCpin 9
#define PWMDeracha 16
#define PwmIzquierda 21
#define EnableDerecha 33 //enable1
#define EnableIzquierda 35 //enable 2
#define AnguloMaximo 50
#define Ganacia 2
#define _Adelante 37
#define _Atraz 39
#define _Acelerar 18
#endif

#ifdef WOROOM
#define ADCpin 34
#define PWMDeracha 32
#define PwmIzquierda 33
#define EnableDerecha 23
#define EnableIzquierda 22
#define AnguloMaximo 50
#define Ganacia 2
#define _Adelante 21
#define _Atraz 19
#define _Acelerar 25
#endif

// lectura maxima a la izquierda 1900
// lectura maxima a la derecha 3100
// medio 2585 

#endif