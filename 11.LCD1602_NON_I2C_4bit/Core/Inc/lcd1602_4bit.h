#ifndef __LCD1602_H
#define __LCD1602_H

#include "main.h"

#define SET_4BIT_MODE 0b0011  // DS7 DS6 DS5 DS4
							  //  0   0   1   1

void pulse_en();
void lcd_send_4bit(char data);
void lcd_send_8bit(char data);
void lcd_clear_display(void);
void lcd_send_string(char *str);
void lcd_set_cursor(int row, int col);
void lcd_init(void);

void lcd_4bit_test(void);

#endif  __LCD1602_H
