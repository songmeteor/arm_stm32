#include "main.h"

#define ADDR_SECONDS 0x80   // write addr만 define 하기로 하자
							//read addr은 write addr에 +1만 하면 되니까 ...
#define ADDR_MINUTES 0x81


#define ADDR_MINUTES 0x82
#define ADDR_HOURS 0x84
#define ADDR_DATES 0x86
#define ADDR_MONTH 0x88
#define ADDR_DAYOFWEEK 0x8a
#define YEAR 0x8c
#define ADDR_WRITEPROTECTED 0x8e

typedef struct s_ds1302
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t dayofweek; //1 : sum 2 : mon
	uint8_t year;
	uint8_t ampm;   // 1 : pm 0 : am
	uint8_t hourmode; // 0: 24  1 : 12
} t_ds1302;

t_ds1302 ds1302;
