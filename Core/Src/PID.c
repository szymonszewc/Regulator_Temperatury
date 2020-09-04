/*
 * PID.c
 *
 *  Created on: 13.06.2020
 *      Author: User
 */
#include "PID.h"
int16_t PID (uint16_t set_value,int16_t ierror, uint16_t Temp, uint16_t last_T,uint16_t time,TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t Kp, uint16_t Ti, uint16_t Td)
{
int16_t error=Temp-set_value;
time=time/1000;
int16_t derror=((Temp-set_value)-(last_T-set_value));
int16_t Ierror=ierror+(((Temp-set_value)+(last_T-set_value)));
if(Ierror>=10000)		//Wieksza wartosc powoduje blokowanie tego czlonu, gdyz uklad chlodzenia jest wysoce niewydajny
{
	Ierror=10000;
}
if(Ierror<(-10000))
{
	Ierror=-10000;
}
int32_t regulation = (error*Kp + derror*Td + Ierror*Ti)+6500;
if(regulation >=19999)
	regulation=19999;
if (regulation<7000)
	regulation=0;
__HAL_TIM_SET_COMPARE(htim, Channel, regulation);
return Ierror;
}
