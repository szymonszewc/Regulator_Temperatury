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
struct value
{
	uint8_t integer;
	uint8_t decimal;
	uint16_t calculation_value;
};

/*
 * Wartoœc calkowita i dziesiêtna jako oddzielne zmienne pozwala mi przesy³ac wartoœc rzeczywist¹ w postaci 0.00 na interfejs w telefonie.
 * Do obliczeñ u¿ywam wartoœci rzeczywistej pomno¿onej razy 10
 */

void increase_value(struct value *data);

/*
 * Pozwala zwiêkszyc wartoœc struktury value
 */

void decrease_value(struct value *data);

/*
 * Pozwala zmniejszyc wartosc wybranej struktury value
 */

void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/*
 * Funkcja konfiguruj¹ca pin jako wejœcie
 */

void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/*
 * Funkcja konfiguruj¹ca pin jako wyjœcie
 */

uint8_t DHT11_start();

/*
 * Funkcja rozpoczynaj¹ca komunikacjê z czujnikiem DHT11
 */

void  DHT11_get_H(uint8_t *humidity_integer, uint8_t *humidity_decimal);

/*
 * Funkcja zwraca informacje o wilgotnoœci
 */

void  DHT11_get_T(uint8_t *temperature_integer, uint8_t *temperature_decimal);

/*
 * Funkcja zwraca informacje o temperaturze
 */

void  DHT11_checksum(uint8_t *check_sum);

#endif /* INC_DHT11_H_ */
