################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/codegen/FFTImplementationCallback.c \
../Core/Src/codegen/colon.c \
../Core/Src/codegen/envelope.c \
../Core/Src/codegen/envelope_xcorr_gen.c \
../Core/Src/codegen/envelope_xcorr_gen_data.c \
../Core/Src/codegen/envelope_xcorr_gen_emxutil.c \
../Core/Src/codegen/envelope_xcorr_gen_initialize.c \
../Core/Src/codegen/envelope_xcorr_gen_terminate.c \
../Core/Src/codegen/fft.c \
../Core/Src/codegen/find.c \
../Core/Src/codegen/ifft.c \
../Core/Src/codegen/interp1.c \
../Core/Src/codegen/mean.c \
../Core/Src/codegen/rtGetInf.c \
../Core/Src/codegen/rtGetNaN.c \
../Core/Src/codegen/rt_nonfinite.c 

OBJS += \
./Core/Src/codegen/FFTImplementationCallback.o \
./Core/Src/codegen/colon.o \
./Core/Src/codegen/envelope.o \
./Core/Src/codegen/envelope_xcorr_gen.o \
./Core/Src/codegen/envelope_xcorr_gen_data.o \
./Core/Src/codegen/envelope_xcorr_gen_emxutil.o \
./Core/Src/codegen/envelope_xcorr_gen_initialize.o \
./Core/Src/codegen/envelope_xcorr_gen_terminate.o \
./Core/Src/codegen/fft.o \
./Core/Src/codegen/find.o \
./Core/Src/codegen/ifft.o \
./Core/Src/codegen/interp1.o \
./Core/Src/codegen/mean.o \
./Core/Src/codegen/rtGetInf.o \
./Core/Src/codegen/rtGetNaN.o \
./Core/Src/codegen/rt_nonfinite.o 

C_DEPS += \
./Core/Src/codegen/FFTImplementationCallback.d \
./Core/Src/codegen/colon.d \
./Core/Src/codegen/envelope.d \
./Core/Src/codegen/envelope_xcorr_gen.d \
./Core/Src/codegen/envelope_xcorr_gen_data.d \
./Core/Src/codegen/envelope_xcorr_gen_emxutil.d \
./Core/Src/codegen/envelope_xcorr_gen_initialize.d \
./Core/Src/codegen/envelope_xcorr_gen_terminate.d \
./Core/Src/codegen/fft.d \
./Core/Src/codegen/find.d \
./Core/Src/codegen/ifft.d \
./Core/Src/codegen/interp1.d \
./Core/Src/codegen/mean.d \
./Core/Src/codegen/rtGetInf.d \
./Core/Src/codegen/rtGetNaN.d \
./Core/Src/codegen/rt_nonfinite.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/codegen/%.o Core/Src/codegen/%.su Core/Src/codegen/%.cyclo: ../Core/Src/codegen/%.c Core/Src/codegen/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Michal/Desktop/bpElectronicTarget/target_unit_piezo/Core/Src/codegen" -I"C:/Users/Michal/Desktop/bpElectronicTarget/target_unit_piezo/Core/Src/ST7789" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-codegen

clean-Core-2f-Src-2f-codegen:
	-$(RM) ./Core/Src/codegen/FFTImplementationCallback.cyclo ./Core/Src/codegen/FFTImplementationCallback.d ./Core/Src/codegen/FFTImplementationCallback.o ./Core/Src/codegen/FFTImplementationCallback.su ./Core/Src/codegen/colon.cyclo ./Core/Src/codegen/colon.d ./Core/Src/codegen/colon.o ./Core/Src/codegen/colon.su ./Core/Src/codegen/envelope.cyclo ./Core/Src/codegen/envelope.d ./Core/Src/codegen/envelope.o ./Core/Src/codegen/envelope.su ./Core/Src/codegen/envelope_xcorr_gen.cyclo ./Core/Src/codegen/envelope_xcorr_gen.d ./Core/Src/codegen/envelope_xcorr_gen.o ./Core/Src/codegen/envelope_xcorr_gen.su ./Core/Src/codegen/envelope_xcorr_gen_data.cyclo ./Core/Src/codegen/envelope_xcorr_gen_data.d ./Core/Src/codegen/envelope_xcorr_gen_data.o ./Core/Src/codegen/envelope_xcorr_gen_data.su ./Core/Src/codegen/envelope_xcorr_gen_emxutil.cyclo ./Core/Src/codegen/envelope_xcorr_gen_emxutil.d ./Core/Src/codegen/envelope_xcorr_gen_emxutil.o ./Core/Src/codegen/envelope_xcorr_gen_emxutil.su ./Core/Src/codegen/envelope_xcorr_gen_initialize.cyclo ./Core/Src/codegen/envelope_xcorr_gen_initialize.d ./Core/Src/codegen/envelope_xcorr_gen_initialize.o ./Core/Src/codegen/envelope_xcorr_gen_initialize.su ./Core/Src/codegen/envelope_xcorr_gen_terminate.cyclo ./Core/Src/codegen/envelope_xcorr_gen_terminate.d ./Core/Src/codegen/envelope_xcorr_gen_terminate.o ./Core/Src/codegen/envelope_xcorr_gen_terminate.su ./Core/Src/codegen/fft.cyclo ./Core/Src/codegen/fft.d ./Core/Src/codegen/fft.o ./Core/Src/codegen/fft.su ./Core/Src/codegen/find.cyclo ./Core/Src/codegen/find.d ./Core/Src/codegen/find.o ./Core/Src/codegen/find.su ./Core/Src/codegen/ifft.cyclo ./Core/Src/codegen/ifft.d ./Core/Src/codegen/ifft.o ./Core/Src/codegen/ifft.su ./Core/Src/codegen/interp1.cyclo ./Core/Src/codegen/interp1.d ./Core/Src/codegen/interp1.o ./Core/Src/codegen/interp1.su ./Core/Src/codegen/mean.cyclo ./Core/Src/codegen/mean.d ./Core/Src/codegen/mean.o ./Core/Src/codegen/mean.su ./Core/Src/codegen/rtGetInf.cyclo ./Core/Src/codegen/rtGetInf.d ./Core/Src/codegen/rtGetInf.o ./Core/Src/codegen/rtGetInf.su ./Core/Src/codegen/rtGetNaN.cyclo ./Core/Src/codegen/rtGetNaN.d ./Core/Src/codegen/rtGetNaN.o ./Core/Src/codegen/rtGetNaN.su ./Core/Src/codegen/rt_nonfinite.cyclo ./Core/Src/codegen/rt_nonfinite.d ./Core/Src/codegen/rt_nonfinite.o ./Core/Src/codegen/rt_nonfinite.su

.PHONY: clean-Core-2f-Src-2f-codegen

