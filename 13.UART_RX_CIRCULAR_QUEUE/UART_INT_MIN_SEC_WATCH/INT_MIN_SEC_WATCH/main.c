/*
 * FND_COUNTER.c
 *  초 counter 를 작성 한다. 
 * Created: 2022-08-05 오전 11:46:29
 * Author : kcci
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <stdio.h>   // printf, scanf등이 정의 되어 있다. 
#include <string.h>  // strcpy, strncmp, strcat 등이 들어 있다. 

#include "fnd.h"
#include "time_clock.h"
#include "stopwatch.h"

extern void UART0_transmit(uint8_t data);
extern void init_UART1();

// 1. for printf 
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t ms_count=0;  // ms를 재는 변수
volatile uint32_t sec_count=0;  // sec를 재는 변수
volatile uint32_t led_ms_count = 0; 
volatile uint32_t led_sec_count = 0;

extern volatile uint8_t rx_ready_flag;   // 완전한 문장이 들어 왔을때 1로 set된다
extern int stopwatch_state;   // stopwatch.c에 들어 있다. 
                              // 다른 화일에 있는 변수를 참조 할떄는 extern 이라고 선언을 해준다. 
extern void pc_command_processing();  // extern : pc_command_processing은 다른 화일에 있다는것을 compiler에게 알려 준다
extern void init_led();
// PORTB.0 : 500ms  on/off
// PORTB.1 : 300ms on/off
// 1. 분주: 64분주 ==> 16000000/64 ==> 250,000HZ
// 2. T 계산 ==> 1/f = 1/250000 = > 0.000004 sec (4us) : 0.004ms
// 3. 8 bit Timer OV: 0.004ms x 256 ==> 0.00124sec ==> 약 1.24ms
//                    0.004ms x 250 ==> 1ms
// 256개의 pulse를 count를 하면 이곳으로 온다.
// 1ms 마다 ISR(TIMER0_OVF_vect) 이곳으로 들어 온다
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	
	ms_count++;
	led_ms_count++;
	if (ms_count >= 1000)   // 1000ms ==> 1sec
	{
		ms_count=0;
		sec_count++;    // sec counter증가
		
	}
	if(led_ms_count >= 300)
	{
		led_sec_count++;
	}
}

#define WATCH 0
#define STOPWATCH 1

int mode = WATCH;

int main(void)
{ 
	TIME myTime;
	
	init_led();
	init_fnd();
	init_button();
	init_uart0();		// uart0 를 초기화 
	init_UART1();       // uart1 를 초기화 
	stdout = &OUTPUT;   // 2. for printf fprintf(stdout,"test"); ==> printf stdin: 입력 (scanf(....); 
	init_timer0();
	sei();				// 전역적으로 인터럽트 허용	
	
	mode = WATCH; // stopwatch 
	stopwatch_state = STOP;
	
	
    while (1) 
    {
		pc_command_processing();
		bt_command_processing();
    }
}

void init_timer0()
{
	TCNT0=6;   // TCNT 6~256 ==> 정확히 1ms 마다 TIMT 0 OVF INT 
	// 분주비를 64로 설정 P269 표13-1
	TCCR0 |= (1 << CS02) | (0 << CS01) | (0 << CS00);

	TIMSK |= (1 << TOIE0);			// 오버플로 인터럽트 허용

}

