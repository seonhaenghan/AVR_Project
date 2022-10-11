/*
 * RC_CAR.c
 *
 * Created: 2022-08-18 오전 9:50:40
 * Author : kccistc
 */ 

/*
	1. LEFT MOTOR
		PORTF.0 : IN1
		PORTF.1 : IN2
	2. RIGHT MOTOR
		PORTF.2 : IN3
		PORTF.3 : IN4 
		IN1.IN3 IN2.IN4 
		======  ======
		   0       1    : 역회전
		   1       0    : 정회전
		   1       1    : STOP
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음



extern void bt_command_processing();
extern void init_UART1();
extern void UART0_transmit(uint8_t data);

extern int get_BUTTON1();
extern void init_BUTTON();
extern void init_uart0();
extern void init_led();

extern void init_pwm_motor(void);
extern void manual_mode_run(void);

extern void distance_check(void); 
extern void init_ultrasonic();
extern int button1_state;

extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);
extern void stop();
// 1. FOR printf
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

extern volatile int ultrasonic_left_distance;
extern volatile int ultrasonic_center_distance;
extern volatile int ultrasonic_right_distance;

extern volatile uint32_t motor_ms_count; 

void init_timer0();

volatile uint32_t ms_count=0;		// ms를 측정
volatile uint32_t ms_count50 = 0; 



int set_speed = 600; 
int obs_distance = 30; // 장애물과 센서 사이 거리  15cm 
int limit_distance = 15;
int max_distance = 100;  
int motion_state = 0;	

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
	// TINT 0 OVF INT
	ms_count++;
	if(ms_count >= 50)
	{
		ms_count = 0; 
		ms_count50++;
	}
	if(ms_count50 >= 3)
	{
		ms_count50 = 0; 
	}
}




int main(void)
{
	init_pwm_motor();
	init_led();
	init_BUTTON();
	init_uart0(); // UART0를 초기화 한다.
	init_UART1(); // UART1 initial
	init_timer0();
	init_ultrasonic();
	
	
	stdout = &OUTPUT; // for printf /fprintf(stdout, "test"); ==> printf stdin : 입력
	sei();			 // global interrupt 활성화
	
	while (1)
	{
		
		manual_mode_run();  // Bluetooth car command run 
		
		auto_mode_check();	// button1 check

		if(button1_state)
		{
			auto_drive(); 
			distance_check();	// 초음파 거리 측정  
		}		
	}
}

// 자율 주행 프로그램을 이곳에 진행 
void auto_drive()
{

	// 각 센서에 장애물이 detect 되었을 때 
	if(ultrasonic_left_distance <= obs_distance || ultrasonic_center_distance <= obs_distance || ultrasonic_right_distance <= obs_distance)
	{
		
		// 장애물이 하나만 있을 때 
			one_obstruction();
		// 장애물이 두개가 있을 때 
			two_obstruction();	
	}
	else if(ultrasonic_left_distance < obs_distance && ultrasonic_center_distance < obs_distance && ultrasonic_right_distance < obs_distance)
	{
		stop(); 
	}
	else
	{
		forward(set_speed); 
	}
} 


void one_obstruction()
{
	
	if(ultrasonic_left_distance <= limit_distance)		// 좌측 장애물
	{
		if(ultrasonic_right_distance > ultrasonic_center_distance)
		{
			turn_left(set_speed); // 90도 회전
		}
		else if(ultrasonic_right_distance < ultrasonic_center_distance)
		{
			forward(set_speed);
		}
	}
	 if(ultrasonic_center_distance <= obs_distance) // 중앙 장애물 
	{
		if(ultrasonic_left_distance > ultrasonic_right_distance)
		{
			turn_left(set_speed); // 90도 회전
		}
		else if(ultrasonic_left_distance < ultrasonic_right_distance)
		{
			turn_right(set_speed);	// 90도 회전
		}
		
	}
	
	if(ultrasonic_right_distance <= limit_distance)		// 좌측 장애물
	{
		if(ultrasonic_right_distance > ultrasonic_center_distance)
		{
			turn_right(set_speed);
		}
		else if(ultrasonic_right_distance < ultrasonic_center_distance)
		{
			forward(set_speed);
		}
	}
	
	
		
}


void two_obstruction()
{
	if(ultrasonic_left_distance <= obs_distance && ultrasonic_center_distance <= obs_distance)
	{
		turn_right(set_speed);
	}
	else if(ultrasonic_left_distance <= obs_distance && ultrasonic_right_distance <= obs_distance)
	{
		forward(set_speed);
	}
	else if(ultrasonic_center_distance <= obs_distance && ultrasonic_right_distance <= obs_distance)
	{
		turn_left(set_speed); 
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



