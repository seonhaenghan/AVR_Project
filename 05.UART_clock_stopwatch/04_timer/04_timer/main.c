/*
 * 04_timer.c
 *
 * Created: 2022-08-08 오전 11:26:48
 * Author : kccistc
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int count = 0;
volatile int state = 0;

volatile int count2 = 0;
volatile int state2 = 0;
// PORTB.0 : 500ms on/off
// PORTB.2 : 300ms on/off
// 분주 : 64분주 ==> 16MHz / 64 = 250kHz 
// 8bit timer 0V : 0.004ms * 256 => 255를 초과하는 값에서 interrupt발생 
//				   0.004ms * 250 => 1ms 

//256개의 Pulse 를 count 를 하면 이곳으로 온다. 
// 1ms 마다 interrupt가 이 곳으로 온다. 
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;	// 6부터 시작한다. 정확한 시간을 맞춰주기 위함 
	count++;
	if(count >=500)// 0.512sec : 512ms 오버플로우 500회 이상일 때 조건문 실행
	{
		count = 0;			// counter initial
		state = !state;		// reverse led state
		if(state)
		{
			PORTB |= 0x01;	// LED ON 해당하는 값만 ON시키기 위해 OR연산자 사용 
		}
		else
		{
			PORTB &= ~0x01;	// LED OFF 해당하는 PIN만 OFF시키기 위해 AND연산자 사용 
		}
		
	}
	
	count2++;
	if(count2 >=300)// 0.3072sec : 3072ms 오버플로우 300회 이상일 때 조건문 실행 
	{
		count2 = 0;			// counter initial
		state2 = !state2;		// reverse led state
		if(state2)
		{					
			PORTB |= 0x04;	// LED ON	
		}
		else
		{						
			PORTB &= ~0x04;	// LED OFF 
		}
		
	}
		
	
	
}

int main(void)
{
    DDRB = 0x01 + 0x04; // 0x05
	PORTB = 0x00;
	
	TCCR0 |= (1<< CS02) | (0<< CS01)| (0<< CS00); // 분주기 64로 사용 
	// 16000000/64 = 250000Hz // T = 4us // 256 * 4u = 0.001024sec 
	TIMSK |= (1<< TOIE0);
	// 오버플로 인터럽트 활성화 (Timer/counter0 Overflow Interrupt Enable)
	sei();
	
    while (1) 
    {
    }
	return 0;
}

