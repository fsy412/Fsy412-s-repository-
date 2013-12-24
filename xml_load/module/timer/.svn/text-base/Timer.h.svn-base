#ifndef CTimer_h
#define CTimer_h 1
#include<stdio.h>
#include <iostream> 

#include <log.hpp>
extern module::CLog *log;

class Timer 
{ 
public: 
	Timer();
	void restart(); 
	double elapsed() const;
	bool timeout();
	void set_interval(double interval);
	void stop(){_run = false;}
	bool isRun(){	return _run;}
private: 
	clock_t _start_time; 
	double _interval;
 	bool _run;
}; // time
 
#endif
