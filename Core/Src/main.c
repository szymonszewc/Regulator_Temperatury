/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht.h"
#include "PID.h"
#include "value.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

char data[150];
uint8_t check_s=0,Received;
int16_t ierror=0,last_ierror=0,last_error,error=0,derror=0,last_T=0,size = 0,send_time=0,time=0;
uint32_t regulation_value=0;

struct value RH;
struct value Temp;
struct value set;
struct value Kp;
struct value Ti;
struct value Td;

/*
 * Definicja struktury value w pliku value.h
 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_SYSTICK_Callback();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void init();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1,&regulation_value,1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,SET);
  HAL_UART_Receive_IT(&huart3, &Received, 1);
  init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

last_T=(100*Temp.integer+Temp.decimal);
last_ierror=ierror;
conversion(&Temp);
conversion(&RH);
conversion(&Kp);
conversion(&Ti);
conversion(&Td);
conversion(&set);
/*
 * value.h
 */
last_T=Temp.calculation_value;
last_ierror=ierror;
last_error=error;
     if(DHT11_start())
     {
    	 DHT11_get_H(&RH.integer,&RH.decimal);
    	 DHT11_get_T(&Temp.integer,&Temp.decimal);
    	 DHT11_checksum(&check_s);
    	 /*
    	  * dht.h
    	  */
     }
    size = sprintf(data, "TEMP: %d.%d \n\rsetT: %d.%d \n\rKp: %d.%d \n\rTi: %d.%d \n\rTd: %d.%d \n\r \n\r \n\r",Temp.integer,Temp.decimal,set.integer,set.decimal,Kp.integer, Kp.decimal,Ti.integer,Ti.decimal,Td.integer,Td.decimal);
    if (time>=500)
    {
    	error=calculate_error(set.calculation_value,Temp.calculation_value);
    	ierror=calculate_ierror(error,last_error,last_ierror);
    	derror=calculate_derror(error,last_error);
    	regulation_value=PID(error,ierror,derror,Kp.calculation_value,Ti.calculation_value,Td.calculation_value);
    	time=0;
    	/*
    	 * PID.h
    	 */
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback()
{
	time++;
	send_time++;
	if(send_time>=1000)
	{
		HAL_UART_Transmit_IT(&huart3, data, size);
		send_time=0;
	}
}

/*
 * Co 1s odœwie¿a interfejs na telefonie wysy³aj¹c wiadomoœc przygotowan¹ w funkcji main
 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch (Received) {

	 case 84:
		 increase_value(&set);
	 break;

	 case 116:
		 decrease_value(&set);
	 break;
	 case 75:
		 increase_value(&Kp);
	 break;
	 case 107:
		decrease_value(&Kp);
	 break;
	 case 73:
		 increase_value(&Ti);
	 break;
	 case 105:
		 decrease_value(&Ti);
	 break;
	 case 68:
		 increase_value(&Td);
	 break;
	 case 100:
		 decrease_value(&Td);
	 break;
	 default:

	 break;
	 }
	HAL_UART_Receive_IT(&huart3, &Received, 1);
}

/*
 * Pozwala zmieniac wartoœci poszczególnych nastaw regulatora przy pomocy aplikacji na telefonie
 */

void init()
{
	  RH.integer=0;
	  RH.decimal=0;
	  Temp.integer=0;
	  Temp.decimal=0;
	  Kp.integer=4;
	  Kp.decimal=50;
	  Ti.integer=1;
	  Ti.decimal=0;
	  Td.integer=0;
	  Td.decimal=50;
	  set.integer=26;
	  set.decimal=0;
}

/*
 * Inicjalizuje pocz¹tkowe nastawy regulatora
 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
