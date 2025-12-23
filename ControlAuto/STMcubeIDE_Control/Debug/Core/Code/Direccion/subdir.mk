################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Code/Direccion/Direccion.cpp 

OBJS += \
./Core/Code/Direccion/Direccion.o 

CPP_DEPS += \
./Core/Code/Direccion/Direccion.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Code/Direccion/%.o Core/Code/Direccion/%.su Core/Code/Direccion/%.cyclo: ../Core/Code/Direccion/%.cpp Core/Code/Direccion/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Comunicacion" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/UserCode" -I"C:/Users/Nahuel/Desktop/GNA/Tejada/Tejada STMCUBEIDE/Core/Code/Direccion" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Code-2f-Direccion

clean-Core-2f-Code-2f-Direccion:
	-$(RM) ./Core/Code/Direccion/Direccion.cyclo ./Core/Code/Direccion/Direccion.d ./Core/Code/Direccion/Direccion.o ./Core/Code/Direccion/Direccion.su

.PHONY: clean-Core-2f-Code-2f-Direccion

