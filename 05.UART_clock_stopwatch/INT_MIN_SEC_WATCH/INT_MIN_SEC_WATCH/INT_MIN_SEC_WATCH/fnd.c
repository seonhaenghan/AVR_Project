/*
 * fnd.c
 *
 * Created: 2022-08-08 오후 2:23:26
 *  Author: kccistc
 */ 

#include "fnd.h" 

uint16_t fnd_data;
uint16_t sec_data;
//uint32_t digit_position=0;  // 출력할 자리수

void set_fnd_data(uint16_t data)
{
	fnd_data = data; 
	
}


uint16_t get_fnd_data(void)
{
	return fnd_data; 
	
}

//extern uint8_t digit_position;		 // main에 전역 변수로 선언한 경우 이 내용을 추가한다. 
void display_fnd(void)
{
	// uint8_t
	#if 1
	unsigned char fnd_font[] = {0xc0, 0xf9, 0xa4,0xb0, 0x99,0x92, 0x82, 0xd8, 0x80, 0x98};   // common애노우드
	#else
	unsigned char fnd_font[] = {~0xc0, ~0xf9, ~0xa4,~0xb0, ~0x99,~0x92, ~0x82, ~0xd8, ~0x80, ~0x98};   // common 캐소우드
	#endif
	
	static uint16_t digit_position = 0; // static 을 선언하면 지역 변수라도 함수 호출 뒤의 값을 그대로 유지 
	
	uint16_t data = get_fnd_data(); 

	switch(digit_position)
	{
		case 0:   // 1단위
		#if 1
		FND_DIGHT_PORT = 0b10000000;  // 에노우드
		#else
		FND_DIGHT_PORT = ~0b10000000;  // 캐소우드
		#endif
		FND_DATA_PORT = fnd_font[data %10];  // 0~9 123456789 101112
		break;
		
		case 1:   // 10단위
		#if 1
		FND_DIGHT_PORT = 0b01000000;  // 에노우드
		#else
		FND_DIGHT_PORT = ~0b01000000;   // 캐소우드
		#endif
		FND_DATA_PORT = fnd_font[data /10%10];
		break;
		
		case 2:   // 100단위   분
		#if 1
		FND_DIGHT_PORT = 0b00100000;  // 에노우드
		#else
		FND_DIGHT_PORT = ~0b00100000;  // 캐소우드
		#endif
		FND_DATA_PORT = fnd_font[data /100%10];
		break;
		
		case 3:   // 1000
		#if 1
		FND_DIGHT_PORT = 0b00010000;  // 에노우드
		#else
		FND_DIGHT_PORT = ~0b00010000;  // 캐소우드
		#endif
		FND_DATA_PORT = fnd_font[data /1000%6];
		break;
	}
	digit_position++;   // 다음 표시할 자리수
	digit_position %= 4;  // digit_position = digit_position % 4
}
void init_fnd(void)
{
	FND_DATA_DDR = 0xff;
	FND_DIGHT_DDR |= 0xf0;   // 4567만 1로 3210은 그대로 유지
	#if 0
	FND_DATA_PORT = ~0x00;   // common 애노우드 FND를 all off
	#else
	FND_DATA_PORT = 0x00;   // 0xff common 케소우드 FND를 all off
	#endif
}


