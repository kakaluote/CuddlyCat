#include "Director.h"


NS_CC_BEGIN

static Director *s_SharedDirector = nullptr;

Director* Director::getInstance()
{
	if (!s_SharedDirector)
	{
		s_SharedDirector = new Director;
		s_SharedDirector->init();
	}

	return s_SharedDirector;
}

void Director::destroyInstance()
{
	CC_SAFE_DELETE(s_SharedDirector);
}

Director::Director():_deltaTime(0)
{
}

Director::~Director()
{
}

bool Director::init()
{
	return true;
}

NS_CC_END