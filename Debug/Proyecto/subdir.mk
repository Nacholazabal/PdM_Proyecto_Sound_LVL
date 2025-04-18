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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/app" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/API/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/ports/port_button" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/ports/port_usb_cdc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/button" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/uart" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/usb_cdc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/ports/port_rfid" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/rfid" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Proyecto

clean-Proyecto:
	-$(RM) ./Proyecto/proyecto.cyclo ./Proyecto/proyecto.d ./Proyecto/proyecto.o ./Proyecto/proyecto.su

.PHONY: clean-Proyecto

