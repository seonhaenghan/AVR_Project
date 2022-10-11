/*
* FND_COUNTER.c
* 초 Counter를 작성한다.
* Created: 2022-08-05 오전 11:46:35
* Author : 603
*/
#define F_CPU 16000000UL // Unsigned Long
#include <avr/io.h>
#include <util/delay.h>
#include "button.h"

// FND PORT 설정
#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

#define FND_DIGHT_DDR DDRB
#define FND_DIGHT_PORT PORTB
#define FND_DIGHT_D1	4
#define FND_DIGHT_D2	5
#define FND_DIGHT_D3	6
#define FND_DIGHT_D4	7

void init_fnd(void);
void display_fnd(void);
void box_s1(void);
void pase(void);

extern int get_BUTTON1();
extern int get_BUTTON2();

uint32_t ms_count = 0; // ms를 측정하는 변수
uint32_t sec_count = 0; // sec를 측정하는 변수
uint32_t digit_position = 0; // 출력할 자리 수

uint32_t ms_countb = 0; // ms를 측정하는 변수
uint32_t sec_countb = 0; // sec를 측정하는 변수
uint32_t digit_position_box = 0; // 출력할 자리 수

uint32_t ms_countp = 0; // ms를 측정하는 변수
uint32_t sec_countp = 0; // sec를 측정하는 변수
uint32_t digit_position_pase = 0; // 출력할 자리 수

int main(void)
{
	uint32_t button_s = 0;
init_fnd();	 // 초기화를 잘해야만 정확하게 동작한다.
	while (1)
	{
	display_fnd();
	_delay_ms(1);
	ms_count++;
	if(ms_count > 1000) // 1초
	{
		ms_count = 0; //reset
		sec_count++; // sec count 증가
	}	
			if(get_BUTTON1())
			{
				while(1)
				{
					box_s1();
					_delay_ms(1); // 1ms마다 켬
					ms_countb++;
					if(ms_countb > 80) // 1초
					{
						ms_countb = 0; //reset
						sec_countb++; // sec count 증가 //
						if(sec_countb > 25)
						{
							sec_countb = 0;
						}
					}
					if(get_BUTTON1()) break;
				}
			}
			if(get_BUTTON2())
			{
				while(1)
				{
					pase();
					_delay_ms(1);
						
					if(get_BUTTON2())
					{
						sec_count = 0;
						break;
					}
				}
			}				
		}
}

void display_fnd(void)
{
	unsigned char fnd_font[] = {0xc0, 0xf9, 0xa4,0xb0, 0x99,0x92, 0x82, 0xd8, 0x80, 0x98};// 0123456789 fnd에 나타나는 문구
// 
switch(digit_position)
{
	case 0: // 1 단위
	
	FND_DIGHT_PORT = 0b10000000;
	if(sec_count%2 == 0 )// 짝수에만 dot display
	{
		FND_DATA_PORT = fnd_font[sec_count%10] + 0b10000000; // 0~9
	}
	FND_DATA_PORT = fnd_font[sec_count%10];
	break;

	case 1: // 10 단위
	FND_DIGHT_PORT = 0b01000000;
	FND_DATA_PORT = fnd_font[sec_count/10%6]; // 초 10 단위
	break;

	case 2: // 100 단위
	FND_DIGHT_PORT = 0b00100000;
	FND_DATA_PORT = fnd_font[sec_count/60%10]; // 분 1자리
	break;

	case 3: // 1000 단위
	FND_DIGHT_PORT = 0b00010000;
	FND_DATA_PORT = fnd_font[sec_count/600%6]; // 분 10 자리
	break;
}
digit_position++; // 다음 표시할 자리 수 // 잔상효과
digit_position %= 4; //

}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;   // 출력모드 사용 
	FND_DIGHT_DDR |= 0xf0; // 7564만 1로 나머지 3210은 그대로 유지
	FND_DATA_PORT = ~0x00; // fnd all off // Com cathode 의 경우 1이 input되어야 led on >> 0x00
}

void box_s1(void)
{
		unsigned char box_font[] = {~0x01, ~0x02, ~0x04, ~0x08, ~0x10, ~0x20};// 0123456789 fnd에 나타나는 문구
		//
		switch(digit_position_box)
		{
			case 0: 
			if(sec_countb%25 >= 0 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[0];
			}
			break;
			
			case 1: 
			if(sec_countb%25 >= 1 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00100000;
				FND_DATA_PORT = box_font[0];
			}
			
			break;

			case 2:
			if(sec_countb%25 >= 2 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b01000000;
				FND_DATA_PORT = box_font[0];
			}
			
			break;

			case 3: // 1000 단위			
			if(sec_countb%25 >= 3 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[0];
			}
			break;
			
			case 4: // 1 단위		
			if(sec_countb%25 >= 4 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[1];
			}			
			break;

			case 5: 
			if(sec_countb%25 >= 5 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[2];
			}			
			break;

			case 6: // 100 단위	
			if(sec_countb%25 >= 6 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[3];
			}			
			break;
			
			case 7: 
			if(sec_countb%25 >= 7 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b01000000;
				FND_DATA_PORT = box_font[3];
			}			
			break;
			
			case 8: // 1 단위		
			if(sec_countb%25 >= 8 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00100000;
				FND_DATA_PORT = box_font[3];
			}			
			break;

			case 9: 		
			if(sec_countb%25 >= 9 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[3];
			}
			break;

			case 10: 			
			if(sec_countb%25 >= 10 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[4];
			}
			
			break;

			case 11: 		
			if(sec_countb%25 >= 11 && sec_countb < 12)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[5];
			}
			
			break;
			
			case 12: // 1 단위
			
			
			if(sec_countb%25 >= 12)
			{
				FND_DIGHT_PORT = 0b11110000;
				FND_DATA_PORT = ~0x00;
			}
			
			break;

			case 13: // 10 단위
			
			
			if(sec_countb%25 >= 13)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[5];
			}
			
			break;

			case 14: // 100 단위
			
			
			if(sec_countb%25 >= 14)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[4];
			}
			
			break;

			case 15: // 1000 단위
			
			
			if(sec_countb%25 >= 15)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[3];
			}
			
			break;
			
			case 16: // 1 단위
			
			
			if(sec_countb%25 >= 16)
			{
				FND_DIGHT_PORT = 0b00100000;
				FND_DATA_PORT = box_font[3];
			}
			
			break;

			case 17: // 10 단위
			
			
			if(sec_countb%25 >= 17)
			{
				FND_DIGHT_PORT = 0b01000000;
				FND_DATA_PORT = box_font[3];
			}
			
			break;

			case 18: // 100 단위
			
			
			if(sec_countb%25 >= 18)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[3];
			}
			
			break;

			case 19: // 1000 단위
			
			
			if(sec_countb%25 >= 19)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[2];
			}
			
			break;
			
			case 20: // 1 단위
			
			
			if(sec_countb%25 >= 20)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[1];
			}
			
			break;

			case 21: // 10 단위
			
			
			if(sec_countb%25 >= 21)
			{
				FND_DIGHT_PORT = 0b10000000;
				FND_DATA_PORT = box_font[0];
			}
			
			
			break;

			case 22: // 100 단위
			
			if(sec_countb%25 >= 22)
			{
				FND_DIGHT_PORT = 0b01000000;
				FND_DATA_PORT = box_font[0];
			}
			
			break;

			case 23: // 1000 단위
			
			
			if(sec_countb%25 >= 23)
			{
				FND_DIGHT_PORT = 0b00100000;
				FND_DATA_PORT = box_font[0];
			}
			
			break;
			
			case 24: // 1000 단위
			
			
			if(sec_countb%25 >= 24)
			{
				FND_DIGHT_PORT = 0b00010000;
				FND_DATA_PORT = box_font[0];
			}
			
			break;
			
			
		}
		digit_position_box++; // 다음 표시할 자리 수 // 잔상효과
		digit_position_box %= 25; //
}
void pase(void)
{
		unsigned char pase_font[] = {~0x73, ~0x77, ~0x6d, ~0x79};// 0123456789 fnd에 나타나는 문구
		//
		switch(digit_position_pase)
		{
			case 0 :
			 FND_DIGHT_PORT = 0b00010000;
			 FND_DATA_PORT = pase_font[0];
			break;
			
			case 1 :
			FND_DIGHT_PORT = 0b00100000;
			FND_DATA_PORT = pase_font[1];
			break;
			
			case 2 :
			FND_DIGHT_PORT = 0b01000000;
			FND_DATA_PORT = pase_font[2];
			break;
			
			case 3 :
			FND_DIGHT_PORT = 0b10000000;
			FND_DATA_PORT = pase_font[3];
			break;
			
		}
		digit_position_pase++; // 다음 표시할 자리 수 // 잔상효과
		digit_position_pase %= 4; //

}


