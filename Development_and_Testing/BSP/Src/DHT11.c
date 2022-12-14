/*
 * DHT11.c
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

/*USAGE:-
 * step1: Create deayyus function  using timers in main.c and give its prototype in main.h
 * step2: Call the init function
 * step3: call the gatherdata() every time you read the sensor
 * step4: Get results using gettemperature()/gethumidity()
 *
 */
#include "main.h"
#include <DHT11.h>




//Create delay in micro Seconds
//void delayus(uint16_t time){
//	//To be implemented in main.c
//	__HAL_TIM_SET_COUNTER(&htim1, 0);
//		while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
//}

//private variables
static uint8_t	checksum;
static uint8_t	humidity_byte1;
static uint8_t	humidity_byte2;
static uint8_t	temperature_byte1;
static uint8_t	temperature_byte2;
static uint8_t	status;


//Private functions
static void dht11_Start(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
static uint8_t dht11_checkdht11status(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
static uint8_t dht11_read(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN);
static void SetPin_as_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void SetPin_as_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/*
 * Initilize dht11 pins
 */
void dht11_Init(dht11_typedef_t dht11){
	// Enable gpio Clock
	GPIO_TypeDef* gpio=dht11.dhtport;
	if(gpio==GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(gpio==GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(gpio==GPIOC){
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(gpio==GPIOD){
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(gpio==GPIOE){
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if(gpio==GPIOF){
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if(gpio==GPIOG){
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if(gpio==GPIOH){
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
	else if(gpio==GPIOI){
		__HAL_RCC_GPIOI_CLK_ENABLE();
	}
	else if(gpio==GPIOJ){
		__HAL_RCC_GPIOJ_CLK_ENABLE();
	}
	else if(gpio==GPIOK){
		__HAL_RCC_GPIOK_CLK_ENABLE();
	}

}

void dht11_DeInit(dht11_typedef_t dht11){
	GPIO_TypeDef* gpio=dht11.dhtport;
		if(gpio==GPIOA){
			__HAL_RCC_GPIOA_CLK_DISABLE();
		}
		else if(gpio==GPIOB){
			__HAL_RCC_GPIOB_CLK_DISABLE();
		}
		else if(gpio==GPIOC){
			__HAL_RCC_GPIOC_CLK_DISABLE();
		}
		else if(gpio==GPIOD){
			__HAL_RCC_GPIOD_CLK_DISABLE();
		}
		else if(gpio==GPIOE){
			__HAL_RCC_GPIOE_CLK_DISABLE();
		}
		else if(gpio==GPIOF){
			__HAL_RCC_GPIOF_CLK_DISABLE();
		}
		else if(gpio==GPIOG){
			__HAL_RCC_GPIOG_CLK_DISABLE();
		}
		else if(gpio==GPIOH){
			__HAL_RCC_GPIOH_CLK_DISABLE();
		}
		else if(gpio==GPIOI){
			__HAL_RCC_GPIOI_CLK_DISABLE();
		}
		else if(gpio==GPIOJ){
			__HAL_RCC_GPIOJ_CLK_DISABLE();
		}
		else if(gpio==GPIOK){
			__HAL_RCC_GPIOK_CLK_DISABLE();
		}
}

void dht11_Start(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN){
	//Refer dataSheet
	/*	1.pull pin low for 18000us
	 * 	2.pull the pin high for 20us
	 * 	3.Set the pin mode to input
	 */
	SetPin_as_Output(DHT11_PORT, DHT11_PIN);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
	delayus(18000);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
	delayus(20); //to be implemented in main.c
	SetPin_as_Input(DHT11_PORT, DHT11_PIN);
}




uint8_t dht11_checkdht11status(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN){
	/*
	 * check if dht11 pulls pin low for 80us & high for 80us
	 */
	uint8_t	status=0;
	delayus(40);
	if(!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))
	{
		delayus(80);
		if((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))){
			status=HAL_OK;
		}
		else{
			status=HAL_ERROR;
		}
	}
	while((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))); //wait for pin to go low

	return status;
}

static uint8_t dht11_read(GPIO_TypeDef* DHT11_PORT, uint16_t DHT11_PIN){
	/* 50us to start a bit
	 * 26-28us of high pulse means logic Zero
	 * 70us of high pulse means logic One
	 */
	uint8_t tempdata=0;
	for(int i=0;i<8;i++){
		while(!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))); //wait for pin to go high
		delayus(40);
		if(!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))){
			tempdata&=~(1<<(7-i));	//write zero
		}
		else{
			tempdata|=(1<<(7-i));	//write one
		}
		while((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))); //wait for pin to go low
	}
	return	tempdata;
}


float dht11_get_humidity(){
//	if(humidity_byte1+humidity_byte2+temperature_byte1+temperature_byte2==checksum)
//	 	{
//		humidity=(float)humidity_byte1+(float)(humidity_byte2/10.0);//check if valid data is received
//		return humidity;
//	 	}
//	else{
//		return HAL_ERROR;
//		}
	float humidity=0.0;
	humidity=(float)humidity_byte1+(float)(humidity_byte2/10.0);
	return humidity;
}

float dht11_get_temperature(){
//	if(humidity_byte1+humidity_byte2+temperature_byte1+temperature_byte2==checksum)
//		{
//		temperature=(float)temperature_byte1+(float)(temperature_byte2/10.0); //check if valid data is received
//		return temperature;
//		}
//	else{
//		return HAL_ERROR;
//		}

	float temperature=0.0;
	temperature=(float)temperature_byte1+(float)(temperature_byte2/10.0);
	return temperature;
}

uint8_t dht11_get_checksum(){
		return checksum;
}



uint8_t gather_data(dht11_typedef_t dht11){
	dht11_Start(dht11.dhtport, dht11.dhtpin);
	status=dht11_checkdht11status(dht11.dhtport, dht11.dhtpin);
	humidity_byte1=dht11_read(dht11.dhtport, dht11.dhtpin);
	humidity_byte2=dht11_read(dht11.dhtport, dht11.dhtpin);
	temperature_byte1=dht11_read(dht11.dhtport, dht11.dhtpin);
	temperature_byte2=dht11_read(dht11.dhtport, dht11.dhtpin);
	checksum=dht11_read(dht11.dhtport, dht11.dhtpin);
	if(status==HAL_OK){
		return HAL_OK;
	}
	else{
		return HAL_ERROR;
	}
}


static void SetPin_as_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void SetPin_as_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}



