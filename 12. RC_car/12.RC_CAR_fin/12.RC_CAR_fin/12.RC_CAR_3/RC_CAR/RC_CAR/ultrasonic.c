/*
 * ultrasonic.c
 *
 * Created: 2022-08-10 오후 4:21:53
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

//좌측 초음파 센서 PE4 : external interrupt 4 초음파 센서 상승, 하강에지 둘다 이쪽으로 들어온다. 
// 초음파 : 1cm 를 이동하는데 29us 소요 1cm 왕복 : 29 * 2 = 58us 소요 
// 16000000Hz / 1024 == 15,625Hz
// T = 1/f ==> 0.000064sec (64us)

ISR(INT4_vect)
{
	if(ECHO_PIN_LEFT & (1<<ECHO_LEFT)) // 상승 에지 
	{
		TCNT3 = 0;						// 상승 일 때 Clear 
	}	
	else					// 하강 에지
	{
		// 에코핀의 펄스 길이를 us 단위로 환산 
		ultrasonic_left_distance = (1000000.0 * TCNT3 * 1024 / F_CPU)/ 58;
		// 1cm : 58us 소요 
	}
}
// 외부 INT 5번 : 중앙의 초음파 센서 
ISR(INT5_vect)
{
	if(ECHO_PIN_CENTER & (1<<ECHO_CENTER)) // 상승 에지
	{
		TCNT3 = 0;						// 상승 일 때 Clear
	}
	else					// 하강 에지
	{
		// 에코핀의 펄스 길이를 us 단위로 환산
		ultrasonic_center_distance = (1000000.0 * TCNT3 * 1024 / F_CPU)/ 58;
		// 1cm : 58us 소요
	}
}

ISR(INT6_vect)
{
	if(ECHO_PIN_RIGHT & (1 << ECHO_RIGHT)) // 상승 에지
	{
		TCNT3 = 0;						// 상승 일 때 Clear
	}
	else					// 하강 에지
	{
		// 에코핀의 펄스 길이를 us 단위로 환산
		ultrasonic_right_distance = (1000000.0 * TCNT3 * 1024 / F_CPU)/ 58;
		// 1cm : 58us 소요
	}
}

// 초음파 센서 초기화 함수
void init_ultrasonic()
{
	//==============================================LEFT Ultra Sonic==================================
	TRIG_DDR_LEFT |= (1<< TRIG_LEFT);		// 출력으로
	ECHO_DDR_LEFT &= ~(1 << ECHO_LEFT);	// 입력으로 사용 
	
	// EICRA : INT 0~3 
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호의 변화가 있든지 INT를 요청 한다. (상승, 하강 에지 둘다 INT 발생)
	EICRB |= (0 << ISC41) | (1 << ISC40); 
	TCCR3B |=(1<<CS32) | (1 << CS30);	// 1024분주 
	// 16비트 timmer 1번을 1024 분주 해서 공급 
	// 16MHz를 1024분주 한다. >> 15625 ==> 15.625kHz
	// 1펄스의 길이 : t = 1/f = 1/15625 = 0.000064 > 64us  
	
	EIMSK |= (1 << INT4); // 외부 INT4(ECHO) 사용
	
	//==============================================CENTER Ultra Sonic==================================
	TRIG_DDR_CENTER |= (1<< TRIG_CENTER);		// 출력으로
	ECHO_DDR_CENTER &= ~(1 << ECHO_CENTER);	// 입력으로 사용
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호의 변화가 있든지 INT를 요청 한다. (상승, 하강 에지 둘다 INT 발생)
	EICRB |= (0 << ISC51) | (1 << ISC50);
	TCCR3B |=(1<<CS32) | (1 << CS30);	// 1024분주
	// 16비트 timmer 1번을 1024 분주 해서 공급
	// 16MHz를 1024분주 한다. >> 15625 ==> 15.625kHz
	// 1펄스의 길이 : t = 1/f = 1/15625 = 0.000064 > 64us
	
	EIMSK |= (1 << INT5); // 외부 INT5(ECHO) 사용
	
	//==============================================RIGHT Ultra Sonic==================================
	TRIG_DDR_RIGHT |= (1<< TRIG_RIGHT);		// 출력으로
	ECHO_DDR_RIGHT &= ~(1 << ECHO_RIGHT);	// 입력으로 사용
	
	// EICRA : INT 0~3
	// EICRB : INT 4~7
	// 0 1 : 어떠한 신호의 변화가 있든지 INT를 요청 한다. (상승, 하강 에지 둘다 INT 발생)
	EICRB |= (0 << ISC61) | (1 << ISC60);
	TCCR3B |=(1<<CS32) | (1 << CS30);	// 1024분주
	// 16비트 timmer 1번을 1024 분주 해서 공급
	// 16MHz를 1024분주 한다. >> 15625 ==> 15.625kHz
	// 1펄스의 길이 : t = 1/f = 1/15625 = 0.000064 > 64us
	
	EIMSK |= (1 << INT6); // 외부 INT6(ECHO) 사용
}
void distance_check(void) // delay를 빼기 위한 함수 
{
	ultrasonic_trigger(); 
	//printf("L : %5dcm\n", ultrasonic_left_distance);
	//printf("C : %5dcm\n", ultrasonic_center_distance);
	//printf("R : %5dcm\n", ultrasonic_right_distance);
}

void ultrasonic_trigger()
{
	 
	switch(ms_count50)
	{
		case 0 : 
		//============================LEFT==============================
		TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT); // low
		_delay_us(1);
		TRIG_PORT_LEFT |= (1 << TRIG_LEFT); // high
		_delay_us(15);
		TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT); // low
		
		// 초음파 센서 Echo 응답 시간 최대 38ms
		// _delay_ms를 timer INT로 처리한다. Delay가 있어서는 안됨
		// 현재는 demo code 지만 timer 처리 예정
		break;
	
		case 1 : 
		//============================CENTER==============================
		TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER); // low
		_delay_us(1);
		TRIG_PORT_CENTER |= (1 << TRIG_CENTER); // high
		_delay_us(15);
		TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER); // low
		break;
		
		case 2 : 
		//============================RIGHT==============================
		TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT); // low
		_delay_us(1);
		TRIG_PORT_RIGHT |= (1 << TRIG_RIGHT); // high
		_delay_us(15);
		TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT); // low
		break;
		
		// ms 단위는 Process에서 큰 단위로 적용되어 timer를 사용해 delay를 주어야 함***
	}
	
	ms_count50 = ms_count50 % 3;
}