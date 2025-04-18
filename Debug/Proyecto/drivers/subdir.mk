################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Proyecto/drivers/button.c \
../Proyecto/drivers/debug_uart.c \
../Proyecto/drivers/port_button.c \
../Proyecto/drivers/port_usb_cdc.c \
../Proyecto/drivers/usb_cdc.c 

OBJS += \
./Proyecto/drivers/button.o \
./Proyecto/drivers/debug_uart.o \
./Proyecto/drivers/port_button.o \
./Proyecto/drivers/port_usb_cdc.o \
./Proyecto/drivers/usb_cdc.o 

C_DEPS += \
./Proyecto/drivers/button.d \
./Proyecto/drivers/debug_uart.d \
./Proyecto/drivers/port_button.d \
./Proyecto/drivers/port_usb_cdc.d \
./Proyecto/drivers/usb_cdc.d 


# Each subdirectory must supply rules for building sources it contributes
Proyecto/drivers/%.o Proyecto/drivers/%.su Proyecto/drivers/%.cyclo: ../Proyecto/drivers/%.c Proyecto/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/app" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/drivers" -I"C:/Users/Ignacio/Desktop/Postgrado/Programacion de MCUs/PdM_Proyecto_Sound_LVL/Proyecto/API/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Proyecto-2f-drivers

clean-Proyecto-2f-drivers:
	-$(RM) ./Proyecto/drivers/button.cyclo ./Proyecto/drivers/button.d ./Proyecto/drivers/button.o ./Proyecto/drivers/button.su ./Proyecto/drivers/debug_uart.cyclo ./Proyecto/drivers/debug_uart.d ./Proyecto/drivers/debug_uart.o ./Proyecto/drivers/debug_uart.su ./Proyecto/drivers/port_button.cyclo ./Proyecto/drivers/port_button.d ./Proyecto/drivers/port_button.o ./Proyecto/drivers/port_button.su ./Proyecto/drivers/port_usb_cdc.cyclo ./Proyecto/drivers/port_usb_cdc.d ./Proyecto/drivers/port_usb_cdc.o ./Proyecto/drivers/port_usb_cdc.su ./Proyecto/drivers/usb_cdc.cyclo ./Proyecto/drivers/usb_cdc.d ./Proyecto/drivers/usb_cdc.o ./Proyecto/drivers/usb_cdc.su

.PHONY: clean-Proyecto-2f-drivers

