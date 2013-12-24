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
	 gettimeofday( &start, NULL );
	_run = true;
}
void Timer::set_interval(double interval)  
{ 
	_interval = interval;
}

double Timer::elapsed() const 
{
    struct timeval end;
    gettimeofday(&end, NULL);
	return _run ?   (1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec)/1000000 : 0;
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
