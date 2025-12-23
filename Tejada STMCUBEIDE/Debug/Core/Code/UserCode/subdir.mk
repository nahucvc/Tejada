################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Code/UserCode/UserCode.cpp 

OBJS += \
./Core/Code/UserCode/UserCode.o 

CPP_DEPS += \
./Core/Code/UserCode/UserCode.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Code/UserCode/%.o Core/Code/UserCode/%.su Core/Code/UserCode/%.cyclo: ../Core/Code/UserCode/%.cpp Core/Code/UserCode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Comunicacion" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/UserCode" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Direccion" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Code-2f-UserCode

clean-Core-2f-Code-2f-UserCode:
	-$(RM) ./Core/Code/UserCode/UserCode.cyclo ./Core/Code/UserCode/UserCode.d ./Core/Code/UserCode/UserCode.o ./Core/Code/UserCode/UserCode.su

.PHONY: clean-Core-2f-Code-2f-UserCode

