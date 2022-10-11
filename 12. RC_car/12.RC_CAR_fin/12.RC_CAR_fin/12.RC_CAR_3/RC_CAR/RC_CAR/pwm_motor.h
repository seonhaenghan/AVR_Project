/*
 * pwm_motor.h
 *
 * Created: 2022-08-19 오후 2:20:54
 *  Author: kccistc
 */ 


#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MOTOR_DRIVER_PORT PORTF
#define MOTOR_DRIVER_PORT_DDR DDRF

#define MOTOR_DDR DDRB
#define MOTOR_RIGHT_PORT_DDR 5 // OC1A
#define MOTOR_LEFT_PORT_DDR 6  // OC1B

extern volatile unsigned char UART1_RX_data;
extern void led_stop();
extern void led_forward();
extern void led_left();
extern void led_right();
void init_pwm_motor(void);

volatile uint32_t motor_ms_count = 0; 
volatile uint32_t motor_sec_count = 0; 


#endif /* PWM_MOTOR_H_ */