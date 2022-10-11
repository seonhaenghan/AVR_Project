/*
 * led_shift.c
 *
 * Created: 2022-08-04 오전 11:30:29
 * Author : kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
//	[AVR] BIT Shifter LED left--right 순차 ON 동작
//  pin 76543210
//             1
//            1
//           1
//          1
//         1
//        1
//       1
//      1
//       1
//        1
//         1
//          1
//           1
//            1
//             1
void shift_light_left_on(void);	
void shift_light_right_on(void)	; // 함수 정의
					
int main(void)
{
	//unsigned char led[]= {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    DDRA = 0xff; // PORTA all high
    while (1) 
    {

		shift_light_left_on(); // 함수 사용
		shift_light_right_on();
    }
}
void shift_light_left_on(void) // 좌측 Shift 함수
{
	for(int i = 0; i < 8; i ++)
	{
		PORTA = 0x01 << i;
		_delay_ms(200);
	}
}

void shift_light_right_on(void) // 우측 Shift 함수
{
	for(int i = 0; i < 8; i++)
	{
		PORTA = 0x80 >> i;
		_delay_ms(200);
	}
	
}

