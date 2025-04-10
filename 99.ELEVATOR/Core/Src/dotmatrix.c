#include "main.h"
#include "elevator.h"
#include <string.h>

extern enum CURRENT_FLOOR current_floor;
extern enum CURRENT_STATE current_state;
extern enum CURRENT_DOOR_STATE current_door_state;

void dotmatrix_main_test();
int dotmatrix_main(void);
int dotmatrix_main_func(void);
void dotmatrix_elevator(void);
void init_dotmatrix(uint8_t matrix[]);

uint8_t allon[] = {			// allon 문자 정의
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
};

uint8_t smile[] = {			// 스마일 문자 정의
	0b00111100,
	0b01000010,
	0b10010101,
	0b10100001,
	0b10100001,
	0b10010101,
	0b01000010,
	0b00111100 };

uint8_t hart[] = {		// hart
	0b00000000,    // hart
	0b01100110,
	0b11111111,
	0b11111111,
	0b11111111,
	0b01111110,
	0b00111100,
	0b00011000
};


uint8_t one[][8] =
{
	{0b00000000,
	0b00001000,
	0b00011000,
	0b00101000,
	0b00001000,
	0b00001000,
	0b01111110,
	0b00000000}
};

uint8_t two[][8] =
{
	{0b00000000,
	0b00011100,
	0b00100010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00111110,
	0b00000000}
};

uint8_t three[][8] =
{
	{0b00000000,
	0b00111100,
	0b00000100,
	0b00001000,
	0b00011000,
	0b00000100,
	0b00111100,
	0b00000000}
};

uint8_t four[][8] =
{
	{0b00000000,
	0b00011000,
	0b00101000,
	0b01001000,
	0b01111110,
	0b00001000,
	0b00001000,
	0b00000000}
};

uint8_t my_name[] =
{0B01111010,
0B00001010,
0B00001010,
0B11111010,
0B00100010,
0B10101110,
0B10000010,
0B11111110};

uint8_t col[4]={0,0,0,0};

unsigned char display_data[8];  // 최종 8x8 출력할 데이터
unsigned char scroll_buffer[8] = {0};  // 스코롤할 데이타가 들어있는 버퍼
int number_of_character = 1;  // 출력할 문자 갯수

uint32_t past_time=0;  // 이전 tick값 저장

void init_dotmatrix(uint8_t matrix[])
{
	memset(scroll_buffer, 0, sizeof(scroll_buffer));
	memset(display_data, 0, sizeof(display_data));

	for (int i=0; i < 8; i++)
	{
		scroll_buffer[i] = matrix[i];
	}

}

void dotmatrix_elevator(void)
{
	uint8_t temp;
	uint32_t now = HAL_GetTick();
    if (now - past_time >= 100)
    {
        past_time = now;

        if(current_state == up)
        {
        	for (int i = 0; i < 7; i++)
            {
                display_data[i] = scroll_buffer[i + 1];
            }
            display_data[7] = scroll_buffer[0];

            int temp = scroll_buffer[0];

            for(int i=0; i<8; i++)
            {
            	if(i==7)
            	{
            		scroll_buffer[7] = temp;
            		break;
            	}
            	scroll_buffer[i] = scroll_buffer[i+1];
            }

        }
        else if(current_state == down)
        {
        	for (int i = 7; i > 0; i--)
            {
                display_data[i] = scroll_buffer[i - 1];
            }
            display_data[0] = scroll_buffer[7];

            int temp = scroll_buffer[7];

            for(int i=7; i>=0; i--)
            {
            	if(i==0)
            	{
            		scroll_buffer[0] = temp;
            		break;
            	}
            	scroll_buffer[i] = scroll_buffer[i-1];
            }
        }
        else
        {
            switch(current_floor){
            case first:init_dotmatrix(one);
            	break;
            case second:init_dotmatrix(two);
            	break;
            case third:init_dotmatrix(three);
            	break;
            case fourth:init_dotmatrix(four);
            	break;
            }
        	for (int i = 0; i < 8; i++)
            {
                display_data[i] = scroll_buffer[i];
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        col[0] = ~(1 << i);  // 00000001  --> 11111110
        col[1] = display_data[i];

        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                temp = col[j];
                HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, (temp & (1 << k)) ? 1 : 0);
                HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1); // clk 상승
                HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0); // clk 하강
            }
        }
        GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
        GPIOB->ODR |= GPIO_PIN_13;    // latch핀을 pull-up
        HAL_Delay(1);
    }
}

void dotmatrix_main_test()
{
  uint8_t temp;
  static int i = 0;

#if 1
	col[0] = ~(1 << i);  // 00000001  --> 11111110
	col[1] = smile[i];
	//HAL_SPI_Transmit(&hspi2, col, 2, 1);
	for (int j=0; j < 2; j++)
	{
		for(int k=0; k<8; k++)
		{
			temp = col[j];
			if(temp & (1 << k))
			{
				HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 1);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 0);
			}
			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1); //clk을 상승에서
			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0); // 하강으로
		}
	}
	GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
	GPIOB->ODR |= GPIO_PIN_13;   // latch핀을 pull-up

	i++;
	i %= 8;
#endif

#if 0

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_15, 0);
  HAL_Delay(10);

  while (1)
  {
        for (int i=0; i < 8; i++)
        {
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = hart[i];
			//HAL_SPI_Transmit(&hspi2, col, 2, 1);
			for (int j=0; j < 2; j++)
			{
				for(int k=0; k<8; k++)
				{
					temp = col[j];
					if(temp & (1 << k))
					{
						HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 1);
					}
					else
					{
						HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 0);
					}
					HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1); //clk을 상승에서
					HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0); // 하강으로
				}
			}
			GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
			GPIOB->ODR |= GPIO_PIN_13;   // latch핀을 pull-up
			HAL_Delay(1);
        }
  }
#endif
}

