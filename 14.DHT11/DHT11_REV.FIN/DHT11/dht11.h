/*
 * dht11.h
 *
 * Created: 2022-08-27 오후 1:47:55
 *  Author: seonhaeng han
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> // printf, scanf 등이 정의 되어 있다.
#include <string.h> // strcpy, strcat, strcmp 등이 들어 있음

#define TRUE 1
#define FALSE 0
#define DHT11_ERROR 255
#define DHT11_DDR	DDRC
#define DHT11_PORT	PORTC
#define DHT11_PIN	PINC
#define DHT11		0

void dht11_process();
void init_trigger_pulse();
void data_line_input();
void data_line_output();
uint8_t DHT11_rx_data();


extern volatile int sec_count;

#endif /* DHT11_H_ */