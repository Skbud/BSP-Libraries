/*
 * DHT11.c
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

/*
 * Facing issue with parameter passing in form of regdef structs as of now ony bare code is
 * implemented for testing purpose
 */
#include "main.h"
#include <DHT11.h>



//#include "DelayMicroSec.h"

//void delayus(uint16_t time){
//	//To be implemented in main.c
//	__HAL_TIM_SET_COUNTER(&htim1, 0);
//		while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
//}

static uint8_t	checksum;
static uint8_t	humidity_byte1;
static uint8_t	humidity_byte2;
static uint8_t	temperature_byte1;
static uint8_t	temperature_byte2;
float humidity;
float temperature;


static uint8_t dht11_read(void);				//Private function
static void SetPin_as_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);//Private function
static void SetPin_as_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);	//Private Functions
static void gather_data(void);						//private Function
/*
 * Initilize dht11 pins
 */

/*
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

	//Set up Gpio Pin
	//SetPin_as_Output(dht11);
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
*/
void dht11_Start(void){
	//Refer dataSheet
	/*	1.pull pin low for 18000us
	 * 	2.pull the pin high for 20us
	 * 	3.Set the pin mode to input
	 */
	SetPin_as_Output(DHT11_PORT, DHT11_PIN);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
	delayus(18000); //to be implemented in main.c
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
	delayus(20);
	SetPin_as_Input(DHT11_PORT, DHT11_PIN);
}




uint8_t dht11_checkdht11status(){
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

 uint8_t dht11_read(void){
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
	 gather_data();
	 uint16_t Humiditytemp;

	 Humiditytemp=(((humidity_byte1)<<8)|(humidity_byte2));
	 humidity=(float)Humiditytemp;

	 return humidity;
}

float dht11_get_temperature(){
		//gather_data();
		uint16_t Temperaturetemp;
		Temperaturetemp=(((temperature_byte1)<<8)|(temperature_byte2));
		temperature=(float)Temperaturetemp;

		return temperature;
}

uint8_t dht11_get_checksum(){
		//gather_data();
		return checksum;
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




static void gather_data(){
	humidity_byte1=dht11_read();
	humidity_byte2=dht11_read();
	temperature_byte1=dht11_read();
	temperature_byte2=dht11_read();
	checksum=dht11_read();
}
