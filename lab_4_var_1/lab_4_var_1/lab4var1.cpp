// lab4var1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab4var1.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "KWnd.h"
#include "resource.h"

#define Pi 3.14159265

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBall(HWND hwnd, HDC hdc, HDC hMemFrameDC, HBITMAP hBmp, BITMAP bm, FLOAT x, FLOAT y, int alpha);

int cx, cy; // ������� ������
BITMAP bm; // ���� � ������

//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	KWnd mainWnd(
		L"Flying ball", 
		hInstance, nCmdShow, WndProc, 
		NULL, 
		0, 0, 
		197, 198, 
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
	static FLOAT x = 0, y = 0; // ������� ������� ���� � �����
	static FLOAT dX, dY; // ���������� ��������� ��� ������ �� ����� �������
	static int alpha = 0; // ����, ������������ �������� ����

	static HBITMAP hBmpBall; // ����� � ������������ ����
	static HDC hMemDcFrame;   // ����������� �������� (�������� � ������)
	static HBITMAP hBmpFrame; // ����� ��� ������������ ���������
	static HDC hDC;          // �������� �������
	RECT rect;  // ������������� ���������� �������

	const static int PERIOD = 1000 / 120; // FPS = 120
	int addition = 0; // ���� �������� (+-)

	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 1, PERIOD, NULL);
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hBmpBall = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBmpBall, sizeof(bm), (LPSTR)&bm);

		hMemDcFrame = CreateCompatibleDC(hDC);
		hBmpFrame = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(hMemDcFrame, hBmpFrame);

		SetGraphicsMode(hMemDcFrame, GM_ADVANCED);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_DOWN:
			if (dY <= 20)
				dY += 5;
			break;
		case VK_UP:
			if (dY >= -20)
				dY -= 5;
			break;
		case VK_LEFT:
			if (dX >= -20)
				dX -= 5;
			break;
		case VK_RIGHT:
			if (dX <= 20)
				dX += 5;
			break;
		}
		break;
	case WM_TIMER:
		x += dX;
		y += dY;
		addition = (int)sqrt(dX*dX + dY*dY) / 2;
		if (dX < 0 || dY < 0)
			addition *= -1;
		alpha = (alpha + addition) % 360;

		// ���� ��� ������ ���� ����, ����������� ��� �������� ����������
		if (x + bm.bmWidth > cx || x < 0)
			dX = -dX;
		if (y + bm.bmHeight > cy || y < 0)
			dY = -dY;
		DrawBall(hWnd, hDC, hMemDcFrame, hBmpBall, bm, 0, 0, alpha);
		if (x >= cx - bm.bmWidth)
			x = cx - bm.bmWidth;
		if (x <= 0)
			x = 0;
		if (y >= cy - bm.bmHeight)
			y = cy - bm.bmHeight;
		if (y <= 0)
			y = 0;

		SetWindowPos(hWnd, HWND_TOPMOST, x, y, NULL, NULL, SWP_NOSIZE);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		ReleaseDC(hWnd, hDC);
		DeleteDC(hMemDcFrame);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

//====================================================================
void DrawBall(HWND hwnd, HDC hdc, HDC hMemFrameDC, HBITMAP hBmp, BITMAP bm, FLOAT x, FLOAT y, int alpha) {
	XFORM xform;

	// ���������� � ������ ����
	HDC hMemDcBall = CreateCompatibleDC(hdc);
	SelectObject(hMemDcBall, hBmp);

	// ������� �������������� ��� ����������� � �������� ����
	xform.eM11 = (FLOAT)cos(alpha * 2 * Pi / 360);	 //��������
	xform.eM12 = (FLOAT)sin(alpha * 2 * Pi / 360);	 //��������
	xform.eM21 = (FLOAT)-sin(alpha * 2 * Pi / 360); //��������
	xform.eM22 = (FLOAT)cos(alpha * 2 * Pi / 360);	 //��������
	xform.eDx = x + bm.bmWidth / 2.;		 //�������� �� ��� x
	xform.eDy = y + bm.bmHeight / 2.;		 //�������� �� ��� y

											 // ����� ����
	SaveDC(hMemFrameDC);
	BOOL ret = SetWorldTransform(hMemFrameDC, &xform);
	BitBlt(hMemFrameDC, -bm.bmWidth / 2, -bm.bmHeight / 2,
		bm.bmWidth, bm.bmHeight,
		hMemDcBall, 0, 0, SRCCOPY);
	RestoreDC(hMemFrameDC, -1);

	// ����������� ����������� �� hMemFrameDC � hdc
	RECT rect;
	GetClientRect(hwnd, &rect);
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hMemFrameDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDcBall);
}
//////////////////////////////////////////////////////////////////////
