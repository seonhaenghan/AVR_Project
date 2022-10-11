/*
 * BUTTON_TOGGLE.c
 *
 * Created: 2022-08-04 오후 4:06:12
 * Author : kccistc
 */ 

#define F_CPU 16000000UL // Unsigned Long
#include <avr/io.h>
#include "BUTTON.h" //만들어준 HEADER를 불러옴 
extern int get_BUTTON1(); //다른 file에 있는 함수라고 CONFILE에 알려줌
extern int get_BUTTON2(); //다른 file에 있는 함수라고 CONFILE에 알려줌
extern int get_BUTTON3();
extern void init_BUTTON();

void button1_play();
void button2_play();
void button3_play();

int BUTTON1_state = 0; // 초기 상태는 버튼을 뗀 상태로 출발 
int BUTTON2_state = 0;
int BUTTON3_state = 0;


int main(void)
{
	LED_DDR = 0xff;		//DDRA = 0xff; LED 출력 모드 이기 때문에 1로
	LED_PORT = 0x00;	//LED port all off
	
	init_BUTTON();

	
	
	while (1)
	{
	button1_play();
	button2_play();	
	button3_play();	
	}
	
	
	
}

void button1_play()
{


		if(get_BUTTON1()) // 버튼을 1번 눌렀다 떼면
		{
			BUTTON1_state = !BUTTON1_state;
			// if(BUTTON1_state ==0)
			//    BUTTON1_state = 1;    logic이 동일함
			// else BUTTON1_state = 0;
			if(BUTTON1_state)	// if(BUTTON1_state >= 1)
			{
				
				for(int i = 0; i < 8; i++)
				{
					LED_PORT = 0x01 << i;
					_delay_ms(100);
				}
				//BUTTON1_state = !BUTTON1_state;
			}
			else
			{
				
				for(int i = 0; i < 8; i++)
				{
					LED_PORT = 0x80 >> i;
					_delay_ms(100);
				}
				//BUTTON1_state = !BUTTON1_state;
			}
			
		}
	
	
	
}


void button2_play()
{
		if(get_BUTTON2()) // 버튼을 1번 눌렀다 떼면
		{
			BUTTON2_state = !BUTTON2_state;
			// if(BUTTON1_state ==0)
			//    BUTTON1_state = 1;    logic이 동일함
			// else BUTTON1_state = 0;
			if(BUTTON2_state)	// if(BUTTON1_state >= 1)
			{
				
				for(int i = 8; i >= 0 ; i--)
				{
					LED_PORT = 0xff >> i;
					_delay_ms(100);
					if(!i)
					LED_PORT = 0x00;
				}
			}
			else
			{
				
				for(int i = 8; i >= 0; i--)
				{
					
					LED_PORT = 0xff << i;
					_delay_ms(100);
					if(!i)
					LED_PORT = 0x00;
				}
				
			}
		}	
}


void button3_play()
{

	
		
		if(get_BUTTON3()) // 버튼을 1번 눌렀다 떼면
		{
			BUTTON3_state = !BUTTON3_state;
			
			if(BUTTON3_state)
			{
				int h,l;
				for(int i = 0; i < 4; i++)
				{
					h = 0x80 >> i;
					l = 0x01 << i;
					LED_PORT = h | l ;
					_delay_ms(1000);
					
				}
			}
			else
			{
				
				int h2,l2;
				for(int i = 0; i < 4; i++)
				{
					h2 = 0x10 << i;
					l2 = 0x08 >> i;
					LED_PORT = h2 | l2 ;
					_delay_ms(1000);
					
				}
				
			}
		}
		
	

}

