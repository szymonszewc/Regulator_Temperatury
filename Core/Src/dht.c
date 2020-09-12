/*
 * DHT11.c
 *
 *  Created on: 02.06.2020
 *      Author: User
 */
#include "dht.h"




void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}


void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}


uint8_t DHT11_start()
{
	set_pin_output(GPIOC,GPIO_PIN_3);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,RESET);
	us_Delay(18000);
	HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,SET);
	us_Delay(20);
	HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,RESET);
	set_pin_input(DHT11_PORT,DHT11_PIN);
	//us_Delay(10);
 uint8_t response=0;
	if(!HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN))

			{
				us_Delay(80);
				if(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN))
					response=1;
				else
					response=0;
			}
		us_Delay(80);
	return response;
}



void DHT11_get_H(uint8_t *humidity_integer, uint8_t *humidity_decimal)
{
	uint8_t i=0;
		for(i=0;i<8;i++)
		{
			while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for DHT signal to go high
			us_Delay (40);
			if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
			{
				*humidity_integer&= ~(1<<(7-i));
			}
			else
			{
				*humidity_integer|= (1<<(7-i));
			while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
			}
		}
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for DHT signal to go high
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
					{
						*humidity_decimal&= ~(1<<(7-i));
					}
					else
					{
						*humidity_decimal|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}
}


void DHT11_get_T(uint8_t *temperature_integer, uint8_t *temperature_decimal)
{
 uint8_t i=0;
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for DHT signal to go high
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
					{
						*temperature_integer&= ~(1<<(7-i));
					}
					else
					{
						*temperature_integer|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
					{
						*temperature_decimal&= ~(1<<(7-i));
					}
					else
					{
						*temperature_decimal|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}

}

void DHT11_checksum(uint8_t *check_sum)
{
 uint8_t i=0,checksum=0;
			for(i=0;i<8;i++)
			{
						//us_Delay (50);   // wait for DHT signal to go high
						while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
						us_Delay (40);
						if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
						{
							checksum&= ~(1<<(7-i));   // write 0
						}
						else
						{
							checksum|= (1<<(7-i)); // if the pin is high, write 1
					//	us_Delay (42);
							while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
						}
					}
		    *check_sum= checksum;
}
