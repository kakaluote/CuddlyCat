#pragma once

#include "Director.h"
#include "define.h"

NS_CC_BEGIN

class Director
{
public:
	static Director* getInstance();
	static void destroyInstance();

	Director();
	~Director();
	bool init();

	void setDeltaTime(double d) {
		_deltaTime = d;
	}
	double getDeltaTime() {
		return _deltaTime;
	}

private:
	double _deltaTime;
};

NS_CC_END