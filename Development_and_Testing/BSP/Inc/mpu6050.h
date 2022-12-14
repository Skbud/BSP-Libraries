/*
 * mpu6050.h
 *
 *  Created on: 30-Sep-2022
 *      Author: Sanjay
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_


#include"main.h"

/*I2c Handle address*/
//extern hi2c3;
//I2C_HandleTypeDef hi2c=hi2c3;

#define MPU6050ADDR		0x68<<1

//uint16_t MPU6050ADDR=0x68;

typedef struct{
	uint8_t mpu6050_Clk_Src;	//@ref Clk_Src
	uint8_t SampPreSc;			/*Sampling prescalar @ref datasheet
								Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
								*/
	uint8_t	DLPF;				//Digital Low Pass Filter @ref DLPF_options
	uint8_t gyroFSR;			//Gyro full scale range	@ref GYRO_FS
	uint8_t	accFSR;				//Acc full scale range @ref	ACC_FS
	uint8_t	enableFIFO;			//@ref	enableFIFO
}mpu6050_t;


//typedef struct{
//	mpu6050_t	userConfig;
//	float		GyroXreading;
//	float		GyroYreading;
//	float		GyroZreading;
//	float		GyroXreading;
//	float		AccXreading;
//	float		AccYreading;
//	float		AccZreading;
//	float		TempSenreading;	//temperature sensor readings
//}mpu_data_t;


/*
 * Clk_Src options
 */
#define	INT_OSC			0 	//Internal 8MHz oscillator
#define	PLL_X_GYRO		1 	//PLL with X axis gyroscope reference
#define	PLL_Y_GYRO		2 	//PLL with Y axis gyroscope reference
#define	PLL_Z_GYRO		3 	//PLL with Z axis gyroscope reference
#define	EXT32KH			4 	//PLL with external 32.768kHz reference
#define	EXT19MH			5 	//PLL with external 19.2MHz reference


/*
 * DLPF_options:A value b/w 0 to 6;
 */


/*
 * GYRO_FS
 */
#define GYRO_250		0 	//± 250 °/s
#define GYRO_500		1 	//± 500 °/s
#define GYRO_1000		2 	//± 1000 °/s
#define GYRO_2000		3 	//± 2000 °/s

/*
 * ACC_FS
 */
#define ACC_2G			0 	//± 2g
#define ACC_4G			1	//± 4g
#define ACC_8G			2 	//± 8g
#define ACC_16G			3 	//± 16g


/*
 * enableFIFO
 */
#define FIFO_ALL		0	//enable fifo for all measurements
#define	FIFO_TEMP		1	//Fifo enable for temperature sensor
#define	FIFO_ACC		2	//FIFO enable for Accelerometer
#define	FIFO_GYRO		3	//Enable enable for Gyro


/*
 *************Supported API***************************
 */
uint8_t mpu6050Init(mpu6050_t	*mpu6050);
void mpu6050Deinit(void);
void gatherRawData(void);
float getXaccelerationRaw(void);
float getYaccelerationRaw(void);
float getZaccelerationRaw(void);
float getXgyroRaw(void);
float getYgyroRaw(void);
float getZgyroRaw(void);
float getTemperatureRaw(void);

float getXaccelerationFIFO(mpu6050_t	*mpu6050);
float getYaccelerationFIFO(mpu6050_t	*mpu6050);
float getZaccelerationFIFO(mpu6050_t	*mpu6050);
float getXgyroFIFO(mpu6050_t	*mpu6050);
float getYgyroFIFO(mpu6050_t	*mpu6050);
float getZgyroFIFO(mpu6050_t	*mpu6050);
float getTemperatureFIFO(mpu6050_t	*mpu6050);


#endif /* INC_MPU6050_H_ */
