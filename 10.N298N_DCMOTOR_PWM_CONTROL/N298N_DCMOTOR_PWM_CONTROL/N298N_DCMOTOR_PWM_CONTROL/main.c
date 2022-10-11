/*
 * N298N_DCMOTOR_PWM_CONTROL.c
 *
 * Created: 2022-08-12 오전 10:23:01
 * Author : kccistc
 */ 


#if 1  // orginal 
// ENA : PORTE.5 (OCR3B(PE5) <=== PORTE.4 (Dc motor driver pin)
// IN1 : PORTE.0 (정회전/ 역회전)
// IN2 : PORTE.2 (속도)
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile int ms_cnt = 0;
volatile int s_cnt = 0;
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	ms_cnt++;
	if(ms_cnt >= 1000)
	{
		s_cnt++; 
	}
}
void pwm_init()
{
	DDRE |= ( 1 << 0) | ( 1 << 2) | ( 1 << 5);  // PE.0 PE.2 PE5 출력 모드로 설정
	// 모드5 : 8비트고속PWM모드 timer 3
	TCCR3A |= (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	// 비반전 모드 TOP: 0xff 비교일치값 : OCR3C : PE5
	TCCR3A |= (1 << COM3C1);
	// 분주비 64  16000000/64  ==> 250000HZ(250KHZ)
	// 256 / 250000 ==> 1.02ms
	// 127 / 250000 ==> 0.5ms
	TCCR3B |= ( 1 << CS31)  | (1 << CS30);   // 분주비 64
	// 1600000HZ / 64분주 ==> 250000HZ (250KHZ)
	// 
	OCR3C=0;  // PORTE.5 
	TCNT3 = 6; 
}
void init_timer0()
{
	// 파형 출력 핀 PB.4 (OC0)을 출력으로 설정
	DDRE |= ( 1 << 0) | ( 1 << 2) | ( 1 << 5);  // PE.0 PE.2 PE5 출력 모드로 설정
	//  타이머/카운터 0번을 고속 PWM 모드로 설정
	TCCR0 |= (1<< WGM01) | (1 << WGM00);
	// 비반전 mode 설정 / 비교 일치 발생 시 OCn 핀의 출력은 LOW 값으로 바뀌고, BOTTOM에서 HIGH 값으로 바뀐다 / (1,0)
	TCCR0 |= (1<< COM01); // P344 표 15-4
	// 분주비 1024 설정 16000000/1024(분주비) = 15625Hz
	// t = 1/15625 = 64usec
	// 8bit Timer/count / 64u * 256 = 16.4ms 마다 Timer0 Overflow interrupt가 발생
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0); // Timer overflow interrupt enable No.0 활성
	TCNT0 = 6; // Timer count initial
}
// ENA : PORTE.5 (OCR3B(PE5) <=== PORTE.4
// IN1 : PORTE.0
// IN2 : PORTE.2

int main(void)
{
	
	init_timer0();
	pwm_init();
	
	// 1) 모터 정지(2초) --> 2) 모터를 정회전(중간속도) 2초 --> 3)모터 정지(2초) --> 4) 모터역회전(중간속도)
	// 5) 모터역회전(최대속도) : 2초간 
    while (1) 
    {
		int atoggle = 0; 
		if(atoggle)
		{
			atoggle = !atoggle;
		}
		
		
		
		// 정회전 : PE2:0 | PE0 1
		PORTE &= 0b11111011;  // 모터정지 : PE0:1 | PE2:0
		PORTE |= 0b00000001;
		OCR3C=127;    // 0.5ms 중간속도   
		_delay_ms(2000);  // 2초간 유지 	

		OCR3C=255;      // max speed 
		_delay_ms(2000);  // 2초간 유지	
		
		OCR3C=0;
		PORTE |= 0b00000101;  // 모터정지 : PE0:1 PE2:1
		_delay_ms(2000);  // 2초간 유지
		
		// 역회전 : PE2:1 PE0 0
		PORTE &= 0b11111110;   
		PORTE |= 0b00000100;	
		
		OCR3C=127;    // 0.5ms 중간속도
		_delay_ms(2000);  // 2초간 유지

		OCR3C=255;      // max speed
		_delay_ms(2000);  // 2초간 유지		
			 		
    }
}
void init_timer0()
{
	TCNT0 = 6;
	
	DDRF = 0x01 + 0x02 + 0x04;
	
	PORTF |= 0x02;
	
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);  //  분주비를 64로 설정
	
	TIMSK |= (1 << TOIE0);
	

}
#endif 


