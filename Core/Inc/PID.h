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
float PID (float set_value, float ierror, float Temp, float last_T,float time, TIM_HandleTypeDef *htim, uint32_t Channel,float Kp,float Ti,float Td);

#endif /* INC_PID_H_ */
