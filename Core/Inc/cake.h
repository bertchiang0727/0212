#pragma once

#include"Servo.h"
#include "stm32f4xx_hal.h"
#include"VL53.h"
//step 1/ steppin(PD2)/ dir(PC12)/EN(PB3)
#define Step1_STEP_Port GPIOD
#define Step1_STEP_Pin GPIO_PIN_2
#define Step1_DIR_Port GPIOC
#define Step1_DIR_Pin GPIO_PIN_12
#define Step1_EN_Port GPIOB
#define Step1_EN_Pin GPIO_PIN_3

#define LED_Port GPIOA
#define LED_Pin GPIO_PIN_15
void suck_the_cake(int servonum, int order);
void unsuck_the_cake(int servonum, int order);
void rotate_the_ring(int numb);
void judge_the_empty_and_order();
void Tower_of_Hanoi(int cake_order);
void press_sensor_feedback();
void put_the_cherry(int numb);
void hand_transfer_fn();
void score_transfer();
void cake_check();
void finish_all_motion();
