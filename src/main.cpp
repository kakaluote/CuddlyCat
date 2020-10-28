
#include <iostream>
#include "CuddlyCat/App.h"
#include "CuddlyCat/define.h"
#include "demo/DemoCtrl.h"

USING_NS_CC

int main() {
	App app;
	if (!app.init()) {
		return -1;
	}

	app.setDemoHandler(DemoCtrl::getInstance()->getDemo(4));

	app.mainLoop();

	DemoCtrl::destroyInstance();
    return 0;
}
