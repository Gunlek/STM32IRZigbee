//
// Created by Fabien AUBRET on 07/12/2025.
//

#ifndef STM32AT24C08_M24M01_H
#define STM32AT24C08_M24M01_H

#include <stdint.h>
#include "stm32wbxx_hal.h"

void M24M01_Init(I2C_HandleTypeDef * i2c);

HAL_StatusTypeDef M24M01_Read(uint32_t address, uint16_t length, uint8_t *buffer_out);
HAL_StatusTypeDef M24M01_Write(uint32_t address, uint8_t value);

#endif //STM32AT24C08_AT24C08_H
