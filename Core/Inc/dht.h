/*
 * DHT11.h
 *
 *  Created on: 02.06.2020
 *      Author: User
 */

#ifndef INC_dht_H_
#define INC_dht_H_
#include "DELAY.h"
#include "gpio.h"
#include <stdlib.h>
#define DHT11_PORT GPIOC
#define DHT11_PIN GPIO_PIN_3



void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/*
 * Funkcja konfigurująca pin jako wejście
 */

void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/*
 * Funkcja konfigurująca pin jako wyjście
 */

uint8_t DHT11_start();

/*
 * Funkcja rozpoczynająca komunikację z czujnikiem DHT11
 */

void  DHT11_get_H(uint8_t *humidity_integer, uint8_t *humidity_decimal);

/*
 * Funkcja zwraca informacje o wilgotności
 */

void  DHT11_get_T(uint8_t *temperature_integer, uint8_t *temperature_decimal);

/*
 * Funkcja zwraca informacje o temperaturze
 */

void  DHT11_checksum(uint8_t *check_sum);

#endif /* INC_DHT11_H_ */
