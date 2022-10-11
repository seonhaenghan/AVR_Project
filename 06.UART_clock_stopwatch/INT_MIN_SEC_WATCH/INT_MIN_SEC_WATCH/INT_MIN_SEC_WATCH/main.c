/*
* FND_COUNTER.c
* 초 Counter를 작성한다.
* Created: 2022-08-05 오전 11:46:35
* Author : kccistc
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음 

#include "fnd.h"
#include "time_clock.h"
#include "stop_watch.h"

extern void bt_command_processing();
extern void init_UART1();
extern void UART0_transmit(uint8_t data);
extern int stopwatch_state; // stop_satch.c 에 들어 있다.
// 외부에 있는 파일에 있는 변수를 참조할 때는 extern로 선언한다. (externally)
extern TIME time_clock;
extern void pc_command_processing();
extern int get_BUTTON1();
extern int get_BUTTON2();
extern int get_BUTTON3();
extern void inc_stopwatch_clock(void);
extern void init_BUTTON();
extern void init_uart0();
extern void stopwatch_stop(); 



extern char scm[50];
extern int stopwatch_hour;
extern int stopwatch_min;
extern int stopwatch_sec;


// 1. FOR printf
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE); 



uint32_t ms_count=0;		// ms를 측정
uint32_t sec_count=0;
uint32_t ms_count2=0;		// sec를 측정 
uint32_t del = 0;
uint32_t funcyc = 0; 
volatile int ms_clock_display = 0; 
volatile int ms_stop_display = 0; 
volatile int ultrasonic_trigger_timer = 0; // 1000ms 마다 trigger 펄스  

void init_timer0();

volatile int count12 = 0;
volatile int state = 0;
volatile int state1 = 0;


// portb.0 : 500ms on/off
// portb.1 : 300ms on/off
// 1. 분주 : 64분주  ==>  16000000 / 64 = 250000Hz
// 2. T 계산 ==>  1/f = 1/250000 = 0.000004sec (4us)
// 3. 8 bit Timer OV : 4us * 256 = 1.024ms
// 256개의 pulse 를 count 하면 이곳으로 온다
//

ISR(TIMER0_OVF_vect) // 인터럽트 루틴을 길게 짤 수록 output delay가 증가하여 원하는 시간에 출력이 나오지 않음 
{
	TCNT0 = 6; // TCNT를 6~256 == > 정확히 1ms 를 유지하기 위해 
			   // TINT 0 OVF INT 6
	ms_count++;
	ms_count2++; 
	ms_clock_display ++; // LCD에 분초 시계 Display 하기 위한 interval 조절 Count
	ms_stop_display ++; 
	ultrasonic_trigger_timer++; 
	del = 300; // delay ms 변수 300ms
	if(ms_count2 % del == 0) 
	{
		
		funcyc++;
	}
	
	if (ms_count >= 1000)   // 1000ms ==> 1sec
	{
		ms_count=0;
		sec_count++;    // sec counter 증가
		inc_time_sec(); // 1초에 한번 씩 call 
	}
	if (ms_count%4 == 0)  //4ms 마다 fnd를 display
		display_fnd();
		
	if(stopwatch_state == RUN)
		inc_stopwatch_clock();
		
	if(get_BUTTON3())			// timer reset 
	{
		time_clock.sec = 0;
		time_clock.min = 0;
		time_clock.hour = 0;
		
		PORTF &= ~0x06;
	}
	
	
	count12++;
	if(count12 >=500)// 0.512sec : 512ms 오버플로우 500회 이상일 때 조건문 실행
	{
		count12 = 0;			// counter initial
		state = !state;		// reverse led state
		if(state)
		{
			PORTF |= 0x01;	// LED ON 해당하는 값만 ON시키기 위해 OR연산자 사용
		}
		else
		{
			PORTF &= ~0x01;	// LED OFF 해당하는 PIN만 OFF시키기 위해 AND연산자 사용
		}
		
	}
	
	if(get_BUTTON1())
	{
		
		state1 = !state1;		// reverse led state
		if(state1)
		{
			PORTF |= 0x04;
			PORTF &= ~0x02;	// LED ON 해당하는 값만 ON시키기 위해 OR연산자 사용
		}
		else
		{
			PORTF |= 0x02;
			PORTF &= ~0x04;	// LED OFF 해당하는 PIN만 OFF시키기 위해 AND연산자 사용
		}		
	}
	
	
	
		
}
//watch의 state를 선언 
#define WATCH 0
#define STOPWATCH 1
int mode = WATCH;
 


int main(void)
{
	TIME myTIME; // 시계를 구동 
	char sbuf[40];
	char sbuf2[40];
	
	
	init_led(); 
	init_fnd();
	init_BUTTON();
	init_uart0(); // UART0를 초기화 한다. 
	init_UART1(); // UART1 initial 
	I2C_LCD_init();  // I2C LCD Initial 
	stdout = &OUTPUT; // for printf /fprintf(stdout, "test"); ==> printf stdin : 입력 
	init_timer0();
	init_ultrasonic();
	sei();			 // global interrupt 활성화 
	
	
	
	mode = WATCH; // 시계모드로 시작 
	stopwatch_state = STOP; 
	
	while (1)
	{

		switch(mode)
		{
			case WATCH : 
			get_time_clock(&myTIME);
			set_fnd_data(myTIME.min*100 + myTIME.sec);
			if(get_BUTTON1())
			{
				mode = STOPWATCH; 
			}
			break;
			
			case STOPWATCH :
			switch (stopwatch_state)
			{
				case STOP :
				stopwatch_stop(); 
				break;
				case RUN :
				stopwatch_run();
				break;
				case RESET :
				
				stopwatch_reset();
				break;

			}
			if(get_BUTTON1())
			{
				
				mode = WATCH;
			}
			break;
		}
		
		pc_command_processing(); // pc에서 받는 command를 처리 fnuc 
		bt_command_processing(); // bluetooth 에서 받는 data 처리 func
		ultrasonic_distance_check();
		ultra_led();
		
		if(ms_clock_display > 1000)  // 1초
		{
			ms_clock_display = 0; // reset
			I2C_LCD_write_string_XY(0, 0, "                ");
			sprintf(sbuf, "clock : %2d:%2d:%2d",myTIME.hour, myTIME.min, myTIME.sec);	// 정수를 ASCII로 변환할 때 사용함
			// \n은 사용할 수 없음 none display char임 
			I2C_LCD_write_string_XY(0, 0, sbuf);							//  1초에 한 번씩 실행하므로 프로세스에 무리가 가지 않는다.
			
			
			 
		} 
		if(ms_stop_display > 500)
		{
			ms_stop_display = 0;
			
			I2C_LCD_write_string_XY(1, 0, "                ");
			sprintf(sbuf2, "stop : %2d:%2d:%2d",stopwatch_hour, stopwatch_min, stopwatch_sec);	// 정수를 ASCII로 변환할 때 사용함
			// \n은 사용할 수 없음 none display char임
			I2C_LCD_write_string_XY(1, 0, sbuf2);
		}
			
	
		
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

void ultrasonic_distance_check()
{
	if(ultrasonic_trigger_timer > 1000) 
	{
		ultrasonic_trigger_timer = 0; 
		printf("%s\n", scm); 
		ultrasonic_trigger();
		
	}
	
}

