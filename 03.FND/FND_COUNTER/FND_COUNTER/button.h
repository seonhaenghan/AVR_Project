/*
 * button.h
 *
 * Created: 2022-08-05 오후 2:42:24
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL // Unsigned Long
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRA
#define LED_PORT PORTA

#define BUTTON_DDR DDRD
#define BUTTON_PIN PIND // >> PORTD를 read하는 Register
// PINx는 하나의 메모리 AVR에서 제공하는 register
// 전압 5V를 읽으면 1, OV를 읽으면 0
// 해당 자리의 Input에 대한 기억
#define BUTTON1		4	// PORTD.4
#define BUTTON2		5	// PORTD.5
#define BUTTON3		6	// PORTD.6
#define BUTTON4		7	// PORTD.7




#endif /* BUTTON_H_ */