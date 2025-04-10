#include "fnd.h"
#include "extern.h"
#include "elevator.h"

extern enum CURRENT_DOOR_STATE current_door_state;

 uint32_t start_tick = 0;
 uint32_t current_tick = 0;
 uint32_t tick=0;

 void fnd_elevator(void);

 void fnd_elevator(void)
 {
     static uint8_t initialized = 0;

     if (!initialized)
     {
         start_tick = HAL_GetTick();
         initialized = 1;
     }

     current_tick = HAL_GetTick();

     if(current_tick - start_tick >= 1000)
     {
         tick++;    // count seconds
         start_tick = current_tick;
     }

     fnd_display((current_door_state == open) ? 0xC0 : 0xC6, 1);    // thousands
     fnd_display((current_door_state == open) ? 0x8C : 0xC7, 2);    // hundreds
     fnd_display((current_door_state == open) ? 0x86 : 0xC0, 3);    // tens
     fnd_display((current_door_state == open) ? 0xC6 : 0x92, 4);    // ones
 }


 void fnd_display(uint8_t fnd, uint8_t location){

 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);	// latch clock pin off

 	uint16_t data = 0;
 	data = fnd << 8;
 	data |= 1 << (location-1);

 	// for example, to display 1 to first FND
 	// MSB 1111 1001 dddd 0001 LSB	(d means don't care)

 	for(int i=0 ; i<16; i++){

 		if(data >> (15-i) & 1)
 		{	// MSB first in
 			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);	// Data line
 		}
 		else
 		{
 			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 0);	// Data line
 		}

 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 1);	// shift clock pin on
 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, 0);	// shift clock pin off
 	}

 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);	// latch clock pin on
 }
