// examproject.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "examproject.h"#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "KWnd.h"
#include "resource.h"

#define Pi 3.14159265

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPlayer(HWND hwnd, HDC hdc, HDC hMemFrameDC, HBITMAP hBmp, BITMAP bm, FLOAT x, FLOAT y);
int getIDRegion(POINT p);

int cx, cy; // размеры экрана
BITMAP bm; // инфо о растре
bool dragging = false; // if left mouse button is pressed right now

//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	KWnd mainWnd(
		L"BeemoPlayer",
		hInstance, nCmdShow, WndProc,
		NULL,
		0, 0,
		500, 500,
		CS_HREDRAW | CS_VREDRAW,
		WS_POPUP | WS_VISIBLE,
		nullptr
	);

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//====================================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static FLOAT x = 0, y = 0; // текуща€ позици€ окна с м€чом
	static FLOAT dX, dY; // приращени€ координат дл€ сдвига на новую позицию
	static int alpha = 0; // угол, определ€ющий вращение м€ча

	static HBITMAP hBmpPlayer; // растр с изображением м€ча
	static HDC hMemDcFrame;   // совместимый контекст (контекст в пам€ти)
	static HBITMAP hBmpFrame; // растр дл€ совместимого контекста
	static HDC hDC;          // контекст диспле€
	static RECT rect;  // пр€моугольник клиентской области
	static POINT MousePnt, curPoint; // used to get coordinates of the mouse

	HDC hMemBmpDcPlayer;

	switch (message) {
	case WM_CREATE:
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hBmpPlayer = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBmpPlayer, sizeof(bm), (LPSTR)&bm);
		break;

	case WM_PAINT:
		hMemBmpDcPlayer = CreateCompatibleDC(hDC);
		SelectObject(hMemBmpDcPlayer, hBmpPlayer);
		BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight,
				hMemBmpDcPlayer, 0, 0, SRCCOPY);
		break;

	case WM_DESTROY:
		ReleaseDC(hWnd, hDC);
		DeleteDC(hMemDcFrame);
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		// TODO : checking with mask
		GetCursorPos(&MousePnt);
		switch (getIDRegion(MousePnt)) {
		// move window
		case 0:	
			dragging = true;
			SetCapture(hWnd);
		default:
			MessageBox(NULL, L"Error", L"Error", 0);
			break;
		}
		
		
		break;

	case WM_MOUSEMOVE:
		if (dragging) {		
			GetCursorPos(&curPoint);

			rect.left = rect.left + (curPoint.x - MousePnt.x);
			rect.top = rect.top + (curPoint.y - MousePnt.y);

			SetWindowPos(hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);

			MousePnt = curPoint; 
		}
		break;
	case WM_LBUTTONUP:
		if (dragging) {
			dragging = false;
			ReleaseCapture();
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

int getIDRegion(POINT p) {
	return 0;
}