#include "led.h"
#include "extern.h"
#include "elevator.h"

extern enum CURRENT_FLOOR current_floor;
extern enum CURRENT_STATE current_state;
extern enum CURRENT_DOOR_STATE current_door_state;
extern volatile led_toggle_counter;

void led_elevator();
void led_all_on(void);

void led_elevator()
{
	uint8_t led_mask = 0;
	uint8_t led_toggle_mask = 0;

	if(current_state == up) // up
	{
	    for (int i = 0; i < current_floor; i++) {
	        led_mask |= (1 << i);
	    }

	    led_toggle_mask |= (1 << current_floor);

		if((led_toggle_counter / 500) % 2 == 0)
		{
			GPIOB->ODR = led_mask | led_toggle_mask;
			//led_toggle_counter = 0;
		}
		else
		{
			GPIOB->ODR = led_mask;
		}

	}
	else if (current_state == down) //down
	{
	    for (int i = 0; i < current_floor-1; i++) {
	        led_mask |= (1 << i);
	    }
	    led_toggle_mask |= (1 << current_floor-1);

		if((led_toggle_counter / 500) % 2 == 0)
		{
			GPIOB->ODR = led_mask | led_toggle_mask;
			//led_toggle_counter = 0;
		}
		else
		{
			GPIOB->ODR = led_mask;
		}

	}
	else // stop
	{
	    for (int i = 0; i < current_floor; i++) {
	        led_mask |= (1 << i);
	    }
		GPIOB->ODR = led_mask;
	}
}

void led_all_on(void)
{
#if 1
	// printf("int %d\n", sizeof(int));	// 4로 찍히는지 확인
	*(unsigned int *)GPIOB_ODR = 0xff;
#else	// org
//	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);
#endif
}

void led_all_off(void)
{
#if 1
	*(unsigned int *)GPIOB_ODR = 0x00;
#else	// org
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
#endif
}

void shift_left_ledon(void)
{
#if 1
	led_all_off();
		for(int i = 0; i < 8; i++)
		{
			*(unsigned int *)GPIOB_ODR = 0x01 << i;
			HAL_Delay(100);
			led_all_off();
			HAL_Delay(100);
		}
#else	// org
	led_all_off();
	for(int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i , 1);
		HAL_Delay(100);
		led_all_off();
		HAL_Delay(100);
	}
#endif
}

void shift_right_ledon(void)
{
#if 1

	led_all_off();
		for(int i = 0; i < 8; i++)
		{
			*(unsigned int *)GPIOB_ODR = 0x01 << i;
			HAL_Delay(100);
			led_all_off();
			HAL_Delay(100);
		}
#else	// org
	led_all_off();
	for(int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i , 1);
		HAL_Delay(100);
		led_all_off();
		HAL_Delay(100);
	}
#endif
}

void shift_left_keep_ledon(void)
{
#if 1
	*(unsigned int *)GPIOB_ODR = 0x00;
#else	// org
	led_all_off();
	HAL_Delay(100);
	for(int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i , 1);
		HAL_Delay(100);
	}
#endif
}

void shift_right_keep_ledon(void)
{
#if 1
	*(unsigned int *)GPIOB_ODR = 0x00;
#else	// org
	led_all_off();
	HAL_Delay(100);
	for(int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i , 1);
		HAL_Delay(100);
	}
#endif
}

void flower_on(void)
{
#if 1	// 구조체 pointer member
	led_all_off();
	HAL_Delay(100);
	for(int i = 0; i < 4; i++)
	{
		GPIOB->ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(100);
	}
#endif
#if 0	// DMA
	led_all_off();
	HAL_Delay(100);
	for(int i = 0; i < 4; i++)
	{
		*(unsigned int *)GPIOB_ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(100);
	}
#endif
#if 0	// org
	led_all_off();
	HAL_Delay(100);
	for(int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, (0x08 >> i | 0x10 << i) , 1);
		HAL_Delay(100);
	}
#endif
}

void flower_off(void)
{
#if 1
	led_all_on();
	HAL_Delay(100);
	for(int i = 0; i < 4; i++)
	{
		*(unsigned int *)GPIOB_ODR &= ~(0x80 >> i | 0x01 << i);
		HAL_Delay(100);
	}
#else
	led_all_on();
	HAL_Delay(100);
	for(int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, (0x01 << i | 0x80 >> i) , 0);
		HAL_Delay(100);
	}
#endif
}

//extern SPI_HandleTypeDef hspi2;

void led_main(void)
{

	uint8_t led_buff[8] = {0xFF, 0x0F, 0xF0, 0x00,0xFF, 0x0F, 0xF0, 0x00};

	 while (1)
	{
#if 1
		//HAL_SPI_Transmit(&hspi2,led_buff, 1, 1);
		GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down ODR(Output Data Register)
		GPIOB->ODR |= GPIO_PIN_13;    // latch핀을 pull-up ODR(Output Data Register)
		HAL_Delay(500);
		//HAL_SPI_Transmit(&hspi2,&led_buff[3], 1, 1);
		GPIOB->ODR &= ~ GPIO_PIN_13;
	    GPIOB->ODR |= GPIO_PIN_13;
	    HAL_Delay(500);
#else
		for (int i=0; i < 4; i++)
		{
		    HAL_SPI_Transmit(&hspi2, &led_buff[i], 1, 1);
		    GPIOB->ODR &= ~ GPIO_PIN_13;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_13;   //  // latch핀을 pull-up
			HAL_Delay(1000);
		}
#endif
//		flower_on();
//		(*GPIOB).ODR |= GPIO_PIN_0;		// LED0 ON
//		GPIOB->ODR ^= GPIO_PIN_1;		// LED1 toggle 반전
//		HAL_Delay(500);
//		GPIOB->ODR &= ~GPIO_PIN_0;		// LED0 OFF
//		HAL_Delay(500);
//		led_all_on();
//		HAL_Delay(500);
//		led_all_off();
//		HAL_Delay(500);
//		flower_off();
//		HAL_Delay(100);

//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//		for(int i=0; i< 50; i++)
//		{
//			delay_us(1000);
//		}
	}
}

