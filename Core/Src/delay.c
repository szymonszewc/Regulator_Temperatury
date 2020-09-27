/*
 * DELAY.c
 *
 *  Created on: 02.06.2020
 *      Author: User
 */
#include "delay.h"
void us_Delay(uint16_t time_us)
{
		__HAL_TIM_SET_COUNTER(&htim1,0);
		while( __HAL_TIM_GET_COUNTER(&htim1) < time_us );
}

