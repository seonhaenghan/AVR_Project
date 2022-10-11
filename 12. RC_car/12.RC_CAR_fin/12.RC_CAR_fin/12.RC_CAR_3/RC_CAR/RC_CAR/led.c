/*
 * led.c
 *
 * Created: 2022-08-09 오후 2:39:42
 *  Author: kccistc
 */ 
#include "led.h"

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

uint32_t led_command = 0; 
extern uint32_t funcyc;
extern uint32_t del;


void init_led()
{
	AUTO_RUN_LED_PORT_DDR |= (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); 
}

void led_right()
{
	AUTO_RUN_LED_PORT |= (1 << 0);
	AUTO_RUN_LED_PORT &= ~(1 << 1);	
}

void led_left()
{
	AUTO_RUN_LED_PORT |= (1 << 1);
	AUTO_RUN_LED_PORT &= ~(1 << 0);	
}

void led_forward()
{
	AUTO_RUN_LED_PORT |= (1 << 1) | (1 << 0);
	AUTO_RUN_LED_PORT &= ~((1 << 2) | (1 << 3));
}

void led_stop()
{
	AUTO_RUN_LED_PORT |= (1 << 2) | (1 << 3);
	AUTO_RUN_LED_PORT &= ~((1 << 1) | (1 << 0));
}