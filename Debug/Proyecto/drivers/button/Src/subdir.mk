################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Proyecto/drivers/button/Src/button.c \
../Proyecto/drivers/button/Src/port_button.c 

OBJS += \
./Proyecto/drivers/button/Src/button.o \
./Proyecto/drivers/button/Src/port_button.o 

C_DEPS += \
./Proyecto/drivers/button/Src/button.d \
./Proyecto/drivers/button/Src/port_button.d 


# Each subdirectory must supply rules for building sources it contributes
Proyecto/drivers/button/Src/%.o Proyecto/drivers/button/Src/%.su Proyecto/drivers/button/Src/%.cyclo: ../Proyecto/drivers/button/Src/%.c Proyecto/drivers/button/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/app" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/API/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/button/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/uart/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/usb_cdc/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/rtc/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/eeprom/Inc" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers/bt/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Proyecto-2f-drivers-2f-button-2f-Src

clean-Proyecto-2f-drivers-2f-button-2f-Src:
	-$(RM) ./Proyecto/drivers/button/Src/button.cyclo ./Proyecto/drivers/button/Src/button.d ./Proyecto/drivers/button/Src/button.o ./Proyecto/drivers/button/Src/button.su ./Proyecto/drivers/button/Src/port_button.cyclo ./Proyecto/drivers/button/Src/port_button.d ./Proyecto/drivers/button/Src/port_button.o ./Proyecto/drivers/button/Src/port_button.su

.PHONY: clean-Proyecto-2f-drivers-2f-button-2f-Src

