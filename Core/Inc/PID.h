/*
 * PID.h
 *
 *  Created on: 13.06.2020
 *      Author: User
 */

#ifndef INC_PID_H_
#define INC_PID_H_
#include "tim.h"
//#define arr 100
int16_t PID (uint16_t set_value, int16_t ierror, uint16_t Temp, uint16_t last_T,uint16_t time, TIM_HandleTypeDef *htim, uint32_t Channel,uint16_t Kp,uint16_t Ti,uint16_t Td);

#endif /* INC_PID_H_ */
