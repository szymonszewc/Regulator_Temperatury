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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht.h"
#include "PID.h"
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
float Rh=0,Temp=0,time=0,last_T=0,Ierror=0,Kp=40,Ti=30,Td=15,set=30;
uint8_t check_s=0,n=0,T_int=0,T_digit=0,H_int=0,H_digit=0,set_int=30,set_digit=0,Kp_int=45,Kp_digit=0,Ti_int=30,Ti_digit=0,Td_int=20,Td_digit=0,Received;
char data[150];
uint16_t size = 0,regulations=0,send_time=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_SYSTICK_Callback();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void conversion();
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
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,SET);
  HAL_UART_Receive_IT(&huart3, &Received, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

last_T=Temp;
conversion();
     n= DHT11_start();
     if(n==1)
     {
    	 DHT11_get_H(&Rh);
    	 DHT11_get_T(&Temp);
    	 DHT11_checksum(&check_s);
     }
     Ierror= PID (set,Ierror,Temp,last_T, time,&htim2,TIM_CHANNEL_1,Kp,Ti,Td);
     time=0;
     size = sprintf(data, "TEMP: %d.%d \n\rsetT: %d.%d \n\rKp: %d.%d \n\rTi: %d.%d \n\rTd: %d.%d \n\r \n\r \n\r",T_int,T_digit,set_int,set_digit,Kp_int, Kp_digit,Ti_int,Ti_digit,Td_int,Td_digit);
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
void conversion()	//Przelicza wartosc float na uint8_t czesc calkowitra i dziesietna w celu przeslania obu czesci po UART w wolnej chwili uzyje struktury lub unii
{
set=(float)set_int+(((float)set_digit)/100);
Kp=(float)Kp_int+(((float)Kp_digit)/100);
Ti=(float)Ti_int+(((float)Ti_digit)/100);
Td=(float)Td_int+(((float)Td_digit)/100);
T_int=(int)Temp;
T_digit=(int)(Temp*100-(T_int*100));
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch (Received) {

	 case 84:
		 if(set_digit<90)
		 {
			 set_digit+=10;
		 }
		 else
		 {
			 set_digit=0;
			 set_int++;
		 }
	 break;

	 case 116:
	 if(set_digit>0)
	 {
		 set_digit-=10;
	 }
	 else
	 {
		 set_digit=90;
		 set_int--;
	 }
	 if (set_int<=0 && set_digit<=0)
	 {
		 set_int=0;
	 	 set_digit=0;
	 }
	 break;
	 case 75:
		 if(Kp_digit<90)
				 {
					 Kp_digit+=10;
				 }
				 else
				 {
					 Kp_digit=0;
					 Kp_int++;
				 }
	 break;
	 case 107:
		 if(Kp_digit>0)
			 {
				 Kp_digit-=10;
			 }
			 else
			 {
				 Kp_digit=90;
				 Kp_int--;
			 }
			 if (Kp_int<=0 && Kp_digit<=0)
			 {
				 Kp_int=0;
			 	 Kp_digit=0;
			 }
	 break;
	 case 73:
		 if(Ti_digit<90)
				 {
					 Ti_digit+=10;
				 }
				 else
				 {
					 Ti_digit=0;
					 Ti_int++;
				 }
	 break;
	 case 105:
		 if(Ti_digit>0)
			 {
				 Ti_digit-=10;
			 }
			 else
			 {
				 Ti_digit=90;
				 Ti_int--;
			 }
			 if (Ti_int<=0 && Ti_digit<=0)
			 {
				 Ti_int=0;
			 	 Ti_digit=0;
			 }
	 break;
	 case 68:
		 if(Td_digit<90)
				 {
					 Td_digit+=10;
				 }
				 else
				 {
					 Td_digit=0;
					 Td_int++;
				 }
	 break;
	 case 100:
		 if(Td_digit>0)
			 {
				 Td_digit-=10;
			 }
			 else
			 {
				 Td_digit=90;
				 Td_int--;
			 }
			 if (Td_int<=0 && Td_digit<=0)
			 {
				 Td_int=0;
			 	 Td_digit=0;
			 }
	 break;
	 default:

	 break;
	 }
	HAL_UART_Receive_IT(&huart3, &Received, 1);
}
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
