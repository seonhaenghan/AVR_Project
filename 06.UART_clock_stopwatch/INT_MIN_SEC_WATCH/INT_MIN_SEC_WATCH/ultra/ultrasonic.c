/*
 * ultrasonic.c
 *
 * Created: 2022-08-10 오후 4:21:53
 *  Author: kccistc
 */ 


#include "ultrasonic.h"

void ultrasonic_trigger();
void ultrasonic_trigger();

volatile int ultrasonic_distance; 
char scm[50]; 

//PE4 : external interrupt 4 초음파 센서 상승, 하강에지 둘다 이쪽으로 들어온다. 
ISR(INT4_vect)
{
	if(ECHO_PIN & (1<<ECHO)) // 상승 에지 
	{
		TCNT1 = 0;
	}	
	else					// 하강 에지
	{
		// 에코핀의 펄스 길이를 us 단위로 환산 
		ultrasonic_distance = 1000000.0 * TCNT1 * 1024 / F_CPU;
		// 1cm : 58us 소요 
		sprintf(scm, "distance : %dcm", ultrasonic_distance/ 58); // cm로 환산 
	}
}
//PG4 : trigger
//PE4 : ECHO (INT4) External Interrupt 4
void init_ultrasonic()
{
	TRIG_DDR |= (1<< TRIG);		// 출력으로
	ECHO_DDR &= ~(1 << ECHO);	// 입력으로 사용 
	
	// 0 1 : 어떠한 형태로든지 신호 변화가 발생이 되면 Interrupt 요청 (Rising, Falling edge 둘다 )
	EICRB |= (0 << ISC41) | (1 << ISC40); 
	TCCR1B |=(1<<CS12) | (1 << CS10);	// 1024분주 
	// 16비트 timmer 1번을 1024 분주 해서 공급 
	// 16MHz를 1024분주 한다. >> 15625 ==> 15.625kHz
	// 1펄스의 길이 : t = 1/f = 1/15625 = 0.000064 > 64us  
	
	EIMSK |= (1 << INT4); // 외부 INT4(ECHO) 사용 
	
}
void ultrasonic_trigger()
{
	TRIG_PORT &= ~(1 << TRIG); // low
	_delay_ms(1);
	TRIG_PORT |= (1 << TRIG); // high
	_delay_ms(15);
	TRIG_PORT &= ~(1 << TRIG); // low 
}
