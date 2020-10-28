#include "Timer.h"


NS_CC_BEGIN


Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::tik()
{
	_start = std::chrono::steady_clock::now();
}

double Timer::tok()
{
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::duration<double> timeUsed = end - _start;
	return timeUsed.count() * 1000;
}

NS_CC_END




