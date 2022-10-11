/*
 * stop_watch.c
 *
 * Created: 2022-08-08 오후 4:07:32
 *  Author: kccistc
 */ 
#include "stop_watch.h"

int stopwatch_state = 0; 
uint16_t stopwatchclock = 0; 
int stopwatch_hour = 0;
int stopwatch_min = 0;
int stopwatch_sec = 0;
 uint8_t count = 0; 


// 1초에 한 번씩 Call을 한다.
void inc_stopwatch_clock(void)
{	
	count++; 
	if(count >= 100 )
	{
		count = 0; 
		
		stopwatch_sec++;
		if(stopwatch_sec >= 60)
		{
			stopwatch_sec = 0; // 다음 초를 위해 CLEAR
			stopwatch_min++;
			
			if(stopwatch_min >= 60)
			{
				stopwatch_min = 0;
				stopwatch_hour++;
				
				if(stopwatch_hour >=24)
				{
					stopwatch_hour = 0;
					
					
				}
			}
		}
	}
	 stopwatchclock = (stopwatch_sec + stopwatch_min * 100 ); // 총 몇인지 나옴 
	
}



 
void stopwatch_run()
{
	set_fnd_data(stopwatchclock); // FND에 출력할 data가 넘어간다. 
	
	if (get_BUTTON2())
	{
		stopwatch_state = STOP; 
		
	}
}

void stopwatch_stop()
{
	set_fnd_data(stopwatchclock); // FND에 출력할 data가 넘어간다.
	
	if (get_BUTTON2())
	{
		stopwatch_state = RUN; 
		
	}
	if (get_BUTTON3())
	{
		stopwatch_state = RESET; 
		
	}
}

void stopwatch_reset()
{
	stopwatchclock = 0; 
	stopwatch_state = 0;
	stopwatch_hour = 0;
	stopwatch_min = 0;
	stopwatch_sec = 0;
	count = 0;
	
	set_fnd_data(stopwatchclock);
	stopwatch_state = STOP;
	
	
}

