

#include "main.h"

#define GPIOA_ODR 	  0x40020014
#define GPIOA_IDR 	  0x40020010
#define GPIOA_MODER   0x40020000
#define DHT_PIN_NUM   0

enum state_define {OK, TIMEOUT, VALUE_ERROR, TRANS_ERROR};

