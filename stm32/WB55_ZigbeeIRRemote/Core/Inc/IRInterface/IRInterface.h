//
// Created by Fabien AUBRET on 07/12/2025.
//

#ifndef STM32AT24C08_IRLOADER_H
#define STM32AT24C08_IRLOADER_H

#include <IRInterface/M24M01.h>
#include <stdlib.h>

enum IRInterface_Commands {
    CMD_OFF = 0,
    CMD_ON_HEAT_20DEG = 116
};

void IRInterface_Init(I2C_HandleTypeDef * i2c);
void IRInterface_Load(enum IRInterface_Commands command);
void IRInterface_Send(TIM_HandleTypeDef *tim, enum IRInterface_Commands command);

#endif //STM32AT24C08_IRLOADER_H
