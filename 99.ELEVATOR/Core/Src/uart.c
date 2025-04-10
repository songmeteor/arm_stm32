#include "uart.h"
#include "extern.h"
#include <stdio.h>
#include <string.h>

volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGHT];
volatile int rear = 0;
volatile int front = 0;

extern UART_HandleTypeDef huart2;

t_print o_prt;

void pc_command_processing(void);

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  *
  *  move from Drivers/STM32F4xx_HAL-driver/Src/stm32f4xx_hal_uart.c to here
  *  예) comportmaster로 부터 1char를 수신하면 HAL_UART_RxCpltCallback으로 진입
  *  9600bps인 경우 HAL_UART_RxCpltCallback를 수행 수 1ms 이내에는 빠져 나가야 한다.
  *  115200bps									  86us
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	volatile static int i=0;

	if(huart == &huart2)
	{
		if (rx_data == '\n')
		{
			rx_buff[rear++][i] = '\0';
			rear %= COMMAND_NUMBER; // rear : 0~9
			i=0; //다음 string을 저장하기 위한 1차원 index값을 0으로
			// !!!! rx_buff queue full check 하는 logic 추가
		}
		else
		{
			rx_buff[rear][i++] = rx_data;
			//COMMAND LENGTH를 check 하는 logic 추가
		}
		 HAL_UART_Receive_IT(&huart2, &rx_data, 1); //주의 반드시 집어 넣어야 다음 INT가 발생
	}
}

void show_command(void)
{
	char *cmd[] =
	{
		"setrtc",
		"print_rtc",
		"printoff_rtc",
		"help"
	};

	for(int i=0; i<4; i++)
	{
		printf("%s\n",cmd[i]);
	}
}

void pc_command_processing(void)
{
	if(front != rear) //rx_buff에 data가 존재
	{
		//printf("%s\n",(const char *)rx_buff[front]); //rx_buff[front][0]
		if(strncmp((const char *)rx_buff[front], "led_all_on",strlen("led_all_on")) == 0)
		{
		   printf("find: led_all_on\n");
		}
		else if(strncmp((const char *)rx_buff[front], "setrtc",strlen("setrtc")) == 0)
		{
			set_RTC((char *)rx_buff[front]);
			//printf("%s\n",rx_buff[front]);
		}
		else if(strncmp((const char *)rx_buff[front], "help",strlen("help")) == 0)
		{
			show_command();
		}
		else if(strncmp((const char *)rx_buff[front], "print_rtc",strlen("print_rtc")) == 0)
		{
			o_prt.p_rtc = 1;
			printf("print_rtc: %d\n",o_prt.p_rtc);
		}
		else if(strncmp((const char *)rx_buff[front], "printoff_rtc",strlen("printoff_rtc")) == 0)
		{
			o_prt.p_rtc = 0;
			printf("print_rtc: %d\n",o_prt.p_rtc);
		}
		front++;
		front %= COMMAND_NUMBER;
	}
}









