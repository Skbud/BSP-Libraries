/*
 * DHT11.h
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#endif /* INC_DHT11_H_ */

#include "main.h"
//#include "DelayMicroSec.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_8


typedef struct{
	uint8_t dhtpin;
	GPIO_TypeDef* dhtport;
}dht11_typedef_t;


void dht11_Init(dht11_typedef_t dht11);
void dht11_DeInit(dht11_typedef_t dht11);
uint8_t dht11_checkdht11status(void);
void dht11_Start(void);
float dht11_get_humidity(void);
float dht11_get_temperature(void);
uint8_t dht11_get_checksum(void);
