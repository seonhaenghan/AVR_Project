/*
 * DHT11.c
 *
 * Created: 2022-08-27 오후 12:23:00
 * Author : seonhaeng han
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음


extern void init_uart0();
extern void UART0_transmit(uint8_t data);
extern void pc_command_processing(); 

extern void dht11_process();
extern void init_trigger_pulse();
extern void data_line_input();
extern void data_line_output();
extern uint8_t rx_data(int anum);
extern void DHT11_init();

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile int ms_count = 0; 
volatile int sec_count = 0; 

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; 
	
	ms_count++; 
	if(ms_count >= 2000)
	{
		ms_count = 0; 
		sec_count++; 
	}
	
}



int main(void)
{
   init_uart0();
   init_timer0();
   DHT11_init();
   sei();
   stdout = &OUTPUT;
    while (1) 
    {
		
			dht11_process();
		
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