#include "Timer.h"
Timer::Timer()
{
	// _start_time = clock(); interval = 1.0;
	stop();
}
void Timer::restart()  
{ 
	log->PushLog(DEBUG," Timer::restart() ");
	_start_time = clock();
	_run = true;
}
void Timer::set_interval(double interval)  
{ 
	_interval = interval;
}

double Timer::elapsed() const 
{
	return _run ?  (double(clock() - _start_time) / CLOCKS_PER_SEC) : 0;
}
bool Timer::timeout()
{
	if ( elapsed() == _interval)
	{
		//restart();
		stop();	
		log->PushLog(DEBUG," Timer::timeout() ");
		return true;
	}else
	{
 		return false;
	}
}
