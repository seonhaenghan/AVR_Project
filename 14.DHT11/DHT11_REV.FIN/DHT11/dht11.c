/*
 * dht11.c
 *
 * Created: 2022-08-27 오후 1:47:31
 *  Author: seonhaeng han
 */ 


#include "dht11.h"
uint8_t rx_data_array[5];
void dht11_process()
{
	uint8_t i_RH, d_RH, i_Tmp, d_Tmp, chksum, chksum_compare;
	
	if(sec_count) // 2000ms cycle
	{
		sec_count = 0; 
		
		init_trigger_pulse();
		
		data_line_input();
		DHT11_dumi_read();
		
		i_RH = DHT11_rx_data();
		d_RH = DHT11_rx_data();
		i_Tmp = DHT11_rx_data();
		d_Tmp = DHT11_rx_data();
		
		chksum_compare = i_RH + d_RH + i_Tmp + d_Tmp;	// chksum 비교를 위한 변수 
		
		chksum = DHT11_rx_data();
		
		data_line_output(); 
		
		if(chksum == chksum_compare)			// checksum을 비교해 data가 정상인지 확인 
		{
			printf("[Tmp]%d.%d\n",(int)i_Tmp,(int)d_Tmp);
			printf("[Wet]%d.%d\n",(int)i_RH,(int)d_RH);
			printf("[chk]%d\n",(int)chksum);
		}
		else
		{
			printf("[data error]\n");			// chksum 비교 시 맞지 않으면 error sign
		}
	}
}

void init_trigger_pulse()
{
	
	DHT11_PORT &= ~(1 << DHT11); // low
	_delay_ms(20); // > 18ms
	DHT11_PORT |= (1 << DHT11); // high
	_delay_us(7);
	
}

void data_line_input()
{
	 DHT11_DDR &= ~(1 << DHT11); // input mode 
	 DHT11_PORT &= ~(1 << DHT11);// pull down 
}

void data_line_output()
{
	DHT11_DDR |= (1 << DHT11); // output 
	DHT11_PORT |= (1 << DHT11); // high (normal state = high)
}

uint8_t DHT11_rx_data()
{
	uint8_t rx_data = 0;
		
		for(int i = 0; i < 8; i++)
		{
			//when Input Data == 0
			while( 0 == DHT11_PIN & (1 << DHT11) ); // data 가 0인 것을 구분해야 함
				
			_delay_us(40);  // high pulse 가 40us 이상일 때 1
							// 따로 50us 만큼 delay를 줄 필요는 없음 
			rx_data <<=1;
				
			//when Input Data == 1
			if(DHT11_PIN & (1 << DHT11))// data 가 1인 것을 구분해야 함
				
			{
				rx_data |= 1;
			}
			while( 1 == DHT11_PIN & (1 << DHT11) );
		}
		
	return rx_data;
}

void DHT11_dumi_read(void)
{
	
	while( 1 == DHT11_PIN & (1 << DHT11) );
	while( 0 == DHT11_PIN & (1 << DHT11) );
	while( 1 == DHT11_PIN & (1 << DHT11) );
	
}

void DHT11_init()
{
	DHT11_DDR |= (1 << DHT11); // output
	DHT11_PORT |= (1 << DHT11); // high (normal state = high)
	_delay_ms(3000);
}