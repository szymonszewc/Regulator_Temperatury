/*
 * DHT11.c
 *
 *  Created on: 02.06.2020
 *      Author: User
 */
#include "dht.h"

/*
 * Funkcja konfiguruj¹ca pin jako wejœcie
 */
void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}

/*
 * Funkcja konfiguruj¹ca pin jako wyjœcie
 */
void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}
/*
 * Funkcja rozpoczynaj¹ca komunikacjê z czujnikiem DHT11
 */
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
/*
 * Funkcja pobieraj¹ca informacje o wilgotnoœci
 */
void DHT11_get_H(float *RH)
{
	uint8_t i=0,rh_int=0,rh_digit=0;
	 float rh_intf=0,rh_digitf=0;
		for(i=0;i<8;i++)
		{
			while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for DHT signal to go high
			us_Delay (40);
			if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
			{
				rh_int&= ~(1<<(7-i));
			}
			else
			{
				rh_int|= (1<<(7-i));
			while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
			}
		}
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for DHT signal to go high
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
					{
						rh_digit&= ~(1<<(7-i));
					}
					else
					{
						rh_digit|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}

				rh_intf=(float)rh_int;
				rh_digitf=(float)rh_digit;
				*RH= (rh_intf+(rh_digitf/10));
}

/*
 * Funkcja pobieraj¹ca informacje o temperaturze
 */
void DHT11_get_T(float *TEMP)
{
 uint8_t i=0,temp_int=0,temp_digit=0;
 float temp_intf=0,temp_digitf=0;
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for DHT signal to go high
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
					{
						temp_int&= ~(1<<(7-i));
					}
					else
					{
						temp_int|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}
		for(i=0;i<8;i++)
		{
					while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					us_Delay (40);
					if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
					{
						temp_digit&= ~(1<<(7-i));
					}
					else
					{
						temp_digit|= (1<<(7-i));
						while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));
					}
				}
				temp_intf=(float)temp_int;
				temp_digitf=(float)temp_digit;
				*TEMP=(temp_intf+(temp_digitf/10));
}

/*
 *
 */
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
