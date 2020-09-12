/*
 * PID.c
 *
 *  Created on: 13.06.2020
 *      Author: User
 */
#include "PID.h"
uint32_t PID (int16_t error,int16_t ierror,int16_t derror, uint16_t Kp, uint16_t Ti, uint16_t Td)
{
if(ierror>=10000)		//Wieksza wartosc powoduje blokowanie tego czlonu, gdyz uklad chlodzenia jest wysoce niewydajny
{
	ierror=10000;
}
if(ierror<(-1000))
{
	ierror=-1000;
}
uint32_t regulation=0;
if ((error*Kp + derror*Td + ierror*Ti)>0)
{
	regulation=(error*Kp + derror*Td + ierror*Ti)+6500;
}
else
{
	regulation=0;
}
if(regulation >=19999)
{
	regulation=19999;
}
if (regulation<5000)
{
	regulation=0;
}
return regulation;
}

int16_t calculate_ierror (int16_t error,int16_t last_error, int16_t last_ierror)
{
	return (last_ierror+(((error)+(last_error)))*0.5); //Co 0,5s, wystarczaj¹ce przyblizenie
}

int16_t calculate_derror(int16_t error, int16_t last_error)
{
	return((error)-(last_error))*2; //Co 0,5 sekundy
}
int16_t calculate_error(uint16_t set_value,uint16_t real_value)
{
	return (real_value-set_value);
}
