//
// Created by Fabien AUBRET on 07/12/2025.
//

#ifndef STM32AT24C08_IRLOADER_H
#define STM32AT24C08_IRLOADER_H

#include <stdlib.h>
#include "AT24C08.h"

enum IRInterface_Commands {
    CMD_POWER = 0,
    CMD_SWING = 67,
    CMD_SPEED_P = 134,
    CMD_SPEED_M = 201
};

void IRInterface_Init(I2C_HandleTypeDef * i2c);
void IRInterface_Load(enum IRInterface_Commands command);
void IRInterface_Send(TIM_HandleTypeDef *tim, enum IRInterface_Commands command);

#endif //STM32AT24C08_IRLOADER_H
