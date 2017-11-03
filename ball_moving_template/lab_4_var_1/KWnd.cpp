#include "stdafx.h"
#include "KWnd.h"

BOOL KWnd::isRegister;

KWnd::KWnd(
	LPCTSTR windowName,
	HINSTANCE hInst,
	int nCmdShow,
	LRESULT(WINAPI* pWndProc)(HWND, UINT, WPARAM, LPARAM),
	LPCTSTR menuName,
	int x, int y,
	int width, int height,
	UINT classStyle,
	DWORD windowStyle,
	HWND hParent
) 
{
	TCHAR szClassName[] = L"KWndClass";
	if (!isRegister) {
		wc.cbSize = sizeof(wc);
		wc.style = classStyle;
		wc.lpfnWndProc = pWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = menuName;
		wc.lpszClassName = szClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	}

	if (!RegisterClassEx(&wc)) {
		TCHAR msg[] = L"Cannot register class: ";
		lstrcat(msg, szClassName);
		MessageBox(NULL, msg, L"Error", MB_OK);
		return;
	} else isRegister = TRUE;

	hWnd = CreateWindow(
		szClassName, 
		windowName, 
		windowStyle,
		x, y, width, height, 
		hParent, NULL, 
		hInst, NULL
	);

	if (!hWnd) {
		TCHAR msg[] = L"Cannot create window: ";
		lstrcat(msg, szClassName);
		MessageBox(NULL, msg, L"Error", MB_OK);
		return;
	}

	//HRGN hRgn = CreateEllipticRgn(0, 0, 400, 400);
	//SetWindowRgn(hWnd, hRgn, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}