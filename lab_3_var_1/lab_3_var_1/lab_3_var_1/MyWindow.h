#pragma once

#include <Windows.h>

class MyWindow {
public:
	HWND hWnd;
	HINSTANCE hInst;
	LPVOID lpProc;

	MyWindow() {}
	~MyWindow() {}

	BOOL CreateMyWindow();
	BOOL ShowMyWindow(bool blshow = true) {}
	void CloseMyWindow() {
		DestroyWindow(hWnd); 
	}

	static LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT message,
		WPARAM wparam,
		LPARAM lparam
	);
};

