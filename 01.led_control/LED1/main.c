/*
 * led.c //source file name
 *
 * Created: 2022-08-03 오후 3:17:48
 * Author : kccistc
 */ 


#define F_CPU 16000000UL // 16MHz로 설정 
#include <avr/io.h> // gpio
#include <util/delay.h>
// ff = 00000000 = 76543210 (Port의 자리 수 )
// 1. 전체 LED를 On --> 1초 유지 --> off --> 1초 유지
// 추가 사항 2. 홀수 번 Bit ON --> 1초 유지 --> 짝수 번 Bit on
// 3. 전체 Off --> 1초 유지
// 4. 배열 처리로 Program 을 구현
// unsigned char led[] = {} ; -->>
//  76543210
//         1 = 300msec 유지
//        1  = 300msec 유지
//		 1	 = 300mesc 유지
//		1 	 = 300mesc 유지
//	   1 	 = 300mesc 유지
//	  1  	 = 300mesc 유지
//   1	     = 300mesc 유지
//  1   	 = 300mesc 유지


int main(void)
{
	DDRA = 0xff; // portA의 전체에 LED가 연결되어 있으므로 전부 출력 MODE로 설정 
	
	unsigned char ledarray[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	// 좌측 부터 2진수 변환 시 
	// 0b00000001 0b00000010 0b00000100 0b00001000 0b00010000 0b00100000 0b01000000 0b10000000
		
	while (1)	 
	{	
		for(int i = 0; i < 8; i++)
		{
			PORTA = ledarray[i];
			_delay_ms(300); // 1sec delay		
		}
		
	}
}


