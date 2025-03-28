#include "main.h"  // HAL_deriver GPIO 정보가 들어 있다.

#define GPIOB_ODR  0x40020414

void led_all_on(void);
void led_all_off(void);
void led_main(void);

void shift_left_led_on(void);
void shift_right_led_on(void);
void shift_left_keep_led_on(void);
void shift_right_keep_led_on(void);
void flower_on(void);
void flower_off(void);
