/*
 * DelayMicroSec.c
 *
 *  Created on: Sep 26, 2022
 *      Author: Sanjay
 */


#include "main.h"
#include "DelayMicroSec.h"

void delayus(uint16_t time){
	//To be implemented in main.c
	__HAL_TIM_SET_COUNTER(&htim1, 0);
		while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}
