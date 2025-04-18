################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Proyecto/proyecto.c 

OBJS += \
./Proyecto/proyecto.o 

C_DEPS += \
./Proyecto/proyecto.d 


# Each subdirectory must supply rules for building sources it contributes
Proyecto/%.o Proyecto/%.su Proyecto/%.cyclo: ../Proyecto/%.c Proyecto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/Proyecto/Proyecto" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/Proyecto/Proyecto/API" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/Proyecto/Proyecto/app" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/Proyecto/Proyecto/drivers" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Proyecto

clean-Proyecto:
	-$(RM) ./Proyecto/proyecto.cyclo ./Proyecto/proyecto.d ./Proyecto/proyecto.o ./Proyecto/proyecto.su

.PHONY: clean-Proyecto

