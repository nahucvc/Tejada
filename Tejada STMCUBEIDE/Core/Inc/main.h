/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Enable_Puente_Pin GPIO_PIN_5
#define Enable_Puente_GPIO_Port GPIOA
#define PIN_C_Pin GPIO_PIN_15
#define PIN_C_GPIO_Port GPIOA
#define PIN_D_Pin GPIO_PIN_10
#define PIN_D_GPIO_Port GPIOC
#define PIN_E_Pin GPIO_PIN_11
#define PIN_E_GPIO_Port GPIOC
#define PIN_F_Pin GPIO_PIN_3
#define PIN_F_GPIO_Port GPIOB
#define PIN_G_Pin GPIO_PIN_4
#define PIN_G_GPIO_Port GPIOB
#define PIN_H_Pin GPIO_PIN_5
#define PIN_H_GPIO_Port GPIOB
#define PIN_I_Pin GPIO_PIN_6
#define PIN_I_GPIO_Port GPIOB
#define PIN_J_Pin GPIO_PIN_7
#define PIN_J_GPIO_Port GPIOB
#define PIN_K_Pin GPIO_PIN_8
#define PIN_K_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
