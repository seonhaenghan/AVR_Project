/*
 * UART0.h
 *
 * Created: 2022-08-09 오전 10:45:18
 *  Author: kccistc
 */ 


#ifndef UART0_H_
#define UART0_H_
#include <stdio.h>
void UART0_transmit(uint8_t data);
void init_uart0();
void pc_command_processing();

volatile unsigned char rx_buffer[100]; // 최적화 방지를 위한 volatile
volatile uint8_t rx_ready_flag = 0; // 완전한 문장이 들어 왔을 때 1로 set 된다.
volatile int i = 0;

#endif /* UART0_H_ */