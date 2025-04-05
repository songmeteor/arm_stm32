#include <stdio.h>
#include <string.h>

#include "lcd1602_4bit.h"
#include "extern.h" // 펄스에서 delay_us 함수 사용하기 위해


void lcd_4bit_test(void)
{
	lcd_init();
	lcd_set_cursor(0,0);
	lcd_send_string("Hello World!");

	int cnt = 0;
	char buffer[16];
	while (1)
	{
		lcd_set_cursor(1, 0);
		sprintf(buffer, "cnt: %d", cnt);
		lcd_send_string(buffer);
		cnt++;
	    HAL_Delay(1000);
	}
}

void pulse_en()
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);
    delay_us(20);   //data 상에 Tpw = 140ns 이상 대기
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);
    delay_us(20);	//data 상에 Tpw = 140ns 이상 대기
}

void lcd_send_4bit(char data)
{
	//data 는 4bit
	// ex) 0b0011
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, ((data >> 3) & 0x01)); //0b00000100 & 1 => 0
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, ((data >> 2) & 0x01)); //0b00001000 & 1 => 0
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, ((data >> 1) & 0x01)); //0b00010001 & 1 => 1
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01));        //0b00100011 & 1 => 1
    pulse_en();
}

void lcd_send_8bit(char data) // 8bit data 4bit로 두번 나눠서 전송
{
	lcd_send_4bit((data >> 4) & 0x0F);  // 8bit data의 상위 4bit 전송
	lcd_send_4bit(data & 0x0F);         // 8bit data의 하위 4bit 전송
}

void lcd_clear_display(void)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0); // 설정 모드
	lcd_send_8bit(0b00000001);
	HAL_Delay(2);
}

void lcd_send_string(char *str)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 1); // 문자 전송 모드
	while (*str)
	{
		lcd_send_8bit(*str++);
	}
}

void lcd_set_cursor(int row, int col)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0); // 설정 모드
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_8bit(col); // Set DDRAM Address -> 2 line display 모드에서는
    					// RS Rw DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    					// 0   0  1  AC6 AC5 AC4 AC3 AC2 AC1 AC0
    					//  AC의 값이 0x00~0x27까지 1번째 line 주소, 0x40~0x67까지 2번쨰 line 주소
    					// 따라서 2번째 line(row=1)에 출력하고 싶으면 0011 0000 값인 0xC0을 or 연산해줘야됨
}


void lcd_init(void)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0); // RS = 0 이면 설정모드, RS = 1 이면 Write/Read 모드
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, 0); // RW = 0 이면 쓰기, RW = 1 이면 읽기

    HAL_Delay(50);         // 15ms 이상 대기

    lcd_send_4bit(SET_4BIT_MODE);
    HAL_Delay(5);          // 4.1ms 이상 대기
    lcd_send_4bit(SET_4BIT_MODE);
    HAL_Delay(1);          // 100us 이상 대기
    lcd_send_4bit(SET_4BIT_MODE);
    HAL_Delay(1);
    lcd_send_4bit(0b0010);  //4bit mode 설정
    HAL_Delay(1);

    lcd_send_8bit(0b00101000); // Function set -> 4번 bit DL=0, 3번 bit N=1, 2번 bit F=0 -> 4bit, 2줄, 5x7 도트
    HAL_Delay(1);              // 실행시간인 37ns 이상 대기
    lcd_send_8bit(0b00001000); // Display ON/OFF  -> 2번 bit D=0, 1번 bit C=0, 0번 bit B=0  -> 디스플레이 OFF, 커서 OFF, 블링크 OFF
    HAL_Delay(1);
    lcd_send_8bit(0b00000001); // Clear Display
    HAL_Delay(2);				// 실행시간인 1.52ms 이상 대기
    lcd_send_8bit(0b00000110); // Entry Mode Set -> I/D = 1, S = 0 -> 커서 오른쪽 이동
    HAL_Delay(1);

    lcd_send_8bit(0b00001100); // Display ON/OFF -> D=0, C=0, B=0  -> 디스플레이 ON, 커서 OFF, 블링크 OFF
    HAL_Delay(1);
}

