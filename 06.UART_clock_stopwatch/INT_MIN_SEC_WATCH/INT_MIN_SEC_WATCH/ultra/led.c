/*
 * led.c
 *
 * Created: 2022-08-09 오후 2:39:42
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

uint32_t led_command = 0; 
// 0 : nothing
// 1 : ledalltoggle 
// 2 : shift_left_led_on
// 3 : shift_right_led_on
// 4 : flower L
extern uint32_t funcyc;
extern uint32_t del;
extern volatile int ultrasonic_distance;
extern uint32_t ms_count2;
void init_led()
{
	DDRA = 0xff; // data direction register
}


void ledalltoggle(void)
{
	
	if(funcyc)
	{
		PORTA = 0xff;
	}
	else 
	{
		PORTA = 0x00;
	}
}

void shiftleftledon(void)
{
	int i ;
	
		for(i = 0 ; i < 8 ; )
		{	
			if(funcyc)
			{
				
			PORTA = 0x01 << i;
			
			i++;		// 이쪽으로 빼지 않으면 funcyc가 1이 아닐 때도 for문이 실행된다. 
			
			funcyc = 0;
			}	 
	}
}

void shiftrightledon(void)
{
	int i ;
	
	
		for(i = 0 ; i < 8 ; )
		{
			if(funcyc)
			{
			PORTA = 0x80 >> i;
			
			
			i++;
			funcyc = 0; 
			}
		}
	
}

void shiftleftholdledon(void)
{
	int i ;
	
	
		for(i = 7 ; i >=0 ; )
		{
			if(funcyc)
			{
			PORTA = 0xff >> i;
			
			i--;
			
			funcyc = 0; 
			}
		}
}

void shiftrightholdledon(void)
{
	int i ;
	
		for(i = 7 ; i >=0 ; )
		{
			if(funcyc)
			{
			PORTA = 0xff << i;
			
			i--;
			
			funcyc = 0; 
			}
		}
}

void f_off(void)
{
	int h,l;
	
	
		for(int i = 0; i < 4; )
		{
			if(funcyc)
			{
			h = 0x80 >> i;
			l = 0x01 << i;
			PORTA = h | l ;
			
			i++; 
			
			funcyc = 0;
			} 
		}
}

void flower_on(void)
{
	int h2,l2;
	
	
		for(int i = 0; i < 4;)
		{
			if(funcyc)
			{
			h2 = 0x10 << i;
			l2 = 0x08 >> i;
			PORTA = h2 | l2 ;
			
			i++;
			 
			funcyc = 0;
			}
		}
}

int toggle = 0; 
void ultra_led()
{
	 uint32_t ultrasonic_distance58= ultrasonic_distance/ 58;
	
	if(ultrasonic_distance58 == 2)
	{
			PORTA = 0xff;
	
	}
	else if(ultrasonic_distance58 == 3)
	{
	
			PORTA = ~0x80;	

	}
	else if(ultrasonic_distance58 == 4)
	{
		
		PORTA = ~0xc0;

	}
	else if(ultrasonic_distance58 == 5)
	{
		
		PORTA = ~0xe0;

	}
	else if(ultrasonic_distance58 == 6)
	{
		
		PORTA = ~0xf0;

	}
	else if(ultrasonic_distance58 == 7)
	{
		
		PORTA = ~0xf8;

	}
	else if(ultrasonic_distance58 == 8)
	{
		
		PORTA = ~0xfc;

	}
	else if(ultrasonic_distance58 == 9)
	{
		
		PORTA = ~0xfe;

	}
	else if(ultrasonic_distance58 >= 10)
	{
		if(ms_count2 >=500)
		{
			ms_count2 = 0;
			toggle  = !toggle
			if(toggle)
			{
				PORTA = 0xff;
				
			}
			else
			{
				PORTA = 0x00;
			}		
		}		
	}
	
}