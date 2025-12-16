//
// Created by Fabien AUBRET on 07/12/2025.
//

#include <IRInterface/M24M01.h>
#include "stm32wbxx_hal_i2c.h"

const uint8_t addressBase   = 0xA0;
const uint8_t e1Flag        = 0x08;
const uint8_t e2Flag        = 0x04;

uint8_t deviceAddress = addressBase | e1Flag | e2Flag;

I2C_HandleTypeDef * M24M01_i2c;

/**
 * Initialize AT24C08 connection
 * @param i2c The I2C handler to be used
 */
void M24M01_Init(I2C_HandleTypeDef * i2c) {
	M24M01_i2c = i2c;
}

/**
 * Read data from M24M01 EEPROM
 * @param address The address in the page to read the data from
 * @param buffer_out The buffer to output the data
 * @return The length of the payload
 */
HAL_StatusTypeDef M24M01_Read(uint32_t address, uint16_t length, uint8_t * buffer_out) {
	uint8_t byteAddrMSB = (address >> 9 & 0x80) >> 6;
	uint8_t deviceReadAddress = deviceAddress | byteAddrMSB;

	uint8_t readAddressUp = address >> 8 & 0xFF;
	uint8_t readAddressDown = address & 0xFF;
	uint8_t addressArray[2] = {readAddressUp, readAddressDown};

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(M24M01_i2c, deviceReadAddress, (uint8_t *)addressArray, 2, HAL_MAX_DELAY);
    status = HAL_I2C_Master_Receive(M24M01_i2c, deviceReadAddress, buffer_out, 1, HAL_MAX_DELAY);

    return status;
}

/**
 * Write data to M24M01 EEPROM
 * @param page The page to write data to
 * @param address The address in the page to write the data to
 * @param value The value to be written
 * @return
 */
HAL_StatusTypeDef M24M01_Write(uint32_t address, uint8_t value) {
	uint8_t byteAddrMSB = (address >> 9 & 0x80) >> 6;
	uint8_t deviceWriteAddress = deviceAddress | byteAddrMSB | 0x01;

	uint8_t writeAddressUp = address >> 8 & 0xFF;
	uint8_t writeAddressDown = address & 0xFF;

    const uint8_t data[3] = {writeAddressUp, writeAddressDown, value};

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(M24M01_i2c, deviceWriteAddress, (uint8_t *)data, 3, HAL_MAX_DELAY);

    return status;
}
