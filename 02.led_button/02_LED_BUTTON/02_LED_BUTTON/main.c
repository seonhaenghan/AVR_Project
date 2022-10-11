/*
 * 02_LED_BUTTON.c
 *
 * Created: 2022-08-04 오후 2:47:45
 * Author : kccistc
 */ 

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

int main(void)
{
    LED_DDR = 0xff; //DDRA = 0xff; LED 출력 모드 이기 때문에 1로 
	BUTTON_DDR &= ~((1 << BUTTON1) | (1 << BUTTON2) | (1 << BUTTON3) | (1 << BUTTON4)); // input mode : 0으로 설정 //initial  
	
    while (1) 
    {
		if((BUTTON_PIN & (1 << BUTTON1))) // == (BUTTON_PIN & (1 << BUTTON1) == 1)
		{
			LED_PORT = 0xff; // LED ON
		}
		if((BUTTON_PIN & (1 << BUTTON2)))
		{
			LED_PORT = 0x00; // LED OFF 
		}	
		if((BUTTON_PIN & (1 << BUTTON3)))
		{
			LED_PORT = 0xff; // LED ON
		}
		if((BUTTON_PIN & (1 << BUTTON4))) 
		{
			LED_PORT = 0x00; // LED OFF
		}
    }
}

