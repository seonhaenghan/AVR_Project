/*
 * led.c
 *
 * Created: 2022-08-09 오후 2:39:40
 *  Author: kcci
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

int led_command=0;  
 
void init_led()
{
	DDRA = 0xff; 
}

int toggle=0;


void ledalltoggle()
{
	toggle = !toggle;
	if (toggle)
		PORTA = 0xff;
	else PORTA = 0x00;
	//_delay_ms(300);
}

