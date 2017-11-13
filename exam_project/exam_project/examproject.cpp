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

int cx, cy; // ������� ������
BITMAP bm; // ���� � ������

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
	static FLOAT x = 0, y = 0; // ������� ������� ���� � �����
	static FLOAT dX, dY; // ���������� ��������� ��� ������ �� ����� �������
	static int alpha = 0; // ����, ������������ �������� ����

	static HBITMAP hBmpPlayer; // ����� � ������������ ����
	static HDC hMemDcFrame;   // ����������� �������� (�������� � ������)
	static HBITMAP hBmpFrame; // ����� ��� ������������ ���������
	static HDC hDC;          // �������� �������
	RECT rect;  // ������������� ���������� �������

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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

//====================================================================
void DrawPlayer(HWND hwnd, HDC hdc, HDC hMemFrameDC, HBITMAP hBmp, BITMAP bm, FLOAT x, FLOAT y) {
	// ���������� � ������ ������
	HDC hMemDcBall = CreateCompatibleDC(hdc);
	SelectObject(hMemDcBall, hBmp);
											 // ����� ����
	SaveDC(hMemFrameDC);
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
