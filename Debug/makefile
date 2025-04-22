################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Proyecto/ports/port_usb_cdc/subdir.mk
-include Proyecto/ports/port_uart/subdir.mk
-include Proyecto/ports/port_rtc/subdir.mk
-include Proyecto/ports/port_eeprom/subdir.mk
-include Proyecto/ports/port_button/subdir.mk
-include Proyecto/ports/port_bt/subdir.mk
-include Proyecto/drivers/usb_cdc/subdir.mk
-include Proyecto/drivers/uart/subdir.mk
-include Proyecto/drivers/rtc/subdir.mk
-include Proyecto/drivers/eeprom/subdir.mk
-include Proyecto/drivers/button/subdir.mk
-include Proyecto/drivers/bt/subdir.mk
-include Proyecto/app/subdir.mk
-include Proyecto/API/Src/subdir.mk
-include Proyecto/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := PdM_Proyecto_Sound_LVL
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PdM_Proyecto_Sound_LVL.elf \

MAP_FILES += \
PdM_Proyecto_Sound_LVL.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
PdM_Proyecto_Sound_LVL.list \


# All Target
all: main-build

# Main-build Target
main-build: PdM_Proyecto_Sound_LVL.elf secondary-outputs

# Tool invocations
PdM_Proyecto_Sound_LVL.elf PdM_Proyecto_Sound_LVL.map: $(OBJS) $(USER_OBJS) C:\Users\Ignacio\Desktop\Postgrado\Programacion\ de\ MCUs\PdM_Proyecto_Sound_LVL\STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "PdM_Proyecto_Sound_LVL.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Ignacio\Desktop\Postgrado\Programacion de MCUs\PdM_Proyecto_Sound_LVL\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="PdM_Proyecto_Sound_LVL.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

PdM_Proyecto_Sound_LVL.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "PdM_Proyecto_Sound_LVL.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) PdM_Proyecto_Sound_LVL.elf PdM_Proyecto_Sound_LVL.list PdM_Proyecto_Sound_LVL.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
