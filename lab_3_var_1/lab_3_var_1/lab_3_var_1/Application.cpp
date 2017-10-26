#include "stdafx.h"
#include "Application.h"

Application::Application() {
	pMyWindow = new MyWindow;
	pMyWindow->CreateMyWindow();
};

BOOL Application::Run() {
	return MessageLoop();
}

BOOL Application::MessageLoop() {
	MSG msg;
	if (!pMyWindow)
		return -2;
	if (pMyWindow->hWnd == INVALID_HANDLE_VALUE)
		return -1;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (BOOL)msg.wParam;
}

