/*
 * PID.c
 *
 *  Created on: 13.06.2020
 *      Author: User
 */
#include "PID.h"
float PID (float set_value,float ierror, float Temp, float last_T,float time,TIM_HandleTypeDef *htim, uint32_t Channel, float Kp, float Ti, float Td)
{
float error=Temp-set_value;
time=time/1000;
float derror=((Temp-set_value)-(last_T-set_value))/time;
float Ierror=ierror+(((Temp-set_value)+(last_T-set_value))*time);
if(Ierror>=100)		//Wieksza wartosc powoduje blokowanie tego czlonu, gdyz uklad chlodzenia jest wysoce niewydajny
{
	Ierror=100;
}
if(Ierror<(-100))
{
	Ierror=-100;
}
uint16_t regulation = (error*Kp + derror*Td + Ierror*Ti)+65;
if(regulation >=199)
	regulation=199;
if (regulation<70)
	regulation=0;
__HAL_TIM_SET_COMPARE(htim, Channel, regulation);
return Ierror;
}
