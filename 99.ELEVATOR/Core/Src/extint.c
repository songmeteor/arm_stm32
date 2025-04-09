#include "extint.h"
#include "extern.h"
#include "stepmotor.h"
#include "elevator.h"

extern uint8_t stepmotor_state;
extern enum CURRENT_FLOOR current_floor;
extern enum CURRENT_STATE current_state ;
extern enum CURRENT_DOOR_STATE current_door_state;
extern int target_floor[5];

/**move from Drivers/ STM32F4XX_HAL_Driver/Src/stm32f4xx_hal_gpio to here
 *	external interrupt callback function
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
	case GPIO_PIN_0:   //PA0  / BTN0
		current_floor = first;
		current_door_state = open;
		stepmotor_state = IDLE;
		target_floor[1] = 0;
		printf("first\n");
		break;
	case GPIO_PIN_4:   //PA0  / BTN0
		current_floor = second;
		current_door_state = open;
		stepmotor_state = IDLE;
		target_floor[2] = 0;
		printf("second\n");
		break;
	case GPIO_PIN_5:   //PA0  / BTN0
		current_floor = third;
		current_door_state = open;
		stepmotor_state = IDLE;
		target_floor[3] = 0;
		printf("third\n");
		break;
	case GPIO_PIN_7:   //PA0  / BTN0
		current_floor = fourth;
		current_door_state = open;
		stepmotor_state = IDLE;
		target_floor[4] = 0;
		printf("fourth\n");
		break;
	}
}
