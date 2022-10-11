/*
 * button.h
 *
 * Created: 2022-08-08 오후 2:17:55
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL // Unsigned Long
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_DDR DDRC
#define BUTTON_PIN PINC // >> PORTD를 read하는 Register
// PINx는 하나의 메모리 AVR에서 제공하는 register
// 전압 5V를 읽으면 1, OV를 읽으면 0
// 해당 자리의 Input에 대한 기억
#define BUTTON1		7	// PORTD.4

int get_BUTTON1();


void init_BUTTON();


#endif /* BUTTON_H_ */