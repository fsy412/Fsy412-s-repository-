#include "Timer.h"
Timer::Timer()
{
	// _start_time = clock(); interval = 1.0;
}
void Timer::restart()  
{ 
	_start_time = clock();
}
void Timer::set_interval(double interval)  
{ 
	_interval = interval;
}

double Timer::elapsed() const 
{
	return  double(clock() - _start_time) / CLOCKS_PER_SEC; 
}
bool Timer::timeout()
{
	if ( elapsed() == _interval)
	{
		restart();
		return true;
	}else
	{
		return false;
	}
}
