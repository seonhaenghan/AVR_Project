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

extern void init_BUTTON(); 
extern int get_BUTTON1();
extern int get_BUTTON2();
extern int get_BUTTON3();
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
/*
16bit 3번 timer/counter를 사용 하겠다. 
 PWM출력 신호
 ===========
 PE.3 : OC3A
 PE.4 : OC3B
 PE.5 : OC3C
 BTN1 : start/stop
 BTN2 : Speed-up(OCR3C : 20씩 증가 max 250)
 BTN3 : Speed-down(OCR3C : 20씩 감소 min 50)
 */

void init_pwm()
{
	DDRE |= (1<<3 ) | (1<<4) | (1<<5); 
	// 모드 5 : 8비트 고속 PWM timer3
	TCCR3A |= (1<< WGM30); 
	TCCR3B |= (1<< WGM32); 
	// 비반전 모드 top : 0xff 비교일치값 OCR3C
	TCCR3A |= (1<< COM3C1); // P344 표 15-4
	// 분주비 64: 16000000/64 ==> 250000Hz
	// 256/ 250000hz = 1.02ms
	// 127 / 250000 => 0.5ms 
	TCCR3B |= (1 << CS31) | (1 << CS30); // 분주비 64 
	OCR3C = 0; 
	
}
void init_led()
{
	DDRF = 0x01;
	PORTF = 0x00; 
	
	DDRA = 0xff;
	PORTA = 0x00; 
}

int main(void)
{
	int fan_mode = 0; // start : 1/ stop : 0 
	
	 
	init_pwm(); 
	init_BUTTON();
	init_led();
	
	int i = 8;
	
    while (1) 
    {
		if(get_BUTTON1()) // start/ stop
		{
			
			if(!fan_mode)		// if(fan_mode == 0)
			{
				OCR3C = 70; // 모터 회전 기본값 
				PORTF |= 0x01; 
				fan_mode = 1; // stop 상태에서 회전 모드로 변환 
				
			}
			else // 
			{
				OCR3C = 0;	// 모터 정지 duty = 0 
				PORTF &= ~0x01; 
				PORTA = 0x00;
				fan_mode = 0; // 정지 모드 
			}
		}
		else if(get_BUTTON2()) // Speed up
		{
			
			if(OCR3C >= 230)
			{
				i = 0;
				OCR3C = 230;
				
			}
			else
			{
				i--;
				if(i >= 0 && i < 9)
				{
					PORTA = 0xff << i;
					
				}
				OCR3C += 20; 	
			}
		}
		else if(get_BUTTON3()) // speed down 
		{
			
			if(OCR3C <= 70)
			{
				i = 9;
				OCR3C = 70;
				
			}
			else
			{
				i++;
				if(i > 0 && i < 9)
				{
					PORTA = 0xff << i;
					
				}
				OCR3C -= 20; 
			}
		}
    }
}

