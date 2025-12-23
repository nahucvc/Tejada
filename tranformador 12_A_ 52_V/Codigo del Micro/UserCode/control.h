/*
 * control.h
 *
 *  Created on: 18 dic 2025
 *      Author: Nahuel
 */

#ifndef CONTROL_H_
#define CONTROL_H_




#ifdef __cplusplus
#include "main.h"
extern "C" {
#endif
#include "arm_math.h"


void control_loop();
void control_break_tim_8();
void trasmitir_datos(void);
void iniciar_filtro1();
void loop_break_control();
#ifdef __cplusplus
}
#endif



/**************************************************************************************************/










#endif /* CONTROL_H_ */
