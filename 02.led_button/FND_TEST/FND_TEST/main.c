/*
 * FND_TEST.c
 *
 * Created: 2022-08-05 오전 11:20:44
 * Author : kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Common anode 방식
int main(void)
{
   DDRC = 0xff;	// 세그먼트 제어 핀 8개를 출력으로 설정 
   DDRB = 0xff; // 자릿수 선택핀 4개를 출력으로 설정 
   PORTB = 0xf0; // 0xf0
   PORTC = ~0xff; // 0xff  
   
   
    while (1) 
    {
    }
}

