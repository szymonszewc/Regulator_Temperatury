/*
 * PID.c
 *
 *  Created on: 13.06.2020
 *      Author: User
 */
#include "PID.h"
/*
 * Wartosc regulation jest to wartosc wpiusywana poprzez DMA do rejestru CCR timera odpowiedzialnego za generowanie sygnalu PWM do silnika naedzajacego wiatrak
 * Wartoœci w obliczeniach zwracanej wartoœci regulacji zwi¹zane s¹ z ustawieniem uzywanego timera
 * AAR=20000
 * PSC=6
 * jezeli ierror przekroczy 1000 powoduje to dlugi czas zwloki dzialania wiatraka dlatego jest to maksymalna wartosc jaka dopuszczam w programie
 * Wartosc 6500 dodaje dla natychmiastowego rozruchu silnika
 *
 */
uint32_t pid(int16_t error,int16_t ierror,int16_t derror, uint16_t Kp, uint16_t Ti, uint16_t Td)
{
		if ( ierror >= 1000 ) {		//Wieksza wartosc powoduje blokowanie tego czlonu, gdyz uklad chlodzenia jest wysoce niewydajny
				ierror = 1000;
		}
		if ( ierror < (-1000) ) {
				ierror = -1000;
		}
		uint32_t regulation=0;
		if ( (error*Kp + derror*Td + ierror*Ti) > 0 ) {
				regulation = ( error*Kp + derror*Td + ierror*Ti ) + 6500;
		} else {
				regulation = 0;
		}
		if ( regulation >= 19999 ) {
				regulation = 19999;
		}
		if ( regulation < 7000 ) { //Warunek powoduje wpisanie do CCR wartosci innej od 0 tylko jesli przekroczy ona orientacyjna wartosc rozruchu silnika (unikniecie piszczenia przy rozruchu)
				regulation = 0;
		}
		return regulation;
}


int16_t pid_calculate_ierror (int16_t error,int16_t last_error, int16_t last_ierror)
{
		return (last_ierror + (((error) + (last_error))) * 0.5); //Co 0,5s, wystarczaj¹ce przyblizenie
}


int16_t pid_calculate_derror(int16_t error, int16_t last_error)
{
		return ((error) - (last_error)) * 2; //Co 0,5 sekundy
}
int16_t pid_calculate_error(uint16_t set_value,uint16_t real_value)
{
		return (real_value - set_value);
}
