#include"cake.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern int acc, acc_temp;
//order:high to low==1->2->3

extern int step1, dir_state1, anglegoal, anglebefore;
extern int states;
extern int step_state1, ms1;
extern int suck1, suck4, suck3;
extern int unsuck1, unsuck4, unsuck3;
extern int suck_success1, suck_success4, suck_success3;
extern int unsuck_success1, unsuck_success4, unsuck_success3;
extern int i1, i2, i3, i4, i5, i6;
extern int suck_temp, unsuck_temp;
extern int cake_order;
int num;
extern char hole[4];
extern int arr;
extern short int mission_array[5];
extern int unsuck_delay, suck_delay, servo_delay;
extern int unsuck_delay_temp, suck_delay_temp, servo_delay_temp;
void suck_the_cake(int servonum, int order) {

	while (1) {
		if (step1 == 0) {
			break;
		}

	}
	if (servonum == 1) {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 20000 * 255); // open pump1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);        // close valve1
		suck1 = 1;
		suck_temp++;
	}
	else if (servonum == 4) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 20000 * 255); // open pump4
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);         // close valve4
		suck4 = 1;
		suck_temp++;
	}
	else if (servonum == 3) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 20000 * 255); // open pump3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);         // close valve3
		suck3 = 1;
		suck_temp++;
	}
	if (order == 1)
		UART_Send_SetMotorPosition(servonum, 1780, 500); // suck the top cake
	else if (order == 2)
		UART_Send_SetMotorPosition(servonum, 1880, 500); // suck the middle cake
	else if (order == 3)
		UART_Send_SetMotorPosition(servonum, 2080, 500); // suck the bottom cake

}
void press_sensor_feedback() {          // wait for press_sensor feedback

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
}
void unsuck_the_cake(int servonum, int order) {

	while (1) {
		if (step1 == 0)
			break;
	}

	if (order == 1)
		UART_Send_SetMotorPosition(servonum, 1780, 500); // put the cake to the top 1320
	else if (order == 2)
		UART_Send_SetMotorPosition(servonum, 1780, 500); // put the cake to the middle 1225
	else if (order == 3)
		UART_Send_SetMotorPosition(servonum, 1780, 500); // put the cake to the bottom 1100

	if (servonum == 1) {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 2000 * 255); // open valve1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);          // close pump1
		unsuck1 = 1;
		unsuck_temp++;
	}
	else if (servonum == 4) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 2000 * 255); // open valve4
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);          // close pump4
		unsuck4 = 1;
		unsuck_temp++;
	}
	else if (servonum == 3) {
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 2000 * 255); // open valve3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);          // close pump3
		unsuck3 = 1;
		unsuck_temp++;
	}
}
void rotate_the_ring(int num) {
	if (num == 1) {
		step1 = 0;
		anglegoal = 0;
	}
	else if (num == 2) {
		step1 = 0;
		anglegoal = 90;
	}
	else if (num == 3) {
		step1 = 0;
		anglegoal = 180;
	}
	else if (num == 4) {
		step1 = 0;
		anglegoal = 270;
	}
	else if (num == 5) {
		step1 = 0;
		anglegoal = -90;
	}
	else if (num == 6) {
		step1 = 0;
		anglegoal = -180;
	}
	else if (num == 7) {
		step1 = 0;
		anglegoal = -270;
	}
	step1 = (anglegoal - anglebefore) * 5 * 8 / 1.8; // calculate the step1
	anglebefore = anglegoal;
	if (step1 >= 0)
		dir_state1 = 1;
	else if (step1 < 0) {
		step1 = -step1;
		dir_state1 = 0;
	}

	while (1) {
		if (step1 == 0) {
			arr = 500;
			break;
		}
	}
}
void put_the_cherry(int num) {
	rotate_the_ring(num);
	UART_Send_SetMotorPosition(6, 2100, 300);
	UART_Send_SetMotorPosition(6, 950, 300);
}
void judge_the_empty_and_order() {
//1=leftfront,2=rightfront,3=rightback,4=leftback
// let three servo turn to the correct side
	for (int i = 0; i < 4; i++) {
		if (hole[i] == 0) {
			num = i + 1;
			break;
		}
	}
	if (num == 1) {
		rotate_the_ring(2);       //90
		anglebefore = 0;
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
		rotate_the_ring(6);       //-180
		anglebefore = 0;
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
		rotate_the_ring(5);       //-90
		anglebefore = 0;
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
		anglebefore = 0;
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
			mission_array[2] = 0;
			mission_array[3] = 1;
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
		if (step1 == 0)
			break;
	}
}
int tower_step = 0;
void Tower_of_Hanoi(int cake_order) {
	if (cake_order == -1)
		return;

	suck_the_cake(1, 1);
	suck_the_cake(4, 1);
	suck_the_cake(3, 1);
	press_sensor_feedback();
	if (cake_order == 1) {
		rotate_the_ring(5);
		tower_step = 1;

		unsuck_the_cake(1, 3);
		press_sensor_feedback();
		tower_step = 2;

		rotate_the_ring(6);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		tower_step = 3;

		rotate_the_ring(7);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		tower_step = 4;

		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		tower_step = 5;

		rotate_the_ring(5);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		tower_step = 6;

		rotate_the_ring(6);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		tower_step = 7;

		rotate_the_ring(1);
		suck_the_cake(4, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		tower_step = 8;

		rotate_the_ring(2);
		unsuck_the_cake(1, 3);
		press_sensor_feedback();
		tower_step = 9;

		rotate_the_ring(1);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		tower_step = 10;

		rotate_the_ring(5);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		tower_step = 11;
		put_the_cherry(1);
		put_the_cherry(2);
		put_the_cherry(3);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}

	else if (cake_order == 2) {
		rotate_the_ring(2);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(3);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		rotate_the_ring(4);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		rotate_the_ring(3);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(4, 3);
		suck_the_cake(1, 3);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(1);
		unsuck_the_cake(4, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		put_the_cherry(1);
		put_the_cherry(5);
		put_the_cherry(6);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}

	else if (cake_order == 3) {
		rotate_the_ring(3);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		rotate_the_ring(4);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		rotate_the_ring(3);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(1);
		unsuck_the_cake(3, 1);
		press_sensor_feedback();
		put_the_cherry(1);
		put_the_cherry(5);
		put_the_cherry(6);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}

	else if (cake_order == 4) {
		rotate_the_ring(6);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		rotate_the_ring(7);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(4, 3);
		press_sensor_feedback();
		rotate_the_ring(6);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(1);
		unsuck_the_cake(1, 1);
		press_sensor_feedback();
		put_the_cherry(1);
		put_the_cherry(2);
		put_the_cherry(3);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}

	else if (cake_order == 5) {
		rotate_the_ring(5);
		unsuck_the_cake(1, 3);
		press_sensor_feedback();
		rotate_the_ring(7);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(6);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(6);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(4, 3);
		suck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(1, 3);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(1);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		put_the_cherry(1);
		put_the_cherry(2);
		put_the_cherry(3);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}

	else if (cake_order == 6) {
		rotate_the_ring(2);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(4);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(3);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(1, 2);
		suck_the_cake(4, 2);
		suck_the_cake(3, 2);
		press_sensor_feedback();
		rotate_the_ring(3);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		rotate_the_ring(1);
		suck_the_cake(4, 3);
		suck_the_cake(1, 3);
		press_sensor_feedback();
		rotate_the_ring(5);
		unsuck_the_cake(3, 3);
		press_sensor_feedback();
		rotate_the_ring(2);
		unsuck_the_cake(1, 2);
		press_sensor_feedback();
		rotate_the_ring(1);
		unsuck_the_cake(4, 1);
		press_sensor_feedback();
		put_the_cherry(1);
		put_the_cherry(5);
		put_the_cherry(6);
		mission_array[0] = 1;
		rotate_the_ring(1);
	}
}
