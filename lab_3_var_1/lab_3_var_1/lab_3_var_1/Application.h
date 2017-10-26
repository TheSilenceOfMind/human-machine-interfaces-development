#pragma once

#include "MyWindow.h"

class Application {
public:
	Application();
	BOOL Run();

private:
	MyWindow* pMyWindow;
	BOOL MessageLoop();
};