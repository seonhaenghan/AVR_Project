/*
 * UART0.c
 *
 * Created: 2022-08-09 오전 10:45:06
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h> // strcpy, strcat, strcmp 
#include "UART0.h"
#include "led.h"

// 1. 전송 속도 설정 9600bps : 총 byte 수(글자 수) : 9600 / 10(st, sp 로 인해 +2bit) == 960 자 (1글자 송.수신 소요시간 약 : 0.96ms)
// 2. 비동기식, data 8bit, non parity 
// 3. RX interrupt 활성화 // data를 놓치지 않기 위해 interrupt 사용 
// 4. 2배속 통신 

// UART0로 부터 1byte가 들어오묜 RX 수신 interrupt가 발생하여 이곳으로 들어옴 
//page278 12-3 참조




ISR(USART0_RX_vect)
{
	unsigned char data;	
	
	data = UDR0; // uart0의 h/w register (URD0)로 1byte를 읽어 들인다. 
	
	if (data == '\r' || data == '\n')// 문장의 끝인 경우 
	{
		rx_buffer[i] = '\0'; // 문장의 끝을 알려줌 (NULL)
		i = 0;				 // 개선점 : circular queue (환형큐)로 개선을 해야한다. 
							 // 이렇게 프로그램을 작성하면, 새로운 메세지가 왔을 때 덮어 쓴다. 
							 
		rx_ready_flag = 1;	 // 완전한 문장이 들어 왔을 때 1로 set 된다. 
	} 
	else
	{
		rx_buffer[i++] = data; // 1. rx_buffer[i] = data  / 2. i++ 
	}
}

void init_uart0()
{
	UBRR0H = 0x00; 
	UBRR0L = 207; // 9600bps 
	
	UCSR0A |= (1 << U2X0); // 2배속 통신
	UCSR0C |= 0x06; // ASYNC(비동기) data 8 bit, none parity 
	
	// RXEN0 : UART0로 부터 수신이 가능 하도록 설정 
	// TXEN0 : UART0로 부터 송신이 가능 하도록 설정
	// RXCIE0 : UART0로 부터 1 byte가 들어 오면 RX(수신) Interrupt를 발생시켜라
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  // 송수신 활성 및 수신 시 Interrupt (alram)발생 
	// 19번 인터럽트 발생 
	
}
// UART0 로 1 byte를 보내는 함수 
void UART0_transmit(uint8_t data) 
{
	while (!(UCSR0A & (1 << UDRE0))) // data가 전송 중이면 data가 다 전송이 될 때 까지 기다린다. 
		;  // No Operation (아무 action도 취하지 않는다.)
	UDR0 = data; // HW 전송 register에 data를 쏴준다. 
	
}

// 1. command를 함수화 하여 led.c 에 배치한다. 
// 2. ledallon
// 3. ledalloff
// 4. ledalltoggle : 300ms 주기로 led 전체를 on&off 반복한다. 

extern int led_command; // flag setting 


void pc_command_processing()
{
	if(rx_ready_flag)	
	{
		rx_ready_flag = 0; // 0으로 변환하여 다음 메세지를 받을 수 있도록 초기화 
		printf("%s\n", rx_buffer);
		
		if(strncmp(rx_buffer, "ledallon" ,strlen("ledallon")-1 )== 0) // 내용이 같으면 0이나옴 // -1은 NULL 문자 제거를 위함 ('\n')
			PORTA = 0xff;
		
		if(strncmp(rx_buffer, "ledalloff" ,strlen("ledalloff")-1 )== 0)
			PORTA = 0x00;
			
		if(strncmp(rx_buffer, "reset" ,strlen("reset")-1 )== 0)
			PORTA = 0x00;
			led_command = 0;
		
		if(strncmp(rx_buffer, "ledalltoggle" ,strlen("ledalltoggle")-1 )== 0) // state를 확인 할 command flag가 필요 

			led_command = 1; 
		
		if(strncmp(rx_buffer, "shiftleftledon" ,strlen("shiftleftledon")-1 )== 0) 
			led_command = 2;
		
		if(strncmp(rx_buffer, "shiftrightledon" ,strlen("shiftrightledon")-1 )== 0) 
			led_command = 3;
		
		if(strncmp(rx_buffer, "shiftleftholdledon" ,strlen("shiftleftholdledon")-1 )== 0) 
			led_command = 4;
		
		if(strncmp(rx_buffer, "shiftrightholdledon" ,strlen("shiftrightholdledon")-1 )== 0) 
			led_command = 5;
		
		if(strncmp(rx_buffer, "f_off" ,strlen("f_off")-1 )== 0) 
			led_command = 6;
		
		if(strncmp(rx_buffer, "flower_on" ,strlen("flower_on")-1 )== 0) 
			led_command = 7;
		
		
		
		
		 
		
	}
	 switch(led_command)
	 {
		
		 
		 case 1:
		 ledalltoggle();
		 
		 break;
		 
		 case 2:
		 shiftleftledon();
		 
		 break;
		 
		 case 3:
		 shiftrightledon();
		 break;
		 
		 case 4:
		 shiftleftholdledon();
		 break;
		 
		 case 5:
		 shiftrightholdledon();
		 break;
		 
		 case 6:
		 f_off();
		 break;
		 
		 case 7:
		 flower_on();
		 break;
		 
	 }
}
