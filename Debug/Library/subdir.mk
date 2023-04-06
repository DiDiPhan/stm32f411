################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Library/Button.c \
../Library/GPIO_Led.c \
../Library/I2C.c \
../Library/SPI1.c \
../Library/UART2_CP2102.c 

OBJS += \
./Library/Button.o \
./Library/GPIO_Led.o \
./Library/I2C.o \
./Library/SPI1.o \
./Library/UART2_CP2102.o 

C_DEPS += \
./Library/Button.d \
./Library/GPIO_Led.d \
./Library/I2C.d \
./Library/SPI1.d \
./Library/UART2_CP2102.d 


# Each subdirectory must supply rules for building sources it contributes
Library/%.o Library/%.su: ../Library/%.c Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"D:/EMBEDED/CODE/projectSTM32f411/Library" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Library

clean-Library:
	-$(RM) ./Library/Button.d ./Library/Button.o ./Library/Button.su ./Library/GPIO_Led.d ./Library/GPIO_Led.o ./Library/GPIO_Led.su ./Library/I2C.d ./Library/I2C.o ./Library/I2C.su ./Library/SPI1.d ./Library/SPI1.o ./Library/SPI1.su ./Library/UART2_CP2102.d ./Library/UART2_CP2102.o ./Library/UART2_CP2102.su

.PHONY: clean-Library

