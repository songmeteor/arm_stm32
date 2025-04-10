#include "fnd.h"
#include "extern.h"
#include "elevator.h"

extern enum CURRENT_DOOR_STATE current_door_state;

 void fnd_elevator(void);
 void fnd_display(uint8_t fnd, uint8_t location);

 void fnd_elevator(void)
 {
//	    static int index = 0;
//	    static const uint8_t fnd_data[4] = {
//	        0xC0, // O
//	        0x8C, // P
//	        0x86, // E
//	        0xC6  // N
//	    };
//
//	    fnd_display(fnd_data[index], index + 1);
//	    index = (index + 1) % 4;

//     fnd_display(0xC0, 1);    // thousands
//     fnd_display(0x8C, 2);    // hundreds
//     fnd_display(0x86, 3);    // tens
//     fnd_display(0xC6, 4);    // ones

     fnd_display((current_door_state == open) ? 0x3F : 0x39, 1);    // thousands
     fnd_display((current_door_state == open) ? 0x73 : 0x38, 2);    // hundreds
     fnd_display((current_door_state == open) ? 0x79 : 0x3F, 3);    // tens
     fnd_display((current_door_state == open) ? 0x54 : 0x6D, 4);    // ones
 }


 void fnd_display(uint8_t fnd, uint8_t location)
 {
 	uint16_t data = 0;
 	data = fnd << 8;
 	data |= ~(1 << (location - 1)) & 0x0F;

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

 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
 }
