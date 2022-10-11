	/*
 * time_clock.c
 *
 * Created: 2022-08-08 오후 3:15:45
 *  Author: kccistc
 */ 

#include "time_clock.h" // "" 현재 디렉토리에 있는 Header file을 사용한다. 

TIME time_clock = {0, 0, 0};
	
	
	
void get_time_clock(TIME *time) // *time 은 TIME의 주소를 갖는다. 
{
	 *time=time_clock;			// call by ref 
	
}
void set_time_clock(TIME time) // *time 은 TIME의 주소를 갖는다.
{
	time_clock = time;		   // call by value
	
}
// 1초에 한 번씩 Call을 한다. 
void inc_time_sec(void)
{
	time_clock.sec++;
	if(time_clock.sec >= 60)
	{
		time_clock.sec = 0; // 다음 초를 위해 CLEAR
		time_clock.min++; 
		
		if(time_clock.min >= 60)
		{
			time_clock.min = 0;
			time_clock.hour++; 
			
			if(time_clock.hour >=24)
			{
				time_clock.hour = 0;
				
				
			}
		}
	}
	
}

