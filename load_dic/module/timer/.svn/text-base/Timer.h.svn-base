#ifndef CTimer_h
#define CTimer_h 1
#include<stdio.h>
#include <iostream> 
class Timer 
{ 
public: 
	Timer();
	void restart(); 
	double elapsed() const;
	bool timeout();
	void set_interval(double interval);
private: 
	clock_t _start_time; 
	double _interval;
 
}; // time
 
#endif
