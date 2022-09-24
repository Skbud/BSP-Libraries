/*
 * DTH11.h
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

#ifndef INC_DTH11_H_
#define INC_DTH11_H_

#endif /* INC_DTH11_H_ */

#include "main.h"


typedef struct{
	uint8_t dthpin;
	uint8_t dthport;
}dht11_typedef_t;

static typedef struct{
	uint8_t	status;
	uint8_t	tempdata;
	uint8_t	humidity_byte1;
	uint8_t	humidity_byte2;
	uint8_t	temperature_byte1;
	uint8_t	temperature_byte2;
	uint8_t	checksum;
}dht11_private_t;

dht11_private_t	dht11_data;

void dht11_Init(dht11_typedef_t* dht11);
void dht11_DeInit(dht11_typedef_t* dht11);
uint8_t checkdht11status(dht11_typedef_t* dht11);
void dht11_Start(dht11_typedef_t* dht11);
float dht11_get_humidity(void);
float dht11_get_temperature(void);
uint8_t dht11_get_checksum(void);
