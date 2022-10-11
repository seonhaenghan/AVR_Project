/*
 * pwm_motor.c
 *
 * Created: 2022-08-19 오후 2:20:14
 *  Author: kccistc
 */ 
#include "pwm_motor.h"


void init_pwm_motor(void)
{
	MOTOR_DRIVER_PORT_DDR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);		// motor driver port
	MOTOR_DDR |= (1 << MOTOR_RIGHT_PORT_DDR) | (1 << MOTOR_LEFT_PORT_DDR); // motor pwm port
	
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10); // 64 분주
	// 16000000Hz / 64 = 250000 Hz (Timer1에 공급되는 Clock)
	// 256bit count ==> 1.02ms (펄스를 256개를 카운트 하면 1.02ms 소요)
	// 1 / 250000  * 127 ==> 0.5ms
	// 0x3ff(1023) ==> 4ms Top값을 이와 같이 설정 시 모터는 최고속도로 동작함
	
	
	TCCR1B |= (1 << WGM13) | (1 << WGM12); // 모드 14 고속 PWM ICR1
	TCCR1A |= (1 << WGM11) | (0 << WGM10); // 모드 14 고속 PWM ICR1
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0); // 비반전 MODE 설정 : OCR 시 low TOP 시 high
	TCCR1A |= (1 << COM1B1) | (0 << COM1B0); // R, L 이 분리되어 있기에 2개 선언
	
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);						   // 전진모드로 Setting
	ICR1 = 0x3ff; // 1023 ==> 4ms
}




void manual_mode_run(void)
{
	switch(UART1_RX_data)
	{
		case 'F' :
		forward(500);	// // 4us * 500 = 0.002sec (2ms)
		break;
		
		case 'B' :
		backward(500);
		break;
		
		case 'L' :
		turn_left(700);
		break;
		
		case 'R' :
		turn_right(700);
		break;
		
		case 'S' :
		stop();
		break;
		
		default:
		break;
	}
}

void forward(int speed)
{
	led_forward();
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);						   // 전진모드로 Setting (1010)
	
	OCR1A = speed;	// PB5 PWM 출력 left
	OCR1B = speed;	// PB6 PWM 출력	right
	
}

void backward(int speed)
{
	led_stop();
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 1);						   // 후진모드로 set (0101)
	
	OCR1A = speed;	// PB5 PWM 출력 left
	OCR1B = speed;	// PB6 PWM 출력	right
	
}

void turn_left(int speed)
{
	led_left();
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);						   // 전진모드로 Setting (1010)
	//MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 1);
	
	OCR1A = 0;		// PB5 PWM 출력 left
	
	OCR1B = 1023;	// PB6 PWM 출력	right
}

void turn_right(int speed)
{
	led_right();
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 2) | (1 << 0);						   // 전진모드로 Setting (1010)
	
	//MOTOR_DRIVER_PORT |= (1 << 3) | (1 << 0);	
	
	OCR1A = 512;	// PB5 PWM 출력 left
	OCR1B = 0;		// PB6 PWM 출력	right
	//OCR1B = speed;
}

void stop()
{
	led_stop();
	OCR1A = 0;		// PB5 PWM 출력 left
	OCR1B = 0;		// PB6 PWM 출력	right
	
	MOTOR_DRIVER_PORT &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)); // Reset
	MOTOR_DRIVER_PORT |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);	   // stop mode
}
