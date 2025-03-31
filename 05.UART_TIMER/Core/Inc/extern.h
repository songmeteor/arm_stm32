#ifndef __EXTERN_H
#define __EXTERN_H

#include "main.h"

#define COMMAND_NUMBER 20
#define COMMAND_LENGHT 40

extern uint8_t rx_data;
extern UART_HandleTypeDef huart2;

extern volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGHT];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
extern volatile int rear; // input index : HAL_UART_RxCpltCallback에서 집어 넣어 주는 index
extern volatile int front; //output index

extern void led_main(void);
extern void button_led_toggle_test(void);
extern void ds1302_main(void);
extern void pc_command_processing(void);

#endif /* __EXTERN_H */
