#ifndef MAINPP_H_
#define MAINPP_H_

#ifdef __cplusplus

#include "usart.h"
#include "ros.h"
#include "std_msgs/Int16MultiArray.h"

extern "C" {

#endif

void setup(void);
void loop(void);
void pub_to_ros();
void pub_the_gate();
//extern void MX_USART1_UART_Init(void);

#ifdef __cplusplus
}

#endif

#endif /* MAINPP_H_ */
