/*
 * ds1302.h
 *
 *  Created on: Mar 28, 2025
 *      Author: microsoft
 */

#include "main.h"	// HAL_GPIO

#define ADDR_SECONDS		0x80	// write addr만 define
									// read addr은 write addr에 +1
#define ADDR_MINUTES		0x82
#define ADDR_HOURS			0x84
#define ADDR_DATE			0x86
#define ADDR_MONTH			0x88
#define ADDR_DAYOFWEEK		0x8A
#define ADDR_YEAR			0x8C
#define ADDR_WRITEPROTECTED	0x8E

#define GPIOA_ODR 0x40020014
#define GPIOA_IDR 0x40020010
#define GPIOA_MODER 0x40020000
#define GPIOA_PUPDR 0x4002000C
#define GPIOA_OSPEEDR 0x40020008

void ds1302_main();
void read_time_ds1302();
void read_date_ds1302();
uint8_t read_ds1302(uint8_t addr);
uint8_t bcd2dec(uint8_t bcd);
uint8_t dec2bcd(uint8_t dec);
void rx_ds1302(unsigned char *pdata);
void init_ds1302();
void write_ds1302(uint8_t addr, uint8_t data);
void tx_ds1302(uint8_t value);
void output_dataline_ds1302();
void input_dataline_ds1302();
void clock_ds1302();
void init_gpio_ds1302();
void init_date_time();

typedef struct s_ds1302
{
	uint8_t seconds;		// sec
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t dayofweek;		// 1: sun 2:mon
	uint8_t year;
	uint8_t ampm;			// 1: pm  2: am
	uint8_t hoursmode;		// 0: 24  1: 12
} t_ds1302;


