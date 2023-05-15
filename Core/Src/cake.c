#include"cake.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include"XBee_TX.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern int acc, acc_temp;
extern int8_t VL53_Status;
//order:high to low==1->2->3

extern int step1, dir_state1, anglegoal, anglebefore;
extern int states;
extern int step_state1, ms1;
extern int suck[4], unsuck[4];
extern int suck_success1, suck_success4, suck_success3;
extern int unsuck_success1, unsuck_success4, unsuck_success3;
extern int i1, i2, i3, i4, i5, i6;
extern int suck_temp, unsuck_temp;
extern int cake_order;
extern int num, rotate, S, US;
extern char hole[4];
extern int arr, step, pauseHanoi, homefree;
extern short int mission_array[5];
extern int unsuck_delay, suck_delay, servo_delay, cherry_delay;
extern int unsuck_delay_temp, suck_delay_temp, servo_delay_temp,
		cherry_delay_temp;
extern int rotate_state[3], rotate_dir, rotate_temp, temp_pub;
extern int CakeStatus[3];
extern int if_gate_ok, finish;
int s = 0;
int step_debug;
int rotate_offset = 0;
short int hand_transfer[4] = { 0 };
int cake_layer[4] = { 0 };
int handStatus[3] = { 0 };
int dont_go_down[3] = { 0 };
int order_arror[4] = { 0 };
extern int steady;
int first_state[3] = { 0 };
extern int disguise, home, steal, putcherry, score_flag[10];
extern uint8_t score;
char scorecount[15] = { 0 };
extern int transfer_the_order;
void score_transfer() {
	if (steady == 1
			&& (score_flag[0] == 1 || score_flag[1] == 1 || score_flag[2] == 1
					|| score_flag[3] == 1)) {
		score = bee("legend");
		HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("legend")));

//		strcpy(scorecount, "cherry");
//		score = bee(scorecount);
//		HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));

	}
	else if (steady == 0) {
		for (int i = 0; i < 4; i++) {
			if (first_state[i] == 1 && score_flag[i] == 1) {
				score = bee("one");
				HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("one")));
//				strcpy(scorecount, "cherry");
//				score = bee(scorecount);
//				HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
			}
			else if (first_state[i] == 2 && score_flag[i] == 1) {
				score = bee("two");
				HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("two")));
//				strcpy(scorecount, "cherry");
//				score = bee(scorecount);
//				HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
			}
			else if (first_state[i] == 3 && score_flag[i] == 1) {
				score = bee("three");
				HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("three")));
//				strcpy(scorecount, "cherry");
//				score = bee(scorecount);
//				HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));

			}
		}
	}
	if (disguise == 1 && score_flag[4] == 1) {
		score = bee("funny");
		HAL_UART_Transmit_IT(&huart1, &score, sizeof(score));
	}
	if (finish == 1 && score_flag[5] == 1) {
		score = bee("end");
		HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("end")));
	}
	if (home == 1 && score_flag[6] == 1) {
		score = bee("home");
		HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("home")));
	}
	if (steal != 0 && score_flag[7] == 1) {
		for (int i = 1; i < 4; i++) {
			if (steal >= i) {
				score = bee("steal");
				HAL_UART_Transmit_IT(&huart1, &score, sizeof(bee("steal")));
			}
		}
		steal = 0;
	}

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

}
void hand_transfer_fn() {
	for (int i = 0; i < 4; i++) {
		if (unsuck[i] == 1 || suck[i] == 1) {
			hand_transfer[(i + rotate_offset) % 4] = 0;
		}
		else {
			hand_transfer[(i + rotate_offset) % 4] = 1;
		}
	}
}
void suck_the_cake(int servonum, int order) {
	if (finish == 1)
		return;

	while (1) {
		if (step == 0) {
			break;
		}

	}
	if (servonum == 7) {
		cake_layer[(0 + rotate_offset) % 4] -= 1;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 20000 * 255); // open pump1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);        // close valve1
		suck[0] = 1;
		handStatus[0] = 1;
		suck_temp++;
	}
	if (servonum == 4) {
		cake_layer[(1 + rotate_offset) % 4] -= 1;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 20000 * 255); // open pump4
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);         // close valve4
		suck[1] = 1;
		handStatus[1] = 1;
		suck_temp++;
	}
	if (servonum == 3) {
		cake_layer[(2 + rotate_offset) % 4] -= 1;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 20000 * 255); // open pump3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);         // close valve3
		suck[2] = 1;
		handStatus[2] = 1;
		suck_temp++;
	}
	hand_transfer_fn();
	if (order == 1) {
//		if (servonum == 7)
		UART_Send_SetMotorPosition(servonum, 1500, 200); // suck the top cake
		S = 1;
//		if (servonum == 7) {
//			UART_Send_SetMotorPosition(servonum, 1490, 200); // suck the top cake
//		}
	}

	else if (order == 2) {
//		if (servonum != 7)
		UART_Send_SetMotorPosition(servonum, 1590, 200); // suck the middle cake
		S = 2;
//		if (servonum == 7)
//			UART_Send_SetMotorPosition(servonum, 1560, 200); // suck the top cake
	}

	else if (order == 3) {
//		if (servonum != 7)
		UART_Send_SetMotorPosition(servonum, 1715, 200); // suck the bottom cake
		S = 3;
//	if (servonum == 7)
//			UART_Send_SetMotorPosition(servonum, 1705, 200); // suck the top cake
	}

	else if (order == 0) {
//		if (servonum != 7)
		UART_Send_SetMotorPosition(servonum, 1440, 200);
		S = 0;
//		if (servonum == 7)
//			UART_Send_SetMotorPosition(servonum, 1430, 200); // suck the top cake
	}

//	HAL_Delay(1000);

}

void press_sensor_feedback() {          // wait for press_sensor feedback
	if (finish == 1)
		return;

	if (suck_temp != 0) {
		suck_delay_temp = -1;
		while (1) {
			if (suck_temp == 0) {
				if (suck_delay_temp == -1)
					suck_delay_temp = 1;
				if (suck_delay_temp == 0)
					break;
			}
		}
	}
	else if (unsuck_temp != 0) {
		while (1) {
			if (unsuck_temp == 0) {
				if (unsuck_delay_temp == 0)
					break;
			}
		}
	}
	hand_transfer_fn();
}
void cake_check(int VL53_check, int order) {
	if (finish == 1)
		return;

	if (VL53_check == 0) {
		VL53_Update();
		if (CakeStatus[0] != (4 - order)) {
			order_arror[0] = CakeStatus[0];
			CakeStatus[0] = 0;
		}
	}
	if (VL53_check == 1) {
		VL53_Update();
		if (CakeStatus[1] != (4 - order)) {
			CakeStatus[1] = 0;
		}
	}
	if (VL53_check == 2) {
		VL53_Update();
		if (CakeStatus[2] != (4 - order)) {
			order_arror[2] = CakeStatus[2];
			CakeStatus[2] = 0;
		}
	}
	if (VL53_check == 3) {
		rotate_the_ring((rotate_offset + 1) % 4);
		VL53_Update();
		if (CakeStatus[2] != (4 - order)) {
			order_arror[3] = CakeStatus[2];
			CakeStatus[2] = 0;
		}
		rotate_the_ring((rotate_offset - 1) % 4);
	}
	if (VL53_check == 4) {
		VL53_Update();
		for (int i = 0; i < 3; i++) {
			if (CakeStatus[i] != 3) {
				steady = 0;
			}
		}
		if (steady == 0) {
			for (int i = 0; i < 3; i++) {
				first_state[i] = CakeStatus[i];
			}
		}
	}

}
void unsuck_the_cake(int servonum, int order) {
	if (finish == 1)
		return;

	if (servonum == 7 && handStatus[0] == 0) {
		dont_go_down[0] = 1;
		return;
	}

	if (servonum == 4 && handStatus[1] == 0) {
		dont_go_down[1] = 1;
		return;
	}

	if (servonum == 3 && handStatus[2] == 0) {
		dont_go_down[2] = 1;
		return;
	}

	while (1) {
		if (step == 0)
			break;
	}
	if (servonum == 7) {
		cake_layer[(0 + rotate_offset) % 4] += 1;
		unsuck[0] = 1;
		unsuck_temp++;
		handStatus[0] = 0;
	}
	if (servonum == 4) {
		cake_layer[(1 + rotate_offset) % 4] += 1;
		unsuck[1] = 1;
		unsuck_temp++;
		handStatus[1] = 0;
	}
	if (servonum == 3) {
		cake_layer[(2 + rotate_offset) % 4] += 1;
		unsuck[2] = 1;
		unsuck_temp++;
		handStatus[2] = 0;
	}
	hand_transfer_fn();
	if (order == 1)
		UART_Send_SetMotorPosition(servonum, 1480, 200); // put the cake to the top 1320
	else if (order == 2)
		UART_Send_SetMotorPosition(servonum, 1480, 200); // put the cake to the middle 1225
	else if (order == 3)
		UART_Send_SetMotorPosition(servonum, 1480, 200); // put the cake to the bottom 1100

	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0)
			break;
	}
	if (servonum == 7) {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 2000 * 255); // open valve1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);      // close pump1

	}
	else if (servonum == 4) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 2000 * 255); // open valve4
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);      // close pump4

	}
	else if (servonum == 3) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 2000 * 255); // open valve3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);      // close pump3

	}

}
void rotate_the_ring(int numb) {
	if (num == 1 && cake_order != -1) {
		rotate_offset = ((numb + 1) % 4);
//		if (rotate_offset == 0)
//			rotate_offset = 4;

		numb = rotate_offset;
	}
	else if (num == 2 && cake_order != -1) {
		rotate_offset = (numb + 2) % 4;
//		if (rotate_offset == 0)
//			rotate_offset = 4;

		numb = rotate_offset;
	}
	else if (num == 3 && cake_order != -1) {
		rotate_offset = (numb + 3) % 4;
//		if (rotate_offset == 0)
//			rotate_offset = 4;

		numb = rotate_offset;
	}
	else if (num == 4 && cake_order != -1)
		rotate_offset = numb;
	else
		rotate_offset = numb;
	rotate_state[0] = 0;
	rotate_state[1] = 0;
	rotate_state[2] = 0;
	if (numb == 0) {
		step1 = 0;
		anglegoal = 0;

	}
	else if (numb == 1) {
		step1 = 0;
		anglegoal = 90;
	}
	else if (numb == 2) {
		step1 = 0;
		anglegoal = 180;
	}
	else if (numb == 3) {
		step1 = 0;
		anglegoal = 270;
	}
//	else if (numb == 5) {
//		step1 = 0;
//		anglegoal = -90;
//	}
//	else if (numb == 6) {
//		step1 = 0;
//		anglegoal = -180;
//	}
//	else if (numb == 7) {
//		step1 = 0;
//		anglegoal = -270;
//	}
	step1 = (anglegoal - anglebefore) * 5 * 8 / 1.8; // calculate the step1
	if (step1 > 4000) {
		step1 = step1 - 8000;
	}
	else if (step1 < -4000) {
		step1 = 8000 + step1;
	}

	rotate_temp = 0;
	anglebefore = anglegoal;
	if (step1 >= 0) {
		dir_state1 = 1;

	}

	else if (step1 < 0) {
		step1 = -step1;

		dir_state1 = 0;
	}
	step1 = step1 + 500;
	step = step1;
	step_debug = step;
	while (1) {
		if (finish == 1) {
			finish_all_motion();
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 20000 * 255);
			break;
		}
		if (numb == rotate_dir) {

			step = 2.86 * 5 * 8 / 1.8;
			step_debug = step;
			s++;
			numb = 5;
		}
		if (step == 0 && numb == 5) {
			arr = 1000;

			break;
		}
		else if (step == 0 && numb != 5) {

			step = 90 * 5 * 8 / 1.8;
			step_debug = step;
		}
	}
}
void put_the_cherry(int numb) {
	if (finish == 1)
		return;
	rotate_the_ring(numb);
	while (1) {
		if (step == 0) {
			break;
		}
	}
	suck[3] = 1;
	if (steady == 0) {
		if (order_arror[(4 + rotate_offset) % 4] == 1) {
			UART_Send_SetMotorPosition(5, 1330, 300);
			S = 1;
		}

		if (order_arror[(4 + rotate_offset) % 4] == 2) {
			UART_Send_SetMotorPosition(5, 1350, 300);
			S = 2;
		}
		if (order_arror[(4 + rotate_offset) % 4] == 3) {
			UART_Send_SetMotorPosition(5, 1450, 300);
			S = 3;
		}

		else {
			UART_Send_SetMotorPosition(5, 1330, 300);
			S = 1;
		}

	}
	if (steady == 1) {
		UART_Send_SetMotorPosition(5, 1330, 300);
		S = 1;
	}

	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0)
			break;
	}
	UART_Send_SetMotorPosition(6, 2100, 500);
	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0)
			break;
	}
	cherry_delay_temp = 1;
	while (1) {
		if (cherry_delay_temp == 0)
			break;
	}
	UART_Send_SetMotorPosition(5, 1200, 300);
	suck[3] = 0;

	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0)
			break;
	}
	UART_Send_SetMotorPosition(6, 1000, 500);
	servo_delay_temp = 1;
	while (1) {
		if (servo_delay_temp == 0)
			break;
	}

}
void judge_the_empty_and_order() {
//1=leftfront,2=rightfront,3=rightback,4=leftback
// let three servo turn to the correct side
	if (if_gate_ok == 0)
		return;
	for (int i = 0; i < 4; i++) {
		if (hole[i] != 0)
			cake_layer[i] = 3;
		if (hole[i] == 0) {
			num = i + 1;
			cake_layer[i] = 0;
		}
	}

	if (num == 1) {
		if (transfer_the_order == 0) {
			rotate_the_ring(1);       //90
			rotate_offset = 1;
		}

		if (hole[1] == 'b' && hole[2] == 'y' && hole[3] == 'p') {
			cake_order = 1;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
		else if (hole[1] == 'p' && hole[2] == 'y' && hole[3] == 'b') {
			cake_order = 2;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[1] == 'y' && hole[2] == 'b' && hole[3] == 'p') {
			cake_order = 3;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[1] == 'p' && hole[2] == 'b' && hole[3] == 'y') {
			cake_order = 4;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
		else if (hole[1] == 'b' && hole[2] == 'p' && hole[3] == 'y') {
			cake_order = 5;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
		else if (hole[1] == 'y' && hole[2] == 'p' && hole[3] == 'b') {
			cake_order = 6;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
	}
	else if (num == 2) {
		if (transfer_the_order == 0) {
			rotate = 2;
			rotate_the_ring(2);       //-180
			rotate_offset = 2;
		}

		if (hole[2] == 'b' && hole[3] == 'y' && hole[0] == 'p') {
			cake_order = 1;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[2] == 'p' && hole[3] == 'y' && hole[0] == 'b') {
			cake_order = 2;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
		else if (hole[2] == 'y' && hole[3] == 'b' && hole[0] == 'p') {
			cake_order = 3;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
		else if (hole[2] == 'p' && hole[3] == 'b' && hole[0] == 'y') {
			cake_order = 4;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[2] == 'b' && hole[3] == 'p' && hole[0] == 'y') {
			cake_order = 5;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[2] == 'y' && hole[3] == 'p' && hole[0] == 'b') {
			cake_order = 6;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
	}
	else if (num == 3) {
		if (transfer_the_order == 0) {
			rotate_the_ring(3);
			rotate_offset = 3;
		}

		if (hole[3] == 'b' && hole[0] == 'y' && hole[1] == 'p') {
			cake_order = 1;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[3] == 'p' && hole[0] == 'y' && hole[1] == 'b') {
			cake_order = 2;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
		else if (hole[3] == 'y' && hole[0] == 'b' && hole[1] == 'p') {
			cake_order = 3;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
		else if (hole[3] == 'p' && hole[0] == 'b' && hole[1] == 'y') {
			cake_order = 4;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[3] == 'b' && hole[0] == 'p' && hole[1] == 'y') {
			cake_order = 5;
			mission_array[1] = 1;
			mission_array[2] = 0;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[3] == 'y' && hole[0] == 'p' && hole[1] == 'b') {
			cake_order = 6;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 0;
		}
	}
	else if (num == 4) {
		rotate_offset = 0;
		if (hole[0] == 'b' && hole[1] == 'y' && hole[2] == 'p') {
			cake_order = 1;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
		else if (hole[0] == 'p' && hole[1] == 'y' && hole[2] == 'b') {
			cake_order = 2;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[0] == 'y' && hole[1] == 'b' && hole[2] == 'p') {
			cake_order = 3;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
		else if (hole[0] == 'p' && hole[1] == 'b' && hole[2] == 'y') {
			cake_order = 4;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
		else if (hole[0] == 'b' && hole[1] == 'p' && hole[2] == 'y') {
			cake_order = 5;
			mission_array[1] = 1;
			mission_array[2] = 1;
			mission_array[3] = 0;
			mission_array[4] = 1;
		}
		else if (hole[0] == 'y' && hole[1] == 'p' && hole[2] == 'b') {
			cake_order = 6;
			mission_array[1] = 0;
			mission_array[2] = 1;
			mission_array[3] = 1;
			mission_array[4] = 1;
		}
	}
	while (1) {
		if (step == 0)
			break;
	}
}

void pauseHanoiing() {
	if (finish == 1)
		return;

	int paused = 0;
	if (pauseHanoi == 1) {
		mission_array[0] = 1;
		temp_pub = 0;
		paused = 1;
		unsuck_the_cake(7, 1);
		unsuck_the_cake(4, 1);
		unsuck_the_cake(3, 1);
//		cake_check(0, cake_layer[0]);
//		cake_check(1, cake_layer[1]);
//		cake_check(2, cake_layer[2]);
//		cake_check(3, cake_layer[3]);
		press_sensor_feedback();
	}

	while (pauseHanoi != 0) {
		if (finish == 1) {
			finish_all_motion();
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 20000 * 255);
			return;
			break;
		}
	}

	if (paused == 1) {
		mission_array[0] = 0;
		if (dont_go_down[0] == 0) {
			suck_the_cake(7, (4 - cake_layer[(0 + rotate_offset) % 4]));

		}
		if (dont_go_down[1] == 0) {
			suck_the_cake(4, (4 - cake_layer[(1 + rotate_offset) % 4]));

		}

		if (dont_go_down[2] == 0) {
			suck_the_cake(3, (4 - cake_layer[(2 + rotate_offset) % 4]));

		}
		for (int i = 0; i < 3; i++)
			dont_go_down[i] = 0;

		UART_Send_SetMotorPosition(7, 1300, 0);
		UART_Send_SetMotorPosition(4, 1200, 200);
		UART_Send_SetMotorPosition(3, 1200, 200);
//		HAL_Delay(1500);
		press_sensor_feedback();

	}
}

int tower_step = 0;
void Tower_of_Hanoi(int cake_order) {
	if (if_gate_ok == 0)
		return;
	if (cake_order == -1)
		return;
	suck_the_cake(7, 1);
	suck_the_cake(4, 1);

	suck_the_cake(3, 1);

	press_sensor_feedback();
	pauseHanoiing();
	if (cake_order == 1) {

		rotate_the_ring(3);
		tower_step = 1;

		unsuck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 2;

		rotate_the_ring(2);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 3;

		rotate_the_ring(1);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 4;

		rotate_the_ring(0);
		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 5;

		rotate_the_ring(3);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 6;

		rotate_the_ring(2);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 7;

		rotate_the_ring(0);
		suck_the_cake(4, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 8;

		rotate_the_ring(1);
		unsuck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 9;

		rotate_the_ring(0);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		tower_step = 10;

		rotate_the_ring(3);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		tower_step = 11;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(1);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}

		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}

	else if (cake_order == 2) {
		rotate_the_ring(1);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(4, 3);
		suck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(3);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}

	else if (cake_order == 3) {
		rotate_the_ring(2);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(7, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(3);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}

	else if (cake_order == 4) {
		rotate = 2;
		rotate_the_ring(2);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 1;
		rotate_the_ring(1);

		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 3;
		rotate_the_ring(3);

		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 0;
		rotate_the_ring(0);

		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 3;
		rotate_the_ring(3);

		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 1;
		rotate_the_ring(1);

		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 0;
		rotate_the_ring(0);

		suck_the_cake(7, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 3;
		rotate_the_ring(3);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 2;
		rotate_the_ring(2);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate = 0;
		rotate_the_ring(0);
		unsuck_the_cake(7, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(1);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}

		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}

	else if (cake_order == 5) {
		rotate_the_ring(3);
		unsuck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(4, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(1);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}

		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}

	else if (cake_order == 6) {
		rotate_the_ring(1);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(7, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(2);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		suck_the_cake(4, 3);
		suck_the_cake(7, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(3);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(1);
		unsuck_the_cake(7, 2);
		press_sensor_feedback();
		pauseHanoiing();
		rotate_the_ring(0);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		pauseHanoiing();
		homefree = 1;
		cake_check(0, 0);
		cake_check(1, 0);
		cake_check(2, 0);
		cake_check(3, 0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(0);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(3);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}
		put_the_cherry(2);
		if (pauseHanoi == 1) {
			mission_array[0] = 1;
			temp_pub = 0;
		}

		rotate_the_ring(0);
		mission_array[0] = 2;
		temp_pub = 0;
	}
}
void finish_all_motion() {
	UART_Send_SetMotorPosition(1, 0, 0);
	UART_Send_SetMotorPosition(4, 0, 0);
	UART_Send_SetMotorPosition(3, 0, 0);
	UART_Send_SetMotorPosition(5, 0, 0);
	UART_Send_SetMotorPosition(6, 0, 0);
	HAL_GPIO_WritePin(Step1_EN_Port, Step1_EN_Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}
