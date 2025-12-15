//
// Created by Fabien AUBRET on 07/12/2025.
//

#ifndef STM32AT24C08_AT24C08_H
#define STM32AT24C08_AT24C08_H

#include <stdint.h>
#include "stm32wbxx_hal.h"

void AT24C08_Init(I2C_HandleTypeDef * i2c);
HAL_StatusTypeDef AT24C08_Read(uint8_t page, uint8_t address, uint16_t length, uint8_t *buffer_out);
uint8_t AT24C08_ReadAddress(uint16_t address, uint16_t length, uint8_t *buffer);
HAL_StatusTypeDef ATC24C08_Write(uint8_t page, uint8_t address, uint8_t value);

#endif //STM32AT24C08_AT24C08_H
