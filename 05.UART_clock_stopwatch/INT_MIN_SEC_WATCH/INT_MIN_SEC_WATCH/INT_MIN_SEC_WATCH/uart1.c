/*
 * uart1.c
 *
 * Created: 2022-08-08 오후 9:23:06
 *  Author: sikwon
 */ 

#include "uart1.h"

volatile uint8_t rx1String[64]={0,};
volatile uint8_t rx1ReadyFlag=0;
extern uint32_t led_command; 

ISR(USART1_RX_vect)
{
	UART1_ISR_Receive();		 // 1char가 들어올 떄마다 call 
}

void init_UART1()
{
	UCSR1B |= ((1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1));	// RX interrupt 수신 알림 
	UCSR1A |= (1<<U2X1);
	UBRR1H = 0;
	UBRR1L = 207; // 9600 buadrate
}


void UART1_transmit(unsigned char data)
{	
	while ( !(UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
}


void UART1_print_string(unsigned char *str)
{
	for (int i=0; str[i]; i++)
	UART1_transmit(str[i]);
}

uint8_t UART1_IsRxD()
{
	return (UCSR1A & (1<<7));
}


void UART1_ISR_Receive()
{
	volatile static uint8_t head=0;
	volatile uint8_t data;
	
	data = UDR1;
	UART0_transmit(data);		// comportmaster로 data 송신 
	if (data == '\n' || data == '\r')
	{
		if (head) rx1String[head] = '\0';
		head=0;
		rx1ReadyFlag = 1;
	}
	else
	{
		rx1String[head] = data;
		head++;
	}
}

void bt_command_processing()
{
	if(rx1ReadyFlag)
	{
		rx1ReadyFlag = 0; // 0으로 변환하여 다음 메세지를 받을 수 있도록 초기화
		printf("%s\n", rx1String);
		
		if(strncmp(rx1String, "ledallon" ,strlen("ledallon")-1 )== 0) // 내용이 같으면 0이나옴 // -1은 NULL 문자 제거를 위함 ('\n')
		PORTA = 0xff;
		
		if(strncmp(rx1String, "ledalloff" ,strlen("ledalloff")-1 )== 0)
		PORTA = 0x00;
		
		if(strncmp(rx1String, "reset" ,strlen("reset")-1 )== 0)
		PORTA = 0x00;
		led_command = 0;
		
		if(strncmp(rx1String, "ledalltoggle" ,strlen("ledalltoggle")-1 )== 0) // state를 확인 할 command flag가 필요

		led_command = 1;
		
		if(strncmp(rx1String, "shiftleftledon" ,strlen("shiftleftledon")-1 )== 0)
		led_command = 2;
		
		if(strncmp(rx1String, "shiftrightledon" ,strlen("shiftrightledon")-1 )== 0)
		led_command = 3;
		
		if(strncmp(rx1String, "shiftleftholdledon" ,strlen("shiftleftholdledon")-1 )== 0)
		led_command = 4;
		
		if(strncmp(rx1String, "shiftrightholdledon" ,strlen("shiftrightholdledon")-1 )== 0)
		led_command = 5;
		
		if(strncmp(rx1String, "f_off" ,strlen("f_off")-1 )== 0)
		led_command = 6;
		
		if(strncmp(rx1String, "flower_on" ,strlen("flower_on")-1 )== 0)
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
