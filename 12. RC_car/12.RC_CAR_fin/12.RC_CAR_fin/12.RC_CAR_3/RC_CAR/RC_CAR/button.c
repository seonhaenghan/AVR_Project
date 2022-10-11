/*
 * button.c
 *
 * Created: 2022-08-08 오후 2:18:09
 *  Author: kccistc
 */ 


#include "BUTTON.h"
#include "led.h" 

int button1_state = 0;

void auto_mode_check(void)
{
	if(get_BUTTON1())
	{
		button1_state = !button1_state; 
	}
	if(button1_state)
	{
		AUTO_RUN_LED_PORT |= 1 << 4; // LED on 
	}
	else
	{
		AUTO_RUN_LED_PORT &= ~(1 << 4); // LED off 
		
	}
}

void init_BUTTON()
{
	BUTTON_DDR &= ~(1 << BUTTON1) ;
	
	 // input mode : 0으로 설정
	 // DDR을 0으로 initial 한다는 개념
}

 
int get_BUTTON1()
{
	 
	static uint8_t prev_state = 0; // 버튼의 상태 0: OFF , 1 : ON / 함수 실행 후에도 값이 변하지 않음
	uint8_t current_state; // unsigned char와 동일함 == unsigned int 8bit  // unsigned char current_state 
	
	current_state = BUTTON_PIN & (1 << BUTTON1); // Button1의 값을 읽는다. , 읽었을 때 (0x10)
	
	if(prev_state == 0 && current_state != 0) // 버튼이 처음 눌려진 상태 	
	{
		_delay_ms(60);	// 노이즈가 지나가기를 기다림
		prev_state = 1; // 처음 눌러진 상태가 아니다. 
		return 0;		// 아직은 진짜로 눌러진 상태가 아니라 0을 return 
		
	}
	else if(prev_state == 1 && current_state == 0) // 버튼이 눌렸다 떼어진 상태 
	{
		prev_state = 0; 
		_delay_ms(30);	// 노이즈가 지나가기를 기다림
		return 1; // 완전히 눌렸다 떼어진 상태로 인정
	}
	return 0; // 아직 완전히 sw 을 눌렀다 떼어지지 않은 상태
}

