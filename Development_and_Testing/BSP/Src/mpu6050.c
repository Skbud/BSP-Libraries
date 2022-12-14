/*
 * mpu6050.c
 *
 *  Created on: 30-Sep-2022
 *      Author: Sanjay
 */
/* ISSUE:-FIFO Reading are same for all measurement at a given time**/


/*********************************************
 * ****************USAGE**********************
 * *******************************************
 * 1.change i2c handle to one being used
 * 2.For getting raw data
 * 		Call gatherRawData->call desired Raw function
 * 3.For using fifo enable fifo for required sensor->then Call desired FIFO functions.
 *
 *
 */

#include "mpu6050.h"
#include "main.h"



//*********Private variables***********/
uint16_t AccX,AccY,AccZ,GyroX,GyroY,GyroZ,TempSen;
float	DFA,DFG;		//Acc and Gyro divide factor to convert to g and dps scale
float	DFT=340.0;		//temperature division factor
//*********Private Defines************/

#define	TRUE	1
#define	FALSE	0

/*
 * Mpu6050 register Addresses
 */

#define SMPLRT_DIV		0X19
#define	CONFIG			0X1A
#define	GYRO_CONFIG		0X1B
#define	ACCEL_CONFIG	0X1C
#define	FIFO_EN			0X23
#define	INT_ENABLE		0X38
#define	INT_STATUS		0X3A
#define	ACCEL_XOUT_H	0X3B
#define	ACCEL_XOUT_L	0X3C
#define	ACCEL_YOUT_H	0X3D
#define	ACCEL_YOUT_L	0X3E
#define	ACCEL_ZOUT_H	0X3F
#define	ACCEL_ZOUT_L	0X40
#define	TEMP_OUT_H		0X41
#define	TEMP_OUT_L		0X42
#define	GYRO_XOUT_H		0X43
#define	GYRO_XOUT_L		0X44
#define	GYRO_YOUT_H		0X45
#define	GYRO_YOUT_L		0X46
#define	GYRO_ZOUT_H		0X47
#define	GYRO_ZOUT_L		0X48
#define	USER_CTRL		0X6A
#define	PWR_MGMT_1		0X6B
#define	FIFO_COUNTH		0X72
#define	FIFO_COUNTL		0X73
#define	FIFO_R_W		0X74
#define	WHO_AM_I		0X75


//*********Private Functions**********//
void enableTempFIFO(void);
void enableAccFIFO(void);
void enableGyroFIFO(void);
void enableTAGFIFO(void);
void disableFIFO(void);
uint16_t getFIFO_Count(void);
uint8_t readFIFO(void);
void resetFIFO(void);
uint8_t checkdataready(void);
uint8_t	imready(void);
void resetFIFO(void);
void getDivFac(mpu6050_t	*mpu6050);


uint8_t mpu6050Init(mpu6050_t	*mpu6050){
	/*	1.Check if device is available or not
	 * 	2.Configure
	 */
	if(imready()==HAL_OK){
		uint8_t data;
		//sampling rate
		data=mpu6050->SampPreSc;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, SMPLRT_DIV, 1, &data, 1, HAL_MAX_DELAY);

		data=0;
		data=mpu6050->DLPF;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,CONFIG, 1, &data, 1, HAL_MAX_DELAY);

		data=0;
		data=mpu6050->mpu6050_Clk_Src;
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);


		data=0;
		data=(mpu6050->gyroFSR<<3);
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,GYRO_CONFIG, 1, &data, 1, HAL_MAX_DELAY);

		data=0;
		data=(mpu6050->accFSR<<3);
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,ACCEL_CONFIG, 1, &data, 1, HAL_MAX_DELAY);

		/*
		 * enable fifo in USER_CTRL and enable interrupt
		 */
		data=0;
		data=(1<<6);
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, USER_CTRL, 1, &data, 1, HAL_MAX_DELAY);

		data=0;
		data=(1<<0);
		HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,INT_ENABLE,1,&data,1,HAL_MAX_DELAY);



		//set division factors for conversion
		getDivFac(mpu6050);

		return	HAL_OK;
	}
	else{
		return HAL_ERROR;
	}
}


void mpu6050Deinit(void){
	//Reset device
	uint8_t data=0;
	data=(1<<7);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR,PWR_MGMT_1,1,&data,1,HAL_MAX_DELAY);
}







void gatherRawData(void){
	uint8_t data[14];
	HAL_I2C_Mem_Read(&hi2c3, MPU6050ADDR, ACCEL_XOUT_H, 1, data, 14, HAL_MAX_DELAY);
	AccX=(uint16_t)(data[0]<<8|data[1]);
	AccY=(uint16_t)(data[2]<<8|data[3]);
	AccZ=(uint16_t)(data[4]<<8|data[5]);
	//TempSen=(uint16_t)(data[6]<<8|data[7]);
	TempSen=(float)data[6]+(float)(data[7]/10.0);
	GyroX=(uint16_t)(data[8]<<8|data[9]);
	GyroY=(uint16_t)(data[10]<<8|data[11]);
	GyroZ=(uint16_t)(data[12]<<8|data[13]);
}

float getXaccelerationRaw(void){
	return AccX/DFA;
}

float getYaccelerationRaw(void){
	return AccY/DFA;
}

float getZaccelerationRaw(void){
	return AccZ/DFA;
}

float getXgyroRaw(void){
	return GyroX/DFG;
}

float getYgyroRaw(void){
	return GyroY/DFG;
}

float getZgyroRaw(void){
	return GyroZ/DFG;
}

float getTemperatureRaw(void){
	return (TempSen/DFT)+36.53;
}


float getTemperatureFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_TEMP){
		enableTempFIFO();
		uint8_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]); //conversion
		return (rdata/DFT)+36.53;
	}
	else if(mpu6050->enableFIFO==FIFO_ALL){
		enableTAGFIFO();
		uint8_t data[8];
		for(int i=0;i<8;i++){
			data[i]=readFIFO();
		}
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[6]<<8|data[7]);
		return	(rdata/DFT)+36.53;
	}
	else{
		return HAL_ERROR;
	}
}

float getXaccelerationFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_ACC||mpu6050->enableFIFO==FIFO_ALL){
		enableAccFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return  rdata/DFA;		 //conversion from raw to g
	}
	else{
		return HAL_ERROR;
	}
}

float getYaccelerationFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_ACC){
		enableAccFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return  rdata/DFA;
	}
	else if(mpu6050->enableFIFO==FIFO_ALL){
			enableTAGFIFO();
			uint8_t data[4];
			for(int i=0;i<4;i++){
				data[i]=readFIFO();
			}
			disableFIFO();
			uint16_t rdata=(uint16_t)(data[2]<<8|data[3]);
			return	rdata/DFA;
	}
	else{
		return HAL_ERROR;
	}
}


float getZaccelerationFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_ACC){
		enableAccFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return  rdata/DFA;
	}
	else if(mpu6050->enableFIFO==FIFO_ALL){
		enableTAGFIFO();
		uint8_t data[6];
		for(int i=0;i<6;i++){
			data[i]=readFIFO();
		}
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[4]<<8|data[5]);
		return	rdata/DFA;
	}
	else{
		return HAL_ERROR;
	}
}

float getXgyroFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_GYRO){
		enableGyroFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return  rdata/DFG; 		//conversion to DPS
		}
	else if(mpu6050->enableFIFO==FIFO_ALL){
		enableTAGFIFO();
		uint8_t data[10];
		for(int i=0;i<10;i++){
			data[i]=readFIFO();
		}
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[8]<<8|data[9]);
		return	rdata/DFG;
		}
	else{
		return HAL_ERROR;
		}
}

float getYgyroFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_GYRO){
		enableGyroFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return  rdata/DFG;
		}
	else if(mpu6050->enableFIFO==FIFO_ALL){
		enableTAGFIFO();
		uint8_t data[12];
		for(int i=0;i<12;i++){
			data[i]=readFIFO();
		}
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[10]<<8|data[11]);
		return	rdata/DFG;
		}
	else{
		return HAL_ERROR;
		}
}

float getZgyroFIFO(mpu6050_t	*mpu6050){
	if(mpu6050->enableFIFO==FIFO_GYRO){
		enableGyroFIFO();
		uint16_t data[2];
		data[0]=readFIFO();
		data[1]=readFIFO();
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[0]<<8|data[1]);
		return rdata/DFG;
		}
	else if(mpu6050->enableFIFO==FIFO_ALL){
		enableTAGFIFO();
		uint8_t data[14];
		for(int i=0;i<14;i++){
			data[i]=readFIFO();
		}
		disableFIFO();
		uint16_t rdata=(uint16_t)(data[12]<<8|data[13]);
		return	rdata/DFG;
		}
	else{
		return HAL_ERROR;
		}
}


void enableTempFIFO(void){
	uint8_t data=0;
	data=(1<<7);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, FIFO_EN, 1, &data, 1, HAL_MAX_DELAY);
}

void enableGyroFIFO(void){
	uint8_t data=0;
	data=(7<<4);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, FIFO_EN, 1, &data, 1, HAL_MAX_DELAY);
}

void enableAccFIFO(void){
	uint8_t data=0;
	data=(1<<3);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, FIFO_EN, 1, &data, 1, HAL_MAX_DELAY);
}


//Enable  Fifo for Acc,Grro and Temp
void enableTAGFIFO(void){
	uint8_t data=0;
	data=(31<3);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, FIFO_EN, 1, &data, 1, HAL_MAX_DELAY);
}

void disableFIFO(void){
	uint8_t data=0;
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, FIFO_EN, 1, &data, 1, HAL_MAX_DELAY);
}



uint8_t checkdataready(void){
	uint8_t dataready=0;
	HAL_I2C_Mem_Read(&hi2c3, MPU6050ADDR, INT_STATUS, 1, &dataready, 1, HAL_MAX_DELAY);
	dataready=dataready&0X01;
	if(dataready==1){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

uint8_t readFIFO(void){
	if(checkdataready()){
		uint8_t data=0;
		HAL_I2C_Mem_Read(&hi2c3, MPU6050ADDR, FIFO_R_W, 1, &data, 1, HAL_MAX_DELAY);
		resetFIFO();
		return	data;
	}
	else{
		return HAL_ERROR;
	}
}

uint16_t getFIFO_Count(void){
	uint8_t countdata[2];
	HAL_I2C_Mem_Read(&hi2c3, MPU6050ADDR, FIFO_COUNTH, 1, countdata, 2, HAL_MAX_DELAY);
	return	(((uint16_t)countdata[0])<<8)|countdata[1];
}

uint8_t	imready(void){
	uint8_t	data;
	HAL_I2C_Mem_Read(&hi2c3, MPU6050ADDR, WHO_AM_I, 1, &data, 1, HAL_MAX_DELAY);
	if(data== 0x68){
		return HAL_OK;
	}
	else{
		return HAL_ERROR;
	}
}

void resetFIFO(void){
	uint8_t data;
	/*	1.Disable FIFO
	 * 	2.Reset FIFO
	 * 	3.Enable FIFO
	 */
	data=0;
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, USER_CTRL, 1, &data, 1, HAL_MAX_DELAY);
	data=(1<<2);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, USER_CTRL, 1, &data, 1, HAL_MAX_DELAY);
	data=0;
	data=(1<<6);
	HAL_I2C_Mem_Write(&hi2c3, MPU6050ADDR, USER_CTRL, 1, &data, 1, HAL_MAX_DELAY);
}

void getDivFac(mpu6050_t	*mpu6050){

	switch(mpu6050->accFSR){
		case ACC_2G:
			DFA=16384.0;
			break;
		case ACC_4G:
			DFA=8192.0;
			break;
		case ACC_8G:
			DFA=4096.0;
			break;
		case ACC_16G:
			DFA=2048.0;
			break;
		}
	switch(mpu6050->gyroFSR){
			case GYRO_250:
				DFG=131.0;
				break;
			case GYRO_500:
				DFG=65.5;
				break;
			case GYRO_1000:
				DFG=32.8;
				break;
			case GYRO_2000:
				DFG=16.4;
				break;
			}
}
