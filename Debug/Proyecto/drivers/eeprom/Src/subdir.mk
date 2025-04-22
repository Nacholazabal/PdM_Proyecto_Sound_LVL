################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Proyecto/drivers/eeprom/Src/eeprom.c \
../Proyecto/drivers/eeprom/Src/port_eeprom.c 

OBJS += \
./Proyecto/drivers/eeprom/Src/eeprom.o \
./Proyecto/drivers/eeprom/Src/port_eeprom.o 

C_DEPS += \
./Proyecto/drivers/eeprom/Src/eeprom.d \
./Proyecto/drivers/eeprom/Src/port_eeprom.d 


# Each subdirectory must supply rules for building sources it contributes
Proyecto/drivers/eeprom/Src/%.o Proyecto/drivers/eeprom/Src/%.su Proyecto/drivers/eeprom/Src/%.cyclo: ../Proyecto/drivers/eeprom/Src/%.c Proyecto/drivers/eeprom/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/app" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/API/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/button/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/uart/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/usb_cdc/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/rtc/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/eeprom/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/bt/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Proyecto-2f-drivers-2f-eeprom-2f-Src

clean-Proyecto-2f-drivers-2f-eeprom-2f-Src:
	-$(RM) ./Proyecto/drivers/eeprom/Src/eeprom.cyclo ./Proyecto/drivers/eeprom/Src/eeprom.d ./Proyecto/drivers/eeprom/Src/eeprom.o ./Proyecto/drivers/eeprom/Src/eeprom.su ./Proyecto/drivers/eeprom/Src/port_eeprom.cyclo ./Proyecto/drivers/eeprom/Src/port_eeprom.d ./Proyecto/drivers/eeprom/Src/port_eeprom.o ./Proyecto/drivers/eeprom/Src/port_eeprom.su

.PHONY: clean-Proyecto-2f-drivers-2f-eeprom-2f-Src

