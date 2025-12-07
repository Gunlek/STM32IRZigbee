//
// Created by Fabien AUBRET on 07/12/2025.
//

#include "IRInterface.h"

#include "main.h"

uint8_t payloadLength;
uint8_t * IRInterface_commandBuffer;

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

//    free(IRInterface_commandBuffer);
}

void IRInterface_Send(TIM_HandleTypeDef * tim, enum IRInterface_Commands commandOffset) {
	IRInterface_Load(commandOffset);

    uint8_t payloadDMALength = payloadLength * 2;
    uint8_t payloadDMA[payloadDMALength];
    uint8_t alternate = 1; // 0 → 0 , 1 → 250

    for (uint8_t i = 0; i < payloadLength-1; i++)
    {
        payloadDMA[2 * i]     = IRInterface_commandBuffer[i];
        payloadDMA[2 * i + 1] = alternate ? (uint8_t)69 : (uint8_t)0;

        alternate ^= 1; // bascule 0 ↔ 1
    }

    HAL_TIM_DMABurst_MultiWriteStart(tim, TIM_DMABASE_RCR, TIM_DMA_UPDATE, payloadDMA, TIM_DMABURSTLENGTH_2TRANSFERS, payloadDMALength);
    HAL_TIM_PWM_Start(tim, TIM_CHANNEL_1);

    free(IRInterface_commandBuffer);
}
