/*
 * ds1302.c
 *
 *  Created on: Mar 28, 2025
 *      Author: microsoft
 */

#include <string.h>
#include <stdio.h>
#include "extern.h"
#include "ds1302.h"
#include "button.h"

t_ds1302 ds1302;
t_ds1302 ds1302_alarm;

void set_RTC(char *date_time);
void init_alarm_clock(void);
void init_gpio_ds1302();
void init_ds1302();
void ds1302_clock(int *current_mode);
void ds1302_set_alarm(int *current_mode);
void display_alarm_clock(void);
void display_date_time(void);
void display_alarm_clock_on(void);
void display_alarm_wake_up(void);
void alarm_operate(int *current_mode);

void set_RTC(char *date_time)
{
	if(strncmp(date_time, "setrtc", 6) == 0)
	{
		ds1302.year = ((int)date_time[6] - 48)*10 + ((int)date_time[7] - 48);
		ds1302.month = ((int)date_time[8] - 48)*10 + ((int)date_time[9] - 48);
		ds1302.date = ((int)date_time[10] - 48)*10 + ((int)date_time[11] - 48);
		ds1302.hours = ((int)date_time[12] - 48)*10 + ((int)date_time[13] - 48);
		ds1302.minutes = ((int)date_time[14] - 48)*10 + ((int)date_time[15] - 48);
		ds1302.seconds = ((int)date_time[16] - 48)*10 + ((int)date_time[17] - 48);

		//front = (front + 1) % COMMAND_NUMBER;
		init_ds1302();
	}
}

void init_alarm_clock(void)
{
	ds1302_alarm.hours = 0;
	ds1302_alarm.minutes = 0;
	ds1302_alarm.seconds = 0;
}

void alarm_operate(int *current_mode)
{
	int lcd_toggle=1;

	if(get_button(GPIOC, GPIO_PIN_1, BTN1) == BUTTON_PRESS)
	{
		*current_mode = CLOCK_MODE;
		init_alarm_clock();
	}
	read_time_ds1302();
	read_date_ds1302();
	pc_command_processing();

	if(TIM11_1ms_counter3>1000)
	{
		TIM11_1ms_counter3=0;
		lcd_toggle = !lcd_toggle;
	}

	if(lcd_toggle)
	{
		display_alarm_wake_up();
	}
	else
	{
		i2c_lcd_init();
		HAL_Delay(500);
	}
}


void ds1302_set_alarm(int *current_mode)
{
	if(get_button(GPIOC, GPIO_PIN_0, BTN0) == BUTTON_PRESS)
	{
		*current_mode = CLOCK_MODE;
		return;
	}
	if(TIM11_1ms_counter2>10000)
	{
		flash_erase();
		flash_write((uint32_t *)&ds1302, sizeof(ds1302));
		TIM11_1ms_counter2=0;
	}
	read_time_ds1302();
	read_date_ds1302();
	pc_command_processing();

	display_alarm_clock();

	if(get_button(GPIOC, GPIO_PIN_1, BTN1) == BUTTON_PRESS)
	{
		if(ds1302_alarm.hours>=23)
		{
			ds1302_alarm.hours=0;
		}
		else
		{
			ds1302_alarm.hours++;
		}
	}

	if(get_button(GPIOC, GPIO_PIN_2, BTN2) == BUTTON_PRESS)
	{
		if(ds1302_alarm.minutes>=59)
		{
			ds1302_alarm.minutes=0;
		}
		else
		{
			ds1302_alarm.minutes++;
		}
	}

	if(get_button(GPIOC, GPIO_PIN_3, BTN3) == BUTTON_PRESS)
	{
		if(ds1302_alarm.seconds>=59)
		{
			ds1302_alarm.seconds=0;
		}
		else
		{
			ds1302_alarm.seconds++;
		}
	}
}

void ds1302_clock(int *current_mode)
{
	if(ds1302.hours==ds1302_alarm.hours && ds1302.minutes == ds1302_alarm.minutes && ds1302.seconds == ds1302_alarm.seconds)
	{
		*current_mode = ALARM_OPERATE_MODE;
		return;
	}

	if(TIM11_1ms_counter2>10000)
	{
		flash_erase();
		flash_write((uint32_t *)&ds1302, sizeof(ds1302));
		TIM11_1ms_counter2=0;
	}
	read_time_ds1302();
	read_date_ds1302();
	pc_command_processing();

	if(ds1302_alarm.hours ==0 && ds1302_alarm.minutes == 0 && ds1302_alarm.seconds ==0)
	{
		display_date_time();
	}
	else
	{
		display_alarm_clock_on();
	}
}

void display_alarm_wake_up(void)
{
	char icd_buff[40];

	sprintf(icd_buff,"   WAKE-UP!!   ");
	move_cursor(0,0);
	lcd_string(icd_buff);

	sprintf(icd_buff,"Time: %2d:%2d:%2d",ds1302.hours,ds1302.minutes,ds1302.seconds);
	move_cursor(1,0);
	lcd_string(icd_buff);
}


void display_alarm_clock_on(void)
{
	char icd_buff[40];

	sprintf(icd_buff,"ALM: %2d:%2d:%2d on",ds1302_alarm.hours,ds1302_alarm.minutes,ds1302_alarm.seconds);
	move_cursor(0,0);
	lcd_string(icd_buff);

	sprintf(icd_buff,"Time: %2d:%2d:%2d",ds1302.hours,ds1302.minutes,ds1302.seconds);
	move_cursor(1,0);
	lcd_string(icd_buff);
}

void display_alarm_clock(void)
{
	char icd_buff[40];

	sprintf(icd_buff,"ALM: %2d:%2d:%2d   ",ds1302_alarm.hours,ds1302_alarm.minutes,ds1302_alarm.seconds);
	move_cursor(0,0);
	lcd_string(icd_buff);

	sprintf(icd_buff,"Time: %2d:%2d:%2d",ds1302.hours,ds1302.minutes,ds1302.seconds);
	move_cursor(1,0);
	lcd_string(icd_buff);
}

void display_date_time(void)
{
	char icd_buff[40];

	sprintf(icd_buff,"Date: %2d-%2d-%2d",ds1302.year+2000,ds1302.month,ds1302.date);
	move_cursor(0,0);
	lcd_string(icd_buff);

	sprintf(icd_buff,"Time: %2d:%2d:%2d",ds1302.hours,ds1302.minutes,ds1302.seconds);
	move_cursor(1,0);
	lcd_string(icd_buff);
}

void read_time_ds1302()
{
	ds1302.seconds = read_ds1302(ADDR_SECONDS);
	ds1302.minutes = read_ds1302(ADDR_MINUTES);
	ds1302.hours = read_ds1302(ADDR_HOURS);
}

void read_date_ds1302()
{
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	ds1302.year = read_ds1302(ADDR_YEAR);
}

uint8_t read_ds1302(uint8_t addr)
{
	unsigned char data8bits = 0;	// 1bit씩 넘어온 것을 담을 그릇(변수)
	// 1. CE high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	// 2. addr 전송
	tx_ds1302(addr+1);				// read address
	// 3. data를 읽는다.
	rx_ds1302(&data8bits);			// 싱글 변수 따라서 &로 주소 불러오기
	// 4. CE low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	// 5. return (bcd to dec)
	return bcd2dec (data8bits);
}
// 1. 입력: bcd
// ex) 25년의 bcd --> dec
//     7654 3210
//     0010 0101
//       2    5
//     x10   x1
//  ===============
//        25
uint8_t bcd2dec(uint8_t bcd)
{
	uint8_t high, low;

	low = bcd & 0x0f;
	high = (bcd >> 4) * 10;	// (0010 0101)bcd >> 4 ==> 0000 0010 x10

	return (high+low);
}
//dec-->bcd
// ex) 25
//    dec		   bcd
// 0001 1001	0010 0101
uint8_t dec2bcd(uint8_t dec)
{
	uint8_t high, low;

	high = (dec/10) << 4;
	low = dec %10;

	return (high+low);
}

void rx_ds1302(unsigned char *pdata)
{
	unsigned char temp=0;
	// IO 포트를 입력 모드로 전환
	input_dataline_ds1302();
	// DS1302로 부터 들어온 bit를 LSB부터 8bit를 받아서 temp변수에 저장
	for (int i=0; i<8; i++)
	{
		// 1bit를 읽는다.
		if (HAL_GPIO_ReadPin(GPIOA, IO_DS1302_Pin))
		{
			// 1의 조건만 set
			temp |= 1 << i;
		}
		if(i != 7) clock_ds1302();	// 바로 읽기 때문에 clock을 한다. 대신 7일때 X
	}
	*pdata = temp;	// pdata는 data8bits의 주소가 저장
					// 따라서 *pdata를 하여 data8bits의 주소로 가서 값을 바꾼다
}


void init_ds1302()
{
	write_ds1302(ADDR_SECONDS, ds1302.seconds);
	write_ds1302(ADDR_MINUTES, ds1302.minutes);
	write_ds1302(ADDR_HOURS, ds1302.hours);
	write_ds1302(ADDR_DATE, ds1302.date);
	write_ds1302(ADDR_MONTH, ds1302.month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302.dayofweek);
	write_ds1302(ADDR_YEAR, ds1302.year);
}

void write_ds1302(uint8_t addr, uint8_t data)
{
	// 1. CE low ----> high
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	// 2. addr 전송
	tx_ds1302(addr);
	// 3. data 전송
	tx_ds1302(dec2bcd(data));
	// 4. CE high ---> low
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
}

void tx_ds1302(uint8_t value)
{
	output_dataline_ds1302();
	// ex) addr 초를 write하는
	// 80h M       L
	//     1000 0000	실제값	(B0를 전송시)
	//     0000 0001 &
	//	   0000 0000 ==> HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	//     1000 0000	실제값	(B7를 전송시)
	//     1000 0000  &		<< 7
	//	   1000 0000 ==> HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 1);
	for (int i=0; i<8; i++)
	{
		if (value & (1 << i))
		{
			HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 0);
		}
		clock_ds1302();
	}
}

void output_dataline_ds1302()
{
	GPIO_InitTypeDef GPIO_init = {0,};	// {0,0,0,0,0}랑 같은 뜻 구조체 다 초기화

	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;	// output mode
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH; // LOW: 2M HIGH: 25~100MHz
	HAL_GPIO_Init(GPIOA, &GPIO_init);		// 싱글변수이기 때문에 &로 주소를 넘김
}

void input_dataline_ds1302()
{
	GPIO_InitTypeDef GPIO_init = {0,};	// {0,0,0,0,0}랑 같은 뜻 구조체 다 초기화

	GPIO_init.Pin = IO_DS1302_Pin;
	GPIO_init.Mode = GPIO_MODE_INPUT;	// input mode
	GPIO_init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_init);		// 싱글변수이기 때문에 &로 주소를 넘김
}


void clock_ds1302()
{
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}

void init_gpio_ds1302()
{
	HAL_GPIO_WritePin(GPIOA, CE_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, IO_DS1302_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, CLK_DS1302_Pin, 0);
}

void init_date_time()
{
	ds1302.year=25;
	ds1302.month=3;
	ds1302.date=28;			// .~ 직접 접근
	ds1302.dayofweek=6;		// Fri
	ds1302.hours=11;
	ds1302.minutes=30;
	ds1302.seconds=00;
}


