#pragma once

#include "stm32f4xx_hal.h"

void UART_Send(uint8_t u8_data);

void UART_Send_SetMotorPosition(uint16_t motorId, uint16_t Position,
		uint16_t Time);

