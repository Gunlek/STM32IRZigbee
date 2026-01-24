//
// Created by Fabien AUBRET on 07/12/2025.
//

#include "IRInterface/IRInterface.h"

#include "main.h"

#include "stm32wbxx_hal.h"

uint8_t payloadLength[2];
uint16_t fullPayloadLength;

uint16_t bufferIndex = 0;
volatile uint8_t freeMemoryFlag = 0;

uint8_t * IRInterface_commandBuffer;

void IRInterface_Init(I2C_HandleTypeDef * i2c) {
	M24M01_Init(i2c);
}

void IRInterface_Load(enum IRInterface_Commands commandOffset) {
    payloadLength[0] = 0;
    payloadLength[1] = 0;
    M24M01_Read(commandOffset, 1, &payloadLength[0]);
    M24M01_Read(commandOffset+1, 1, &payloadLength[1]);

    fullPayloadLength = payloadLength[0] << 8 | payloadLength[1];

    IRInterface_commandBuffer = malloc(fullPayloadLength);
    for(int k=0; k < fullPayloadLength; k++) {
    	// --- Offset by 2 as the command length is stored in two bytes
    	M24M01_Read((commandOffset + 2) + k, 1, &IRInterface_commandBuffer[k]);
    }
}

void IRInterface_Send(TIM_HandleTypeDef * htim, enum IRInterface_Commands commandOffset) {
	IRInterface_Load(commandOffset);

    HAL_TIM_Base_Start_IT(htim);
    HAL_TIM_PWM_Start_IT(htim, TIM_CHANNEL_1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	TIM_TypeDef *tim = htim->Instance;

	if(tim == TIM17) {
		if(bufferIndex < fullPayloadLength) {
			tim->CCR1 = bufferIndex & 1 ? 0 : 280;
			tim->RCR = IRInterface_commandBuffer[bufferIndex];
			bufferIndex++;
		}
		else {
			if(bufferIndex < fullPayloadLength+1){
				// --- Ajout d'une condition de stop
				// --- Un 0 final
				htim->Instance->CCR1 = 0;
				htim->Instance->RCR = 45;
				bufferIndex++;
			}
			else {
				HAL_TIM_Base_Stop_IT(htim);
				HAL_TIM_PWM_Stop_IT(htim, TIM_CHANNEL_1);
				bufferIndex = 0;
				freeMemoryFlag = 1;
			}
		}
	}
}
