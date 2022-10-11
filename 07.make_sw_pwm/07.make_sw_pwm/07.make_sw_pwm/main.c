/*
 * 07.make_sw_pwm.c
 *
 * Created: 2022-08-11 오전 11:47:10
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음


int main(void)
{
	int  dim = 0;	   // LED 밝기 조절
	int direction = 1; // 밝기 증가 (1) 감소 (-1)
	
	DDRA = 0xff;	// PORTA OUTPUT MODE
	PORTA = 0xff;   // led all on
					// d >> b  b >> d 
	// us 단위로 led switching 을 해야 빛이 유지되는 것 처럼 보임 
    while (1) 
    {
		led_on_pwm(dim); 
		dim += direction; // 밝기 조절
		if(dim == 0) direction = 1; 
		if(dim == 255 ) direction = -1;  			
    }
}

void led_on_pwm(dim)
{ 
	PORTA = 0xff; 
	
	for (int i = 0; i < 256; i++)	// 	
	{
		if(i > dim)
			PORTA = 0x00;			// duty cycle 이 넘어 가면 led off 
		_delay_us(20); 
		
	} 
}