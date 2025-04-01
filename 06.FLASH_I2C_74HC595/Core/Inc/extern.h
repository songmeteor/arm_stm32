#ifndef __EXTERN_H
#define __EXTERN_H

#include "main.h"

//#define COMMAND_NUMBER 20
//#define COMMAND_LENGHT 40

extern uint8_t rx_data;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

//extern volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGHT];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
//extern volatile int rear; // input index : HAL_UART_RxCpltCallback에서 집어 넣어 주는 index
//extern volatile int front; //output index
extern volatile int TIM11_1ms_counter;
extern volatile int TIM11_1ms_counter2;
extern t_print o_prt;

extern void led_main(void);
extern void button_led_toggle_test(void);
extern void ds1302_main(void);
extern void pc_command_processing(void);
extern void set_RTC(char *date_time);
extern void delay_us(int us);

extern void flash_main();
extern HAL_StatusTypeDef flash_write (uint32_t *data32, int size);
extern HAL_StatusTypeDef flash_read(uint32_t *addr32, int size);
extern void flash_set_time(void);
extern HAL_StatusTypeDef flash_erase();

extern void i2c_lcd_main(void);

#endif /* __EXTERN_H */
