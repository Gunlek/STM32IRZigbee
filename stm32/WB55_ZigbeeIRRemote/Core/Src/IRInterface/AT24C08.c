//
// Created by Fabien AUBRET on 07/12/2025.
//

#include "IRInterface/AT24C08.h"

#include "stm32wbxx_hal_i2c.h"

uint8_t AT24C08_deviceAddress = 0xA0;
I2C_HandleTypeDef * AT24C08_i2c;

uint8_t AT24C08_pages = 4;
uint16_t AT24C08_bytesPerPage = 256;

/**
 * Initialize AT24C08 connection
 * @param i2c The I2C handler to be used
 */
void AT24C08_Init(I2C_HandleTypeDef * i2c) {
    AT24C08_i2c = i2c;
}

/**
 * Read data from AT24C08 EEPROM
 * @param page The page number to read the data from
 * @param address The address in the page to read the data from
 * @param buffer_out The buffer to output the data
 * @return The length of the payload
 */
uint8_t AT24C08_Read(uint8_t page, uint8_t address, uint16_t length, uint8_t * buffer_out) {
    const uint8_t readAddress = (AT24C08_deviceAddress & 0xF0) | (page << 1);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(AT24C08_i2c, readAddress, &address, 1, HAL_MAX_DELAY);
    status = HAL_I2C_Master_Receive(AT24C08_i2c, readAddress, buffer_out, 1, HAL_MAX_DELAY);

    return status;
}

uint8_t AT24C08_ReadAddress(uint16_t address, uint16_t length, uint8_t * buffer_out) {
    const uint8_t page = address / AT24C08_bytesPerPage;
    const uint8_t pageAddress = address % AT24C08_bytesPerPage;

    return AT24C08_Read(page, pageAddress, length, buffer_out);
}

/**
 * Write data to AT24C08 EEPROM
 * @param page The page to write data to
 * @param address The address in the page to write the data to
 * @param value The value to be written
 * @return
 */
HAL_StatusTypeDef AT24C08_Write(uint8_t page, uint8_t address, uint8_t value) {
    const uint8_t writeAddress = (AT24C08_deviceAddress & 0xF0) | (page << 1) | 0x01;

    const uint8_t data[2] = {address, value};

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(AT24C08_i2c, writeAddress, (uint8_t *)data, 2, HAL_MAX_DELAY);

    return status;
}
