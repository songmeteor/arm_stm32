
#include "ds1302.h"

void ds1302_main(void)
{
	init_date_time();
}

void init_date_time(void)
{
	ds1302.year = 25;
	ds1302.month = 3;
	ds1302.date = 28;
}


