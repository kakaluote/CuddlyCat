#pragma once

#include "define.h"
#include <chrono>


NS_CC_BEGIN

class Timer
{
public:
	Timer();
	~Timer();

	void tik();
	double tok();

private:
	std::chrono::steady_clock::time_point _start;
};

NS_CC_END