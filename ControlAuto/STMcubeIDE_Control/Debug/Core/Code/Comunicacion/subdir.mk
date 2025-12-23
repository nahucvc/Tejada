################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Code/Comunicacion/Serial.cpp \
../Core/Code/Comunicacion/SerialUSB.cpp 

OBJS += \
./Core/Code/Comunicacion/Serial.o \
./Core/Code/Comunicacion/SerialUSB.o 

CPP_DEPS += \
./Core/Code/Comunicacion/Serial.d \
./Core/Code/Comunicacion/SerialUSB.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Code/Comunicacion/%.o Core/Code/Comunicacion/%.su Core/Code/Comunicacion/%.cyclo: ../Core/Code/Comunicacion/%.cpp Core/Code/Comunicacion/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Comunicacion" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/UserCode" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Direccion" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Code-2f-Comunicacion

clean-Core-2f-Code-2f-Comunicacion:
	-$(RM) ./Core/Code/Comunicacion/Serial.cyclo ./Core/Code/Comunicacion/Serial.d ./Core/Code/Comunicacion/Serial.o ./Core/Code/Comunicacion/Serial.su ./Core/Code/Comunicacion/SerialUSB.cyclo ./Core/Code/Comunicacion/SerialUSB.d ./Core/Code/Comunicacion/SerialUSB.o ./Core/Code/Comunicacion/SerialUSB.su

.PHONY: clean-Core-2f-Code-2f-Comunicacion

