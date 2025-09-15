#include "Inicio.h"

//Variables Globales
uint32_t ValorADC;
uint32_t ADC_kalman;
KalmanFilter k1(0/*valor inicial*/,1 /*incertidumbre inicial*/,0.2 /*Ganacia de Kalman inicial*/ ,5/*Varianza de la medición*/); //objeto con los metodos y varibles necesarios para el filtro

// Función para configurar el clock del microcontrolador
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}
// Función de llamada para cuando se termina de convertir el adc
void ADC_ConvCpltCallback(__ADC_HandleTypeDef* hadc)
{
  ValorADC= HAL_ADC_GetValue(&hadc1);
  k1.update(ValorADC);
  ADC_kalman = k1.getState();
  ADC1_2_IRQHandler(); 
}

void TIM_periodicCallback (__TIM_HandleTypeDef *htim)
{
  Control();
}

// Funcion  para configurar los perifericos
void InicioConfig()
{

  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();
  MX_TIM7_Init();
  hadc1.ConvCpltCallback = ADC_ConvCpltCallback;
  htim7.PeriodElapsedCallback= TIM_periodicCallback;
  HAL_ADC_Start_IT(&hadc1);
  HAL_TIM_Base_Start(&htim8);
  HAL_TIM_Base_Start_IT(&htim7);
  MX_DAC1_Init();
  HAL_DAC_Start(&hdac1,DAC_CHANNEL_1 );
  
}

//funcion para actualizar el acelerador

void update_Aceleracion(float ac)
{

HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1,DAC_ALIGN_12B_R, (uint16_t) (4096/ac));

}