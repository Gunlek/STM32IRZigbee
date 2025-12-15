################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/IRInterface/AT24C08.c \
../Core/Src/IRInterface/IRInterface.c 

OBJS += \
./Core/Src/IRInterface/AT24C08.o \
./Core/Src/IRInterface/IRInterface.o 

C_DEPS += \
./Core/Src/IRInterface/AT24C08.d \
./Core/Src/IRInterface/IRInterface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/IRInterface/%.o Core/Src/IRInterface/%.su Core/Src/IRInterface/%.cyclo: ../Core/Src/IRInterface/%.c Core/Src/IRInterface/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-IRInterface

clean-Core-2f-Src-2f-IRInterface:
	-$(RM) ./Core/Src/IRInterface/AT24C08.cyclo ./Core/Src/IRInterface/AT24C08.d ./Core/Src/IRInterface/AT24C08.o ./Core/Src/IRInterface/AT24C08.su ./Core/Src/IRInterface/IRInterface.cyclo ./Core/Src/IRInterface/IRInterface.d ./Core/Src/IRInterface/IRInterface.o ./Core/Src/IRInterface/IRInterface.su

.PHONY: clean-Core-2f-Src-2f-IRInterface

