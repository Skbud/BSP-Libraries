/*
 * Servo_Motor.c
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

#include	"Servo_Motor.h"
#include	"main.h"

//static uint32_t	map(uint32_t value,uint32_t minI,uint32_t maxI,uint32_t minO,uint32_t maxO);
//static void generatepwm(TIM_HandleTypeDef* timer,uint32_t pulse);

///*
// * Returns the timer used based on pin and port number
// */
//static uint32_t Timer_Used(servo_config_t *timer){
//	uint32_t timerhandle;
//	uint32_t	timerused;
//	timerused=timer->timer;
//	switch (timerused){
//		case tim1:
//			timerhandle=htim1;
//			break;
//		case tim2:
//			timerhandle=htim2;
//			break;
//		case tim3:
//			timerhandle=htim3;
//			break;
//		case tim4:
//			timerhandle=htim4;
//			break;
//		case tim5:
//			timerhandle=htim5;
//			break;
//		case tim8:
//			timerhandle=htim8;
//			break;
//		case tim9:
//			timerhandle=htim9;
//			break;
//		case tim10:
//			timerhandle=htim10;
//			break;
//		case tim11:
//			timerhandle=htim11;
//			break;
//		case tim12:
//			timerhandle=htim12;
//			break;
//		case tim13:
//			timerhandle=htim13;
//			break;
//		case tim14:
//			timerhandle=htim14;
//			break;
//	}
//	return timerhandle;
//}
//
//
//static uint32_t Timer_Channel_Used(servo_config_t *timer){
//	uint32_t channel=
//}
///*
// * initilizes the struct
// */
////servo_connected_to(servo_config_t *servo,uint8_t *timer,uint8_t *port,uint8_t *pin){
////	servo->timclock=HAL_RCC_GetHCLKFreq();
////	servo->timerused=Timer_Used(&servo);
////	servo->timchannel=timerused.
////
////
////
////}
//
//
//void servo_Init(TIM_HandleTypeDef *timer, servo_config_t *servo){
//	servo->timerused=Timer_Used(&servo);
//	servo->timclock=HAL_RCC_GetHCLKFreq();
//	servo->timprescalar=timer->Init.Prescaler;
//
//
//}

void move_servo(TIM_HandleTypeDef *timer ,uint8_t angle){
	uint32_t pulsevalue=map(angle,0,180,0,2500);
	generatepwm(timer,pulsevalue);
}


/*
 * Mapping Function
 */
static uint32_t	map(uint32_t value,uint32_t minI,uint32_t maxI,uint32_t minO,uint32_t maxO){
	return ((((value - minI)*(maxO - minO))/(maxI - minI)) + minO);
}
/*
 *Generates Pwm Signal
 */
static void generatepwm(TIM_HandleTypeDef *timer,uint32_t pulse){
	timer->Instance->CCR1=pulse;
}








