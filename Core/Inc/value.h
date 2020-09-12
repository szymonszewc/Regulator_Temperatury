/*
 * value.h
 *
 *  Created on: 12.09.2020
 *      Author: User
 */

#ifndef INC_VALUE_H_
#define INC_VALUE_H_
#include "main.h"

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

void conversion(struct value *data);

/*
 * Pozwala modyfikowac wartosc do obliczen na podstawie czesci calkowitej i dziesietnej
 */

void increase_value(struct value *data);

/*
 * Pozwala zwiêkszyc wartoœc struktury value
 */

void decrease_value(struct value *data);

/*
 * Pozwala zmniejszyc wartosc wybranej struktury value
 */

#endif /* INC_VALUE_H_ */
