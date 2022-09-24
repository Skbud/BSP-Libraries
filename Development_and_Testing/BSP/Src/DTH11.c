/*
 * DTH11.c
 *
 *  Created on: 24-Sep-2022
 *      Author: Sanjay
 */

#include	"main.h"
#include 	"DTH11.h"


uint32_t delayus(uint8_t delay){
	//dummy implemetation
}

static uint8_t dht11_read(void);				//Private function
static SetPin_as_Output(dht11_typedef_t* dht11);//Private function
static SetPin_as_Input(dht11_typedef_t* dht11);	//Private Functions
static gather_data(void);						//private Function
/*
 * Initilize dht11 pins
 */
void dht11_Init(dht11_typedef_t* dht11){
	/* Enable gpio Clock */
	uint8_t gpio=dht11->dthport;
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

	/*Set up Gpio Pin*/
	SetPin_as_Output(dht11);
}

void dht11_DeInit(dht11_typedef_t* dht11){
	uint8_t gpio=dht11->dthport;
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

void dht11_Start(dht11_typedef_t* dht11){
	//Refer dataSheet
	/*	1.pull pin low for 18us
	 * 	2.pull the pin high for 30us
	 * 	3.Set the pin mode to input
	 */
	HAL_GPIO_WritePin(dht11->dthport, dht11->dthpin, GPIO_PIN_RESET);
	delayus(18); //to be implemented in main.c
	HAL_GPIO_WritePin(dht11->dthport, dht11->dthpin, GPIO_PIN_SET);
	delayus(30);
	SetPin_as_Input(dht11);
}


uint8_t checkdht11status(dht11_typedef_t* dht11){
	/*
	 * check if dht11 pulls pin low for 80us & high for 80us
	 */
	delayus(40);
	if(!(HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin)))
	{
		delayus(80);
		if(HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin)){
			dht11_data.status=HAL_OK;
		}
		else{
			dht11_data.status=HAL_ERROR;
		}
	}
	while(HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin)); //wait for pin to go low

	return dht11_data.status;
}

float dht11_get_humidity(void){
	 gather_data();
	 uint16_t Humiditytemp;
	 float humidity;
	 Humiditytemp=(((dht11_data.humidity_byte1)<<8)|(dht11_data.humidity_byte2));
	 humidity=(float)Humiditytemp;

	 return humidity;
}

float dht11_get_temperature(void){
		gather_data();
		uint16_t Temperaturetemp;
		float temperature;
		Temperaturetemp=(((dht11_data.temperature_byte1)<<8)|(dht11_data.temperature_byte2));
		temperature=(float)Temperaturetemp;

		return temperature;
}

uint8_t dht11_get_checksum(void){
		gather_data();
		return dht11_data.checksum;
}

static SetPin_as_Input(dht11_typedef_t* dht11){
	GPIO_InitTypeDef Gpiodht11;
	Gpiodht11.Pin=dht11->dthpin;
	Gpiodht11.Mode=GPIO_MODE_INPUT;
	Gpiodht11.Pull=GPIO_PULLUP;
	Gpiodht11.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dht11->dthport, &Gpiodht11);
}

static SetPin_as_Output(dht11_typedef_t* dht11){
	GPIO_InitTypeDef Gpiodht11;
	Gpiodht11.Pin=dht11->dthpin;
	Gpiodht11.Mode=GPIO_MODE_OUTPUT_PP;
	Gpiodht11.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dht11->dthport, &Gpiodht11);
}

static uint8_t dht11_read(dht11_typedef_t* dht11){
	/* 50us to start a bit
	 * 26-28us of high pulse means logic Zero
	 * 70us of high pulse means logic One
	 */
	dht11_data.tempdata=0;
	for(int i=0;i<8;i++){
		while(!(HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin))); //wait for pin to go high
		delayus(40);
		if(!(HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin))){
			(dht11_data.tempdata)&=~(1<<(7-i));	//write zero
		}
		else{
			(dht11_data.tempdata)|=(1<<(7-i));	//write one
		}
		while((HAL_GPIO_ReadPin(dht11->dthport, dht11->dthpin))); //wait for pin to go low
	}

	return	dht11_data.tempdata;
}

static gather_data(void){
	dht11_data.humidity_byte1=dht11_read(dht11);
	dht11_data.humidity_byte2=dht11_read(dht11);
	dht11_data.temperature_byte1=dht11_read(dht11);
	dht11_data.temperature_byte2=dht11_read(dht11);
	dht11_data.checksum=dht11_read(dht11);
}
