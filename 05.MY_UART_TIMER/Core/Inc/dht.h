

#include "main.h"

#define GPIOA_ODR 	  0x40020014
#define GPIOA_IDR 	  0x40020010
#define DHT_PIN_NUM   0

enum state_define {OK, TIMEOUT, VALUE_ERROR, TRANS_ERROR};
enum state_define dht11_state = OK;	// 앞으로 관리할 state변수 생성 (초기값 OK)
