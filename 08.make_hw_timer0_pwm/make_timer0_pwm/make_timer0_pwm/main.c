/*
 * make_timer0_pwm.c
 *
 * Created: 2022-08-11 오후 1:11:33
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음

// 16.4ms 마다 이곳으로 진입 
volatile int tim_16ms = 0; 
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0; // CTC mode가 아닌 경우 무조건 초기화 시켜주어야 한다.
	tim_16ms++; // int 마다 1씩 증가 
	
}
// OC0 : PWM 파형 출력 신호 PB.4 으로 출력을 설정 
void init_timer0_pwm(void)
{
	// 파형 출력 핀 PB.4 (OC0)을 출력으로 설정
	DDRB |= (1 << 4); // timer 0번 설정 
	//  타이머/카운터 0번을 고속 PWM 모드로 설정 
	TCCR0 |= (1<< WGM01) | (1 << WGM00); 
	// 비반전 mode 설정 / 비교 일치 발생 시 OCn 핀의 출력은 LOW 값으로 바뀌고, BOTTOM에서 HIGH 값으로 바뀐다 / (1,0)
	TCCR0 |= (1<< COM01); // P344 표 15-4 
	// 분주비 1024 설정 16000000/1024(분주비) = 15625Hz 
	// t = 1/15625 = 64usec 
	// 8bit Timer/count / 64u * 256 = 16.4ms 마다 Timer0 Overflow interrupt가 발생 
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0); // Timer overflow interrupt enable No.0 활성 
	TCNT0 = 0; // Timer count initial 
}
int main(void)
{
	int dim = 0; 
	int direction = 1;
	
	init_timer0_pwm();
	sei();
	
	OCR0 = dim; // overflow compare register 
    while (1) 
    {
		// duty cycle 설정 : 1클럭 64us 
		// 초기 OCR0 : 1(64us) 부터 OCR0 : 255(16.4ms) 까지 
		// 16.4ms단위로 서서히 밝아 지도록 한다. OCR0 : 0 ==> 255
		if(tim_16ms >= 1)
		{
			tim_16ms = 0; 
			OCR0 = dim; 
			dim += direction; // 밝기 변화 
		}
    }
}

