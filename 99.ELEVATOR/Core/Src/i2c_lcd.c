/*
 * i2c_lcd.c
 *
 *  Created on: 2019. 9. 4.
 *      Author: k
 */
#include "main.h"
#include "extern.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "i2c_lcd.h"

#define RESET 0
#define SET 1

extern I2C_HandleTypeDef hi2c1;

void i2c_lcd_init(void);
void lcd_string(uint8_t *str);
void move_cursor(uint8_t row, uint8_t column);
void i2c_lcd_dis_line0(void);
void i2c_lcd_dis_line1(void);

void i2c_ack();
void i2c_clk();
void i2c_start_sig();
void i2c_stop_sig();
void read_signal(uint8_t *i2c_buffer);
void read_address();
void I2C_Master_Transmit();

#define I2C_LCD_ADDRESS (0x27 << 1)
unsigned char lcd_test[4] = { '4','3', 0};


void lcd_command(uint8_t command){

	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = command & 0xf0;
	low_nibble = (command<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1

	//I2C_Master_Transmit(i2c_buffer);
	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
			i2c_buffer, 4, 100)!=HAL_OK){
		//HAL_Delay(1);
	}

	return;
}
void lcd_data(uint8_t data){

	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = data & 0xf0;
	low_nibble = (data<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
	//I2C_Master_Transmit(i2c_buffer);
	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
			i2c_buffer, 4, 100)!=HAL_OK){
		//HAL_Delay(1);
	}
	return;
}
void i2c_lcd_init(void){

	lcd_command(0x33);
	lcd_command(0x32);
	lcd_command(0x28);	//Function Set 4-bit mode
	lcd_command(DISPLAY_ON);
	lcd_command(0x06);	//Entry mode set
	lcd_command(CLEAR_DISPLAY);
	HAL_Delay(2);
}
void lcd_string(uint8_t *str){
	while(*str)lcd_data(*str++);
}
void move_cursor(uint8_t row, uint8_t column){
	lcd_command(0x80 | row<<6 | column);
	return;
}


// HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2c_buffer, 4, 100
// 일단 master에서 start(SDA-LOW, SCL-high), slave address 보내고, write bit, buffer 4번(1byte - 4번), 1byte 보낼때마다 ack, 이후 SDA-HIGH, SCL-high로 끝
// 이러면 handshaking 끝
// write address 0x4E 이미 선언
// PB9 I2C_SDA
// PB8 I2C_SCL

void I2C_Master_Transmit(uint8_t *i2c_buffer)   // 전제 i2c_protocol
{
   i2c_start_sig();
   read_address();
   read_signal(i2c_buffer);
   i2c_stop_sig();
}

void read_signal(uint8_t *i2c_buffer)
{
   for(int j = 0; j < 4; j++)
   {
      for(int i = 7; i >= 0; i--)   // 1.MSB ~ 8.LSB
      {
         // 하나씩 뺴서 읽기
         // 삼항 연산자(조건식) ? (true) : (false)
         int bit = (i2c_buffer[j]>>i & 0x01) ?  SET : RESET ; // 버퍼의 각 자리에 대한 변수를 선언
//         if (SET) bit = 1;      // bit를 지워도 되나?
//         else (RESET) bit = 0;
      } // 여기서 고민이 for문 하나 더 사용해서 ack를 더해주기
         i2c_clk();
   }
   i2c_ack();
}

void read_address()
{
   for(int i=7; i>=0; i--)   // 1.MSB ~ 8.LSB
   {
      // 하나씩 뺴서 읽기
      // 삼항 연산자(조건식) ? (true) : (false)
      int bit = (I2C_LCD_ADDRESS>>i & 0x01) ?  SET : RESET ; // 버퍼의 각 자리에 대한 변수를 선언
//      if (SET) bit = 1;      // bit를 지워도 되나?
//      else (RESET) bit = 0;
      i2c_clk();
   }
   i2c_ack();
}

void i2c_ack()   // data 보낸 직후 SDA가 LOW이면 ack   ack 신호는 CLK이 하강 엣지 일때
{
   *(unsigned int*)GPIOB_ODR |= 1<<8;      // SCL HIGH
   *(unsigned int*)GPIOB_ODR &= ~(1<<9);   // SDA LOW
   *(unsigned int*)GPIOB_ODR &= ~(1<<8);   // SCL LOW
   i2c_clk();
}

void i2c_clk()   // SCL이 상승엣지일때 SDA를 읽는다
{
   *(unsigned int*)GPIOB_ODR &= ~(1<<8);   // SCL LOW
   *(unsigned int*)GPIOB_ODR |= 1<<8;      // SCL HIGH
   HAL_Delay(1);
}

void i2c_start_sig()   // SCL-high, SDA-하강엣지
{

   *(unsigned int*)GPIOB_ODR |= 1<<8;      // SCL HIGH
   *(unsigned int*)GPIOB_ODR |= 1<<9;      // SDA HIGH
   *(unsigned int*)GPIOB_ODR &= ~(1<<9);   // SDA LOW 여기서 시작 신호 발생
   *(unsigned int*)GPIOB_ODR &= ~(1<<8);   // SCL LOW
   HAL_Delay(1);
}

void i2c_stop_sig()   // SCL-high, SDA-상승엣지
{

   *(unsigned int*)GPIOB_ODR |= 1<<8;      // SCL HIGH
   *(unsigned int*)GPIOB_ODR &= ~(1<<9);   // SDA LOW
   *(unsigned int*)GPIOB_ODR |= 1<<9;      // SDA HIGH
   *(unsigned int*)GPIOB_ODR &= ~(1<<8);   // SCL LOW
   HAL_Delay(1);
}








