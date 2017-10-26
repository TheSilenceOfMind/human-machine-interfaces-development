#include "stdafx.h"
#include "MyWindow.h"

static int count = 0;

BOOL MyWindow::CreateMyWindow() {
	BOOL result = 0;

	count++;
	hWnd = CreateWindow(
		L"edit", L"text editor",
		WS_VISIBLE,
		100*count, 100*count, 300, 300,
		NULL, NULL,
		(HINSTANCE)GetModuleHandle(NULL), this
	);
	if (hWnd == INVALID_HANDLE_VALUE)
		return GetLastError();

	// save the pointer to wndProc AS THE CLASS MEMBER
	lpProc = (LPVOID)GetWindowLong(hWnd, GWLP_WNDPROC);

	// change the pointer to handle all messages to WndProc defined by us
	if (!SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc)) { 
		return GetLastError();
	}

	// init data handler
	if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this)) {
		return GetLastError();
	}

	return 0;
}

LRESULT CALLBACK MyWindow::WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wparam,
	LPARAM lparam) 
{
	MyWindow* pmw = (MyWindow*)GetWindowLong(hwnd, GWLP_USERDATA);

	switch (message) {
		case WM_CLOSE:
			pmw->CloseMyWindow();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			DefWindowProc(hwnd, message, wparam, lparam);
			break;
	}

	return CallWindowProc(
		(WNDPROC)pmw->lpProc, 
		hwnd,
		message, 
		wparam, 
		lparam
	);
}