#include "Servo.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <math.h>

extern UART_HandleTypeDef huart3;
int Checksum_Calc, count = 0;
void UART_Send(uint8_t u8_data) {
	uint8_t *u8_pointer = &u8_data;
	HAL_UART_Transmit(&huart3, u8_pointer, 1, 100);

	Checksum_Calc += u8_data;
}

void UART_Send_SetMotorPosition(uint16_t motorId, uint16_t Position,
		uint16_t Time) {
	Checksum_Calc = 0;
	UART_Send(0x80 + motorId);    //header mark & broadcast ID
	UART_Send(0x83);              //header mark & command code
	UART_Send(0x05);              //total data length
	UART_Send((Position / 256) & 0x7F);  //Servo Pos_H
	UART_Send(Position % 256);           //Servo Pos_L
	UART_Send((Time / 256) & 0x7F); //Servo Time_H
	UART_Send(Time % 256);          //Servo Time_L
	UART_Send(Checksum_Calc);     //data length (one servo with time and speed)
}

