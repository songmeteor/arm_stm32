#include <dht.h>
#include <extern.h>

#include <string.h>
#include <stdio.h>

uint8_t us_count = 0;
enum state_define dht11_state = OK;

void init_dht11(void)
{
	*(unsigned int *)GPIOA_ODR |= 1<<DHT_PIN_NUM;
}


void dht11_main(void)
{
    uint8_t data[5] = {0,};  // 5bytes(humi(2)+temp(2)+checksum(1))

    init_dht11();

    while(1)
    {
        memset(data, 0, sizeof(data));
        dht11_state = OK;

        //========= step1 : start signal  ================
        init_dht11();
        delay_ms(100);

        DHT_PORT &= ~(1 << DHT_PIN_NUM); // DATA LOW
        delay_ms(20);

        DHT_PORT |= 1 << DHT_PIN_NUM; // DATA HIGH
        DHT_DDR &= ~(1 << DHT_PIN_NUM); // input mode
        _delay_us(1);

        // response check
        us_count = 0;
        while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM) // 아직도 high 인지 check
        {
            _delay_us(2);
            us_count += 2;
            if(us_count > 50) // 50us만큼 기다렸는데도 high면 error
            {
                dht11_state = TIMEOUT;
                break;
            }
        }

        //========= step2 : response signal check ================
        if(dht11_state == OK) // 정상 상태면 DATA pin이 LOW이다.
        {
            // response check
            us_count = 0;
            while(!((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)) // LOW 일동안 반복
            {
                _delay_us(2);
                us_count += 2;
                if(us_count > 100) // spec에는 80us인데 여유를 둬서 100us만큼 기다렸는데도 low 면 error
                {
                    dht11_state = TIMEOUT;
                    break;
                }
            }
        }

        //response HIGH check
        if(dht11_state == OK) // 정상 상태면 DATA pin이 LOW이다.
        {
            // response check
            us_count = 0;
            while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM) //  high 일동안 반복
            {
                _delay_us(2);
                us_count += 2;
                if(us_count > 100) // spec에는 80us인데 여유를 둬서 100us만큼 기다렸는데도 low 면 error
                {
                    dht11_state = TIMEOUT;
                    break;
                }
            }
        }

        //=============== step3 data bit 수신 from DHT11======================
        if(dht11_state == OK)
        {
            uint8_t pulse[8] = {0,}; //8개의 pulse를 담는 그릇(변수)

            for(int i = 0; i < 5; i++)  //5 bytes 이니까
            {
                for(int j = 7; j >= 0; j--) //byte당 8bit
                {
                    //LOW 확인 50us check
                    us_count = 0;
                    while(!((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)) // LOW 일동안 반복
                    {
                        _delay_us(2);
                        us_count += 2;
                        if(us_count > 70) // spec에는 50us인데 여유를 둬서 70us만큼 기다렸는데도 low 면 error
                        {
                            dht11_state = TIMEOUT;
                            i = 5;
                            j = -1; //for문 전체 탈출
                            break;
                        }
                    }

                    if(dht11_state == OK)
                    {
                        us_count = 0;
                        while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM) // HIGH 일동안 반복
                        {
                            _delay_us(2);
                            us_count += 2;
                            if(us_count > 90) // '0' : HIGH 길이 : 26~28us '1' : HIGH 길이 70us 인데 이것보다 길게 90us
                            {
                                dht11_state = TIMEOUT;
                                j = -1; //for문 전체 탈출
                                i = 5;
                                break;
                            }
                        }

                        if(dht11_state == OK)
                        {
                            if(us_count < 40) // '0'
                            {
                                pulse[j] = 0;
                            }
                            else if(us_count > 40) // '1'
                            {
                                pulse[j] = 1;
                            }
                        }
                    }
                }

                if(dht11_state == OK)
                {
                    data[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 |
                             pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
                }
            }

            //check-sum check
            if(dht11_state == OK)
            {
                if(data[4] != data[0] + data[1] + data[2] + data[3])
                {
                    dht11_state = VALUE_ERROR;
                }
            }
            _delay_us(60); // SPEC에는 50us인데 여유를 둬서 60us
        }

        //값 출력
        if(dht11_state == OK)
        {
            printf("temp : %d.%d\n", data[2], data[3]);
            printf("humi : %d.%d\n", data[0], data[1]);
        }
        if(dht11_state != OK)
        {
            printf("error code : %d\n", dht11_state);
        }
        _delay_ms(2000);
    }
}
