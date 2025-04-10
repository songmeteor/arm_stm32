#include "extint.h"
#include "extern.h"
#include "stepmotor.h"
#include "elevator.h"

extern uint8_t stepmotor_state;
extern enum CURRENT_FLOOR current_floor;
extern enum CURRENT_STATE current_state ;
extern enum CURRENT_DOOR_STATE current_door_state;
extern int target_floor[5];
extern uint8_t buzzer_start;

uint32_t last_time_pin0 = 0;
uint32_t last_time_pin4 = 0;
uint32_t last_time_pin5 = 0;
uint32_t last_time_pin7 = 0;

/**move from Drivers/ STM32F4XX_HAL_Driver/Src/stm32f4xx_hal_gpio to here
 *	external interrupt callback function
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t now = HAL_GetTick();

    switch(GPIO_Pin){
        case GPIO_PIN_0:
        	current_floor = first;
        	init_dotmatrix(one);
        	if(target_floor[1] == 0) return;
            if (now - last_time_pin0 < 200) return;
            last_time_pin0 = now;
            current_door_state = open;
            stepmotor_state = IDLE;
            target_floor[1] = 0;
            buzzer_start = 1;
            //printf("first\n");
            break;

        case GPIO_PIN_4:
            current_floor = second;
        	init_dotmatrix(two);
        	if(target_floor[2] == 0) return;
            if (now - last_time_pin4 < 200) return;
            last_time_pin4 = now;
            current_door_state = open;
            stepmotor_state = IDLE;
            target_floor[2] = 0;
            buzzer_start = 1;
            //printf("second\n");
            break;

        case GPIO_PIN_5:
            current_floor = third;
        	init_dotmatrix(three);
        	if(target_floor[3] == 0) return;
            if (now - last_time_pin5 < 200) return;
            last_time_pin5 = now;
            current_door_state = open;
            stepmotor_state = IDLE;
            target_floor[3] = 0;
            buzzer_start = 1;
            //printf("third\n");
            break;

        case GPIO_PIN_7:
            current_floor = fourth;
        	init_dotmatrix(four);
        	if(target_floor[4] == 0) return;
            if (now - last_time_pin7 < 200) return;
            last_time_pin7 = now;
            current_door_state = open;
            stepmotor_state = IDLE;
            target_floor[4] = 0;
            buzzer_start = 1;
            //printf("fourth\n");
            break;
    }
}
