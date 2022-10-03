################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Src/DHT11.c \
../BSP/Src/Servo_Motor.c \
../BSP/Src/mpu6050.c 

OBJS += \
./BSP/Src/DHT11.o \
./BSP/Src/Servo_Motor.o \
./BSP/Src/mpu6050.o 

C_DEPS += \
./BSP/Src/DHT11.d \
./BSP/Src/Servo_Motor.d \
./BSP/Src/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/Src/%.o BSP/Src/%.su: ../BSP/Src/%.c BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I"C:/Users/pc/Documents/GitHub/BSP-Libraries/Development_and_Testing/BSP/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-Src

clean-BSP-2f-Src:
	-$(RM) ./BSP/Src/DHT11.d ./BSP/Src/DHT11.o ./BSP/Src/DHT11.su ./BSP/Src/Servo_Motor.d ./BSP/Src/Servo_Motor.o ./BSP/Src/Servo_Motor.su ./BSP/Src/mpu6050.d ./BSP/Src/mpu6050.o ./BSP/Src/mpu6050.su

.PHONY: clean-BSP-2f-Src

