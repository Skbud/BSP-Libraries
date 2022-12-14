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

typedef struct{
	uint16_t dhtpin;
	GPIO_TypeDef* dhtport;
}dht11_typedef_t;


void dht11_Init(dht11_typedef_t dht11);
void dht11_DeInit(dht11_typedef_t dht11);
uint8_t gather_data(dht11_typedef_t dht11);
float dht11_get_humidity(void);
float dht11_get_temperature(void);
uint8_t dht11_get_checksum(void);



