//
// Created by fabie on 06/12/2025.
//

#ifndef STM32IRLOADER_IRPAYLOAD_H
#define STM32IRLOADER_IRPAYLOAD_H

#include "main.h"

uint8_t readIRPayload(I2C_HandleTypeDef * i2c, uint8_t deviceAddress, uint8_t page, uint8_t * payload);

#endif //STM32IRLOADER_IRPAYLOAD_H