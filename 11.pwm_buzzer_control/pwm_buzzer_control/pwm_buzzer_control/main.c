/*
 * main.c
 *
 * Created: 2021-04-05 오전 10:46:57
 * Author : kcci
 */ 

#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>

extern void Music_Player(int *tone, int *Beats);
extern void init_speaker(void);
extern void Beep();
extern void Siren(int repeat);
extern void RRR(void);

extern const int Elise_Tune[];
extern const int Elise_Beats[];
extern const int School_Bell_Tune[];
extern const int School_Bell_Beats[];
// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.

int main(void)
{
	init_speaker();	

	while(1)
	{
		Beep();
/*
		
		OCR3A=1702;   // 삐~뽀 레 
		_delay_ms(1000);
		OCR3A=1431;   // 파  
		_delay_ms(1000);
*/

		
		// Beep(5);
	//	RRR();
	//	_delay_ms(3000);
 		//Siren(5);
	//	_delay_ms(3000);
		//Music_Player(School_Bell_Tune, School_Bell_Beats);		
	}
}
