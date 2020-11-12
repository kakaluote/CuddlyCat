#pragma once

#include "define.h"
#include "Object.h"

NS_CC_BEGIN

typedef enum {
	Transform = 0,
	MeshRenderer
}ComponentType;

class Component:public Object
{
public:
	Component();
	~Component();

private:
};





NS_CC_END