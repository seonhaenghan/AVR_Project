/*
 * time_clock.h
 *
 * Created: 2022-08-08 오후 3:15:26
 *  Author: kccistc
 */ 


#ifndef TIME_CLOCK_H_
#define TIME_CLOCK_H_
#include <avr/io.h> // ddr관련 모두 다 들었음

// char a[10];
// 서로 다른 변수의 묶음 : 구조체에서 정의할 수 있다. 

typedef struct _time // 사용자 정의 
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec; // unsigned char sec; 와 동일함 
}TIME; // TIME이라는 이름으로 재정의 한다.  

void inc_time_sec(void);
void set_time_clock(TIME time);
void get_time_clock(TIME *time);
void watch_reset();

#endif /* TIME_CLOCK_H_ */