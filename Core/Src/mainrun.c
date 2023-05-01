#include"mainrun.h"
#include<stm32f4xx_hal.h>
#include"Servo.h"
#include"cake.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "mainpp.h"
#include"VL53.h"
#include"XBee_TX.h"
#include"string.h"
#include"stm32f4xx_hal_uart.h"
uint16_t ADC_cake[3] = { 0 }; //PA5 PA6 PA7
int step1 = 0, dir_state1 = 0, anglegoal = 0, anglebefore = 0;
int states = 0;
int step_state1 = 0, ms1 = 0;
int suck1 = 0, suck4 = 0, suck3 = 0;
int unsuck1 = 0, unsuck4 = 0, unsuck3 = 0;
int suck_temp = 0, unsuck_temp = 0;
int suck_success1 = 0, suck_success4 = 0, suck_success3 = 0;
int unsuck_success1 = 0, unsuck_success4 = 0, unsuck_success3 = 0;
int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0;
int place = 1200;
int step = 0;
char hole[4] = { 0 };
int arr = 500, temp = 0;
int vl53_sensor = 0;
int cake_order = -1;
int counttry = 0;
int num = 0;
int b_bool = 0, y_bool = 0, p_bool = 0, h_bool = 0;
int unsuck_delay = 0, suck_delay = 0, servo_delay = 0, cherry_delay = 0;
int unsuck_delay_temp = 0, suck_delay_temp = 0, servo_delay_temp = 0, temp_pub =
		0, cherry_delay_temp = 0;
short int mission_array[5] = { 0 }, rotate_state[3] = { 0 };
int rotate = 1, rotate_dir = 1, rotate_temp = 0;
int k = 1, flag = 0;
int button_gate = 0, button_step = 0, if_step_ok = 1, if_gate_ok = 1;
char scorecount[15] = { 0 };
uint8_t score;
/*cake_order=1: brown == 1 && yellow == 2 && pink == 3
 cake_order=2: brown == 3 && yellow == 2 && pink == 1
 cake_order=3: brown == 2 && yellow == 1 && pink == 3
 cake_order=4: brown == 2 && yellow == 3 && pink == 1
 cake_order=5: brown == 1 && yellow == 3 && pink == 2
 cake_order=6: brown == 3 && yellow == 1 && pink == 2
 */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim10;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//
//}
void mainrun() {

	HAL_TIM_Base_Start_IT(&htim6); //clock for step1
	HAL_TIM_Base_Start_IT(&htim7); //clock for feedback
	HAL_TIM_Base_Start_IT(&htim10); //clock for communicate
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //for pump1 PA0
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //for valve1 PA1
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //for pump4 PC6
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //for valve4 PC7
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); //for pump3 PC8
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); //for valve3 Pc9
	//set the cake
//	hole[0] = 0;
//	hole[1] = 'y';
//	hole[2] = 'p';
//	hole[3] = 'b';
	//initialization all the mosfet
	//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_cake, 3);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
//	VL53_Init();
	//initialization all the servo
	UART_Send_SetMotorPosition(1, 1200, 300);
	UART_Send_SetMotorPosition(4, 1200, 300);
	UART_Send_SetMotorPosition(3, 1200, 300);
	UART_Send_SetMotorPosition(5, 1200, 300);
	UART_Send_SetMotorPosition(6, 950, 300);
	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0) {
			break;
		}

	}
	UART_Send_SetMotorPosition(1, 1250, 300);
	UART_Send_SetMotorPosition(4, 1250, 300);
	UART_Send_SetMotorPosition(3, 1250, 300);
	UART_Send_SetMotorPosition(5, 1250, 300);
	UART_Send_SetMotorPosition(6, 2100, 300);
	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0) {
			break;
		}

	}
	UART_Send_SetMotorPosition(1, 1200, 300);
	UART_Send_SetMotorPosition(4, 1200, 300);
	UART_Send_SetMotorPosition(3, 1200, 300);
	UART_Send_SetMotorPosition(5, 1200, 300);
	UART_Send_SetMotorPosition(6, 950, 300);
	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0) {
			break;
		}

	}
//	VL53_FirstMeasurement();
//	strcpy(scorecount, "one");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "two");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "three");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "legend");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "cherry");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "funny");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "steal");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "home");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
//	HAL_Delay(2000);
//	strcpy(scorecount, "end");
//	score = bee(scorecount);
//	HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));

//	if (button_gate == 1)
//		if_gate_ok = 0;
//	if (button_step == 1) {
//		if_step_ok = 0;
////		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//	}
//	if (if_step_ok == 1) {
//		if (if_gate_ok == 1) {
//			while (1) {
//				dir_state1 = 1;
//				if (step1 == 0)
//					step1 = 90 * 5 * 8 / 1.8;
//				if (rotate_state[0] == 1) {
//					step1 = 0;
//					break;
//				}
//			}
//			dir_state1 = 1;
//			step1 = 360 * 5 * 8 / 1.8;
//			while (1) {
//				if (step1 == 0)
//					break;
//			}
//			dir_state1 = 0;
//			step1 = 360 * 5 * 8 / 1.8;
//			while (1) {
//				if (step1 == 0)
//					break;
//			}
//			if ((rotate_state[0] + rotate_state[1] + rotate_state[2]) != 3) {
//				if_gate_ok = 0;
//				//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
//
//			}
//
//		}
//	}
////
////	put_the_cherry(2);
////	put_the_cherry(3);
////	put_the_cherry(4);
//
//	if (servo_delay_temp == 0) {
////		if (b_bool == 1 && y_bool == 1 && p_bool == 1 && h_bool == 1) {
//		judge_the_empty_and_order();
//
////	UART_Send_SetMotorPosition(5, 1800, 1500);
//		Tower_of_Hanoi(cake_order); //
//		mission_array[0] = 1;
////		}
//
//// close all the mosfet
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
//		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
//	}

	while (1) {

//		servo_delay_temp = 1;
//		while (1) {
//			if (servo_delay_temp == 0)
//				break;
//		}

//		if (k == 0) {
////			UART_Send_SetMotorPosition(1, place, 500);
////			judge_the_empty_and_order();
//			rotate_the_ring(rotate);
//			k = 1;
//		}
//		rotate_the_ring(rotate);
//		states = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
//		HAL_Delay(5000);
		if (servo_delay_temp == 0) {
			if (b_bool == 1 && y_bool == 1 && p_bool == 1 && h_bool == 1) {

				judge_the_empty_and_order();
//				VL53_Update();
//			mission_array[0] = 2;
//			temp_pub = 0;
				//	rotate_the_ring(2);
				//	UART_Send_SetMotorPosition(5, 1800, 1500);
				Tower_of_Hanoi(cake_order);
				mission_array[0] = 2;
				temp_pub = 0;
				if (num == 1) {
					rotate_the_ring(4);
				}
				else if (num == 2) {
					rotate_the_ring(3);
				}
				else if (num == 3) {
					rotate_the_ring(2);
				}
				HAL_Delay(5000);
				anglebefore = 0;
				rotate_dir = 1;
				cake_order = -1;
				rotate_state[0] = 0;
				rotate_state[1] = 0;
				rotate_state[2] = 0;
				b_bool = 0;
				y_bool = 0;
				p_bool = 0;
				h_bool = 0;

			}

			// close all the mosfet
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);

		}
		if (mission_array[0] == 0) {
			UART_Send_SetMotorPosition(1, 1200, 300);
			UART_Send_SetMotorPosition(4, 1200, 300);
			UART_Send_SetMotorPosition(3, 1200, 300);
			UART_Send_SetMotorPosition(5, 1200, 300);
			UART_Send_SetMotorPosition(6, 950, 300);
		}
	}

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == GPIO_PIN_4) {            //1
		rotate_state[0] = 1;
	}

	if (GPIO_Pin == GPIO_PIN_10) {            //2
		rotate_state[1] = 1;
	}

	if (GPIO_Pin == GPIO_PIN_11) {            //3
		rotate_state[2] = 1;
	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		ms1++;
		if (step != 0) {
			if (dir_state1 == 1)
				HAL_GPIO_WritePin(Step1_DIR_Port, Step1_DIR_Pin, GPIO_PIN_SET);
			else if (dir_state1 == 0)
				HAL_GPIO_WritePin(Step1_DIR_Port, Step1_DIR_Pin,
						GPIO_PIN_RESET);
			temp++;
			if (step_state1 == 1) {
				HAL_GPIO_WritePin(Step1_STEP_Port, Step1_STEP_Pin,
						GPIO_PIN_RESET);

				step_state1 = 0;
			}
			else if (step_state1 == 0) {
				HAL_GPIO_WritePin(Step1_STEP_Port, Step1_STEP_Pin,
						GPIO_PIN_SET);
				step_state1 = 1;
				step--;
				rotate_temp++;
			}
			if (temp == 2) {
				if (step > 300) {
					arr = arr - 4;
					if (arr < 200)
						arr = 200;
					__HAL_TIM_SET_AUTORELOAD(&htim6, arr);
					temp = 0;
				}

				if (step <= 300) {
					arr = arr + 4;
					__HAL_TIM_SET_AUTORELOAD(&htim6, arr);
					temp = 0;

				}
			}
			if (flag == 0 && rotate_temp >= 444) {
				flag = 1;
				rotate_state[0] = 0;
				rotate_state[1] = 0;
				rotate_state[2] = 0;
			}
			if (rotate_state[0] + rotate_state[1] + rotate_state[2] == 3
					&& rotate_temp >= 444) {
				rotate_state[0] = 0;
				rotate_state[1] = 0;
				rotate_state[2] = 0;
				rotate_temp = 0;
				flag = 0;
				pub_the_gate();
			}
			if (rotate_temp >= 444 && flag == 1) {
				if (rotate_state[0] == 0 && rotate_state[1] == 1
						&& rotate_state[2] == 1
						&& (rotate_dir == 2 || rotate_dir == 4)) {
//						step1 = 2.86 * 5 * 8 / 1.8;
					rotate_dir = 1;
					rotate_state[0] = 0;
					rotate_state[1] = 0;
					rotate_state[2] = 0;
					rotate_temp = 0;
					flag = 0;
					pub_the_gate();
				}
				else if (rotate_state[0] == 1 && rotate_state[1] == 1
						&& rotate_state[2] == 0
						&& (rotate_dir == 1 || rotate_dir == 3)) {
//						step1 = 2.86 * 5 * 8 / 1.8;
					rotate_dir = 2;
					rotate_state[0] = 0;
					rotate_state[1] = 0;
					rotate_state[2] = 0;
					rotate_temp = 0;
					flag = 0;
					pub_the_gate();
				}
				else if (rotate_state[0] == 1 && rotate_state[1] == 0
						&& rotate_state[2] == 0
						&& (rotate_dir == 2 || rotate_dir == 4)) {
//						step1 = 2.86 * 5 * 8 / 1.8;
					rotate_dir = 3;
					rotate_state[0] = 0;
					rotate_state[1] = 0;
					rotate_state[2] = 0;
					rotate_temp = 0;
					flag = 0;
					pub_the_gate();
				}
				else if (rotate_state[0] == 0 && rotate_state[1] == 0
						&& rotate_state[2] == 1
						&& (rotate_dir == 1 || rotate_dir == 3)) {
//						step1 = 2.86 * 5 * 8 / 1.8;
					rotate_dir = 4;
					rotate_state[0] = 0;
					rotate_state[1] = 0;
					rotate_state[2] = 0;
					rotate_temp = 0;
					flag = 0;
					pub_the_gate();
				}

//				else if (rotate_state[0] == 1 && rotate_state[1] == 1
//						&& rotate_state[2] == 0
//						&& (rotate_dir == 1 || rotate_dir == 6)) {
////						step1 = 2.86 * 5 * 8 / 1.8;
//					rotate_dir = 5;
//					rotate_state[0] = 0;
//					rotate_state[1] = 0;
//					rotate_state[2] = 0;
//					rotate_temp = 0;
//					flag = 0;

//				}
//				else if (rotate_state[0] == 0 && rotate_state[1] == 1
//						&& rotate_state[2] == 1
//						&& (rotate_dir == 5 || rotate_dir == 7)) {
////						step1 = 2.86 * 5 * 8 / 1.8;
//					rotate_dir = 6;
//					rotate_state[0] = 0;
//					rotate_state[1] = 0;
//					rotate_state[2] = 0;
//					rotate_temp = 0;
//					flag = 0;
//				}
//				else if (rotate_state[0] == 0 && rotate_state[1] == 0
//						&& rotate_state[2] == 1 && rotate_dir == 6) {
////						step1 = 2.86 * 5 * 8 / 1.8;
//					rotate_dir = 7;
//					rotate_state[0] = 0;
//					rotate_state[1] = 0;
//					rotate_state[2] = 0;
//					rotate_temp = 0;
//					flag = 0;
//				}
			}

		}
	}
	if (htim->Instance == TIM7) {

		if (suck1 == 1) {           //press_sensor1 feedback(suck)
			i1++;
//			if (ADC_cake[0] < 200) {        //PA5
			if (i1 == 500) {
				suck_success1 = 1;
				suck1 = 0;
				UART_Send_SetMotorPosition(1, 1200, 300);
				i1 = 0;
				suck_temp--;
				//			}
			}
		}
		if (suck4 == 1) {             //press_sensor4 feedback(suck)
			i2++;
//			if (ADC_cake[1] < 200) {     //PA6
			if (i2 == 500) {
				suck_success4 = 1;
				UART_Send_SetMotorPosition(4, 1200, 300);
				i2 = 0;
				suck_temp--;
				suck4 = 0;
				//			}
			}
		}
		if (suck3 == 1) {             //press_sensor3 feedback(suck)
			i3++;
//			if (ADC_cake[2] < 200) {      //PA7
			if (i3 == 500) {
				suck_success3 = 1;
				suck3 = 0;
				UART_Send_SetMotorPosition(3, 1200, 300);
				i3 = 0;
				suck_temp--;
				suck3 = 0;
				//			}
			}
		}
		if (unsuck1 == 1) {         //press_sensor1 feedback(unsuck)
			i4++;

//			if (ADC_cake[0] > 1900) {       //PA5
			if (i4 == 500) {
				unsuck_success1 = 1;
				UART_Send_SetMotorPosition(1, 1200, 300);
				i4 = 0;
				unsuck_temp--;
				unsuck1 = 0;
				//			}
			}
		}
		if (unsuck4 == 1) {        // press_sensor4 feedback(unsuck)
			i5++;
//			if (ADC_cake[1] > 1900) {               //PA6
			if (i5 == 500) {
				unsuck_success4 = 1;
				UART_Send_SetMotorPosition(4, 1200, 300);
				i5 = 0;
				unsuck_temp--;
				unsuck4 = 0;
				//			}
			}
		}
		if (unsuck3 == 1) {      //press_sensor3 feedback(unsuck)
			i6++;
//			if (ADC_cake[2] > 1900) {          //PA7
			if (i6 == 500) {
				unsuck_success3 = 1;
				UART_Send_SetMotorPosition(3, 1200, 300);
				i6 = 0;
				unsuck_temp--;
				unsuck3 = 0;
				//			}
			}
		}
	}
	if (htim->Instance == TIM10) {
		loop();
		counttry++;
		if (suck_delay_temp == 1) {
			suck_delay++;
			if (suck_delay == 30) {
				suck_delay = 0;
				suck_delay_temp = 0;
			}
		}
		if (unsuck_delay_temp == 1) {
			unsuck_delay++;
			if (unsuck_delay == 30) {
				unsuck_delay = 0;
				unsuck_delay_temp = 0;
			}
		}
		if (servo_delay_temp == 1) {
			servo_delay++;
			if (servo_delay == 50) {
				servo_delay = 0;
				servo_delay_temp = 0;
			}
		}
		if (cherry_delay_temp == 1) {
			cherry_delay++;
			if (cherry_delay == 50) {
				cherry_delay = 0;
				cherry_delay_temp = 0;
			}
		}
		if (mission_array[0] >= 1 && temp_pub < 10) {
			pub_to_ros();
			temp_pub++;
		}

	}
}

