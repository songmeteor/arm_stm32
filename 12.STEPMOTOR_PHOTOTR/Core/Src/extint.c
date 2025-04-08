#include "extint.h"
#include "extern.h"
#include "stepmotor.h"

extern uint8_t stepmotor_state;

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
		stepmotor_state = IDLE;
printf("GPIO_PIN_0\n");
		break;
	case GPIO_PIN_4:   //PA4 / BTN1
		if(stepmotor_state == IDLE)
			stepmotor_state = FORWARD;
		else if(stepmotor_state == FORWARD)
			stepmotor_state = BACKWARD;
		else if(stepmotor_state == BACKWARD)
			stepmotor_state = FORWARD;
printf("GPIO_PIN_4\n");
		break;
	}
}
