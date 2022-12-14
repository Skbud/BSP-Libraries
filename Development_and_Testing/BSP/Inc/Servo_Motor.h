/*
 * Servo_Motor.h
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */
/*
 * Important Note!
 * This library uses Hal_Tim drivers for its operation which require low level initilization of
 * timer peripherals in hal_msp.c so to use this library, proceed with following steps;
 * 1.Configure the timer in pwm mode with 20ms period using your code/ CubeMx
 * 2.Start the timer in PWM
 * 3.use move_servo  functions to  move the servo.
 */
#ifndef INC_SERVO_MOTOR_H_
#define INC_SERVO_MOTOR_H_

#include	"main.h"

//static typedef struct{
//	uint32_t			timerused;			//timer used to generate pwm
//	 uint32_t			timchannel;		//timer channel to which servo is connected
//	static uint32_t		timclock;		//Clock source for the timer
//	static uint32_t		timprescalar;	//Timer prescalar
//	uint32_t			timperiod;		//timer period(ARR)
//	uint32_t			tim_pulse;		//timer pulse(CCR1)
//}servo_config_t;

//void servo_connected_to(servo_config_t *servo,uint8_t *timer,uint8_t *port,uint8_t *pin);
//void servo_Init(TIM_HandleTypeDef *timer,servo_connected_at_t *servo);
//void servo_Deinit(servo_connected_at_t *servo);
void move_servo(TIM_HandleTypeDef *timer ,uint8_t angle);

//static uint8_t get_Timer_handle(void);
//static uint8_t Timer_Channel_Used(void);
static uint32_t	map(uint32_t value,uint32_t minI,uint32_t maxI,uint32_t minO,uint32_t maxO);
static void generatepwm(TIM_HandleTypeDef* timer,uint32_t pulse);

#endif /* INC_SERVO_MOTOR_H_ */
