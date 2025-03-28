#ifndef INC_BUTTON_H
#define INC_BUTTON_H

#include "main.h"

#define GPIOC_PUPDR 0x4002080C

#define BTN0  0 //PC0
#define BTN1  1 //PC1
#define BTN2  2 //PC2
#define BTN3  3 //PC3
#define BTN4  4 //PC13  demo borad

#define BUTTON_PRESS   0  // active low
#define BUTTON_RELEASE 1  // button을 땐 상태
#define BUTTON_NUMBER  5

#endif
