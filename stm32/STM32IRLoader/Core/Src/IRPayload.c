//
// Created by fabie on 06/12/2025.
//

#include "IRPayload.h"

uint8_t readIRPayload(I2C_HandleTypeDef * i2c, uint8_t deviceAddress, uint8_t page, uint8_t * payload) {
    for (uint8_t k=0; k < 255; k++){payload[k] = 0;}

    const uint8_t address = (deviceAddress & 0xF0) | (page << 1);

    HAL_I2C_Master_Transmit(i2c, address, 0, 1, HAL_MAX_DELAY);

    uint8_t payloadLength;
    HAL_I2C_Master_Receive(i2c, address, &payloadLength, 1, HAL_MAX_DELAY);

    // --- payloadLength now contains the number of bytes to be read
    HAL_I2C_Master_Receive(i2c, address, payload, payloadLength, HAL_MAX_DELAY);

    return payloadLength;
}

