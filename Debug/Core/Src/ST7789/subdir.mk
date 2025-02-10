################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ST7789/fonts.c \
../Core/Src/ST7789/st7789.c 

OBJS += \
./Core/Src/ST7789/fonts.o \
./Core/Src/ST7789/st7789.o 

C_DEPS += \
./Core/Src/ST7789/fonts.d \
./Core/Src/ST7789/st7789.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ST7789/%.o Core/Src/ST7789/%.su Core/Src/ST7789/%.cyclo: ../Core/Src/ST7789/%.c Core/Src/ST7789/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Michal/Desktop/bpElectronicTarget/target_unit_piezo/Core/Src/codegen" -I"C:/Users/Michal/Desktop/bpElectronicTarget/target_unit_piezo/Core/Src/ST7789" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-ST7789

clean-Core-2f-Src-2f-ST7789:
	-$(RM) ./Core/Src/ST7789/fonts.cyclo ./Core/Src/ST7789/fonts.d ./Core/Src/ST7789/fonts.o ./Core/Src/ST7789/fonts.su ./Core/Src/ST7789/st7789.cyclo ./Core/Src/ST7789/st7789.d ./Core/Src/ST7789/st7789.o ./Core/Src/ST7789/st7789.su

.PHONY: clean-Core-2f-Src-2f-ST7789

