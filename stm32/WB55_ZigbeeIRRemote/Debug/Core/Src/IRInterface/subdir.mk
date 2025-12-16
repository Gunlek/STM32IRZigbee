################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/IRInterface/IRInterface.c \
../Core/Src/IRInterface/M24M01.c 

OBJS += \
./Core/Src/IRInterface/IRInterface.o \
./Core/Src/IRInterface/M24M01.o 

C_DEPS += \
./Core/Src/IRInterface/IRInterface.d \
./Core/Src/IRInterface/M24M01.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/IRInterface/%.o Core/Src/IRInterface/%.su Core/Src/IRInterface/%.cyclo: ../Core/Src/IRInterface/%.c Core/Src/IRInterface/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DZIGBEE_WB -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/extras -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zgp -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/mac -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/key -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/se -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/security -I../Middlewares/ST/STM32_WPAN/zigbee/stack/include/zcl/general -I../Middlewares/ST/STM32_WPAN/zigbee/platform -I../Middlewares/ST/STM32_WPAN/zigbee/core/inc -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-IRInterface

clean-Core-2f-Src-2f-IRInterface:
	-$(RM) ./Core/Src/IRInterface/IRInterface.cyclo ./Core/Src/IRInterface/IRInterface.d ./Core/Src/IRInterface/IRInterface.o ./Core/Src/IRInterface/IRInterface.su ./Core/Src/IRInterface/M24M01.cyclo ./Core/Src/IRInterface/M24M01.d ./Core/Src/IRInterface/M24M01.o ./Core/Src/IRInterface/M24M01.su

.PHONY: clean-Core-2f-Src-2f-IRInterface

