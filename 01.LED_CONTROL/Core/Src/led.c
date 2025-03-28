#include "led.h"


void led_all_on(void)
{
#if 1
   //printf(*int %d\n" ,sizeof(int));  //int가 32bit 인지(4bytes)인지 확인
  *(unsigned int*)GPIOB_ODR = 0xff;
#else

	//	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
			GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , 1);
#endif
}

void led_all_off(void)
{


#if 1
  *(unsigned int*)GPIOB_ODR = 0x00;
#else
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
				GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , 0);
#endif
}

void shift_left_led_on(void)
{
	led_all_off();
#if 1
	for(int i=0 ; i<8 ; i++)
	{
		*(unsigned int*)GPIOB_ODR = 0x01 << i;
		HAL_Delay(50);
	}
#else
	for(int i=0 ; i<8 ; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b00000001 << i, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOB, 0b00000001 << i, 0);
	}
#endif
}

void shift_right_led_on(void)
{
	led_all_off();
#if 1
	for(int i=0 ; i<8 ; i++)
	{
		*(unsigned int*)GPIOB_ODR = 0x80 >> i;
		HAL_Delay(50);
	}
#else
	for(int i=0 ; i<8 ; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b10000000 >> i, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOB, 0b10000000 >> i, 0);
	}
#endif
}

void shift_left_keep_led_on(void)
{
	led_all_off();
#if 1
	for(int i=0; i<8 ; i++)
	{
		*(unsigned int*)GPIOB_ODR |= 0x01 << i;
		HAL_Delay(50);
	}
#else
	for(int i=0; i<8 ; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b00000001 << i, 1);
		HAL_Delay(50);
	}
#endif
}

void shift_right_keep_led_on(void)
{
	led_all_off();
#if 1
	for(int i=0; i<8 ; i++)
	{
		*(unsigned int*)GPIOB_ODR |= 0x80 >> i;
		HAL_Delay(50);
	}
#else
	for(int i=0; i<8 ; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b10000000 >> i, 1);
		HAL_Delay(50);;
	}
#endif
}

void flower_on(void)
{

#if 1 //구초제 pointer member access
	for(int i=0; i<4; i++)
	{
		GPIOB->ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(50);
	}
		led_all_off();
		HAL_Delay(50);
#endif

#if 0 //DMA
	for(int i=0; i<4; i++)
	{
		*(unsigned int*)GPIOB_ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(50);
	}
#endif

#if 0 //HAL
	for(int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b00011000 << i, 1);
		HAL_GPIO_WritePin(GPIOB, 0b00011000 >> i, 1);
		HAL_Delay(50);
	}
#endif
	led_all_off();
}

void flower_off(void)
{
	led_all_on();
#if 1
	for(int i=0; i<4; i++)
	{
		*(unsigned int*)GPIOB_ODR &= 0b01111110 >> i & 0b01111110 << i;
		HAL_Delay(50);
	}
#else
	for(int i=0; i<4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0b00000001 << i, 0);
		HAL_GPIO_WritePin(GPIOB, 0b10000000 >> i, 0);
		HAL_Delay(50);
	}
#endif
}

void led_main(void)
{
	while(1)
	{
		//(*GPIOB).ODR
//		GPIOB->ODR ^= GPIO_PIN_0 | GPIO_PIN_3;
//		HAL_Delay(500);

//		led_all_on();
//		HAL_Delay(100);
//		led_all_off();
//		HAL_Delay(100);
//
//		shift_left_led_on();
//		shift_right_led_on();
//    	shift_left_keep_led_on();
//		shift_right_keep_led_on();
	    flower_on();
//		flower_off();
	}
}
