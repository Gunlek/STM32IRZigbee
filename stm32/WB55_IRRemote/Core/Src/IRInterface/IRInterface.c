//
// Created by Fabien AUBRET on 07/12/2025.
//

#include "IRInterface/IRInterface.h"

#include "main.h"

uint8_t payloadLength;
uint8_t * IRInterface_commandBuffer;

uint16_t bufferIndex = 0;

void IRInterface_Init(I2C_HandleTypeDef * i2c) {
    AT24C08_Init(i2c);
}

void IRInterface_Load(enum IRInterface_Commands commandOffset) {
    payloadLength = 0;
    AT24C08_ReadAddress(commandOffset, 1, &payloadLength);

    IRInterface_commandBuffer = malloc(payloadLength);
    for(int k=0; k < payloadLength; k++) {
    	AT24C08_ReadAddress(commandOffset + 1 + k, 1, &IRInterface_commandBuffer[k]);
    }
}

void IRInterface_Send(TIM_HandleTypeDef * htim, enum IRInterface_Commands commandOffset) {
	IRInterface_Load(commandOffset);

    HAL_TIM_Base_Start_IT(htim);
    HAL_TIM_PWM_Start_IT(htim, TIM_CHANNEL_1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM1) {
		if(bufferIndex < payloadLength) {
			uint8_t repeat = IRInterface_commandBuffer[bufferIndex];
			if(bufferIndex % 2 == 0) {
				htim->Instance->CCR1 = 267;
			}
			else {
				htim->Instance->CCR1 = 0;
			}
			htim->Instance->RCR = repeat;
			bufferIndex++;
		}
		else {
			bufferIndex = 0;
			free(IRInterface_commandBuffer);
			HAL_TIM_Base_Stop_IT(htim);
		    HAL_TIM_PWM_Start_IT(htim, TIM_CHANNEL_1);
		}
	}
}
