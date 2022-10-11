/*
 * uart0.c
 *
 * Created: 2022-08-09 오전 10:44:59
 *  Author: kcci
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>  // strncmp, strcpy 등등 

#include "uart0.h"
 
ISR(USART0_RX_vect) // intput 
{
	uint8_t data;
	
	data = UDR0; // uart0의 HW register(UDR0)로 1 byte를 읽어 들인다.
	
	if (data == '\r' || data == '\n')   // 문장의 끝 이면 
	{
		
		rx_buff[input_pointer][i] = '\0';   // 문장의 끝을 가리키는 NULL
		i=0;     // 개선점 : circular queue (환형큐)로 개선을 해야 한다. 
		         // 이렇게 짜면 새로운 메세지가 오면 덮어 쓴다.
		
		 input_pointer = input_pointer +1 %100; // 완전한 문장이 들어왔을 때 Rear pointer 증가
		 
		 if(input_pointer == output_pointer)   // REAR와 Front 가 겹치지 않는지 확인 
		 {
			input_pointer = input_pointer -1 %100;
			printf("full\n");  
			
		 }
		 
		 
		rx_ready_flag=1;  // 완전한 문장이 들어 왔을때 1로 set된다
		
	}
	else
	{
		rx_buff[input_pointer][i++] = data;  // 1. rx_buff[i] = data  2. i++
	}
	
}
// 1. 전송속도: 9600bps : 총 byte(글자) 수: 9600 / 10 ==> 960자 (1글자 송.수신 소요 시간 : 약 1ms)
// 2. 비동기식, data 8bit, none parity
// 3. RX(수신) Interrupt 활성화 
void init_uart0(void)
{
	UBRR0H = 0x00;
	UBRR0L = 207;  // 9600bps  p219 표9-9
	
	UCSR0A |= (1 << U2X0);   // 2배속 통신 
	UCSR0C |= 0x06;  // ASYNC(비동기) data 8 bit, none parity
	
	// RXEN0 : UART0로부터 수신이 가능 하도록 설정
	// TXEN0 : UART0로부터 송신이 가능 하도록 설정
	// RXCIE0 : UART0로 부터 1 byte가 들어 오면 rx(수신) interrupt를 띄어 달라 
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	
}

// UART0로 1 byte를 보내는 함수
void UART0_transmit(uint8_t data)
{
	while ( !(UCSR0A & (1 << UDRE0)))  // data가 전송 중이면 data가 다 전송될떄 까지 기다린다.
		;    // no operation ㅣ 아무 action도 취하지 않는다. 
	UDR0 = data;   // HW 전송 register에 data를 쏴준다. 
}

// 1. command 를 함수화 하여 led.c에 배치 한다. 
// ledallon
// ledalloff
// ledalltoggle : 300ms 주기로 led전체를 on/off 반복 한다.
extern int led_command;  // nothing 

void pc_command_processing() // output 
{
	
	if (rx_ready_flag)  // if (rx_ready_flag >= 1)
	{
		
		rx_ready_flag=0;
		
		
		
		printf("%s\n", rx_buff);
		if (strncmp(rx_buff, "ledallon", strlen("ledallon")-1) == 0)
		{
			PORTA = 0xff;
		}
		if (strncmp(rx_buff, "ledalloff", strlen("ledalloff")-1) == 0)
		{
			PORTA = 0x00;
		}
		if (strncmp(rx_buff, "led_left_shift_on", strlen("led_left_shift_on")-1) == 0)
		{
			PORTA = 0x80 >> k;
			if(led_sec_count % 2 == 0)
			{
				k++;
				if(k > 8)
				{
					k = 0;
				}
			}
		}
		if (strncmp(rx_buff, "led_right_shift_on", strlen("led_right_shift_on")-1) == 0)
		{
			PORTA = 0x01 << k;
			if(led_sec_count % 2 == 0)
			{
				k++;
				if(k > 8)
				{
					k = 0; 
					
			
				}
			}
		}
		if (strncmp(rx_buff, "led1on", strlen("led1on")-1) == 0)
		{
			PORTA |= 0x01;
		}
		if (strncmp(rx_buff, "led1off", strlen("led1off")-1) == 0)
		{
			PORTA &= ~0x01;
		}
		if (strncmp(rx_buff, "led2off", strlen("led2off")-1) == 0)
		{
			PORTA &= ~0x02;
		}
		if (strncmp(rx_buff, "led3off", strlen("led3off")-1) == 0)
		{
			PORTA &= 0x04;
		}
				
	}	
	
	switch(led_command)
	{
		case 1:
		ledalltoggle();
		break;
		case 2:
		break;
	}
	
}