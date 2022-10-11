/*
 * uart0.h
 *
 * Created: 2022-08-09 오전 10:45:18
 *  Author: kcci
 */ 


#ifndef UART0_H_
#define UART0_H_
void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing();


//  UART0로 부터 1 byte가 들어 오면 rx(수신) interrupt발생 되어 이곳으로 들어 온다.
// P278 12-3 참조

  // new circular queue (환형버퍼, 환형queue, ring buffer)
volatile int input_pointer=1;   // ISR(USART0_RX_vect)에서 save하는 index값
volatile int output_pointer=0;  // pc_command_processing 에서 가져 가는 index값
volatile unsigned char rx_buff[100][80];
// 제목:  UART0 RX Circular Queue 구현 
// 아래의 내용을 구현 후 PT로 작성 하여 이메일로 제출 하시오 
// 1. circular queue 조사 
// 2. pc_command_processing 와 ISR(USART0_RX_vect)를 circular queue 적용 source code 
//   ledallon
//   ledalloff
//   led_left_shift_on
//   led_right_shift_on
//   led1on
//   led1off 
//   led2off
//   led3off 
//  3. 효과 파악( Circular queue 적용전 적용후 비교 )


volatile uint8_t rx_ready_flag=0;   // 완전한 문장이 들어 왔을때 1로 set된다
volatile int i=0;

extern volatile uint32_t led_ms_count;
extern volatile uint32_t led_sec_count;

int k = 0;


#endif /* UART0_H_ */