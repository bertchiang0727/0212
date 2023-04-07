#pragma once

#include"Servo.h"
#include "stm32f4xx_hal.h"
//step 1/ steppin(PD2)/ dir(PC12)
#define Step1_STEP_Port GPIOD
#define Step1_STEP_Pin GPIO_PIN_2
#define Step1_DIR_Port GPIOC
#define Step1_DIR_Pin GPIO_PIN_12
void suck_the_cake(int servonum, int order);
void unsuck_the_cake(int servonum, int order);
void rotate_the_ring(int num);
void judge_the_empty_and_order();
void Tower_of_Hanoi(int cake_order);
void press_sensor_feedback();
void put_the_cherry(int num);
