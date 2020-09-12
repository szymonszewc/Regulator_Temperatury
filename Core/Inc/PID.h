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
uint32_t PID (int16_t error,int16_t ierror,int16_t derror, uint16_t Kp, uint16_t Ti, uint16_t Td);
int16_t calculate_ierror(int16_t error,int16_t last_error, int16_t last_ierror);
int16_t calculate_derror(int16_t error, int16_t last_error);
int16_t calculate_error(uint16_t set_value,uint16_t real_value);
#endif /* INC_PID_H_ */
