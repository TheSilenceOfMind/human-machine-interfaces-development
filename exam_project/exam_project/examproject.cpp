// examproject.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "examproject.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <atlbase.h>
#include "KWnd.h"
#include "resource.h"
#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int getIDRegion(int x, int y, BYTE *maskBits, int width);
void debug(int val);

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
		WS_POPUP,
		nullptr
	);
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//====================================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HBITMAP hBmpPlayer;
	static HDC hDC;
	static RECT rect;
	static POINT MousePnt, curPoint; // used to get coordinates of the mouse
	static unsigned char *pMaskBits; // used in getIDRegion function
	static BITMAP bmMask;
	static BITMAP bm;
	static HRGN colorReg = 0;
	const static int PERIOD = 100;  // (ms) for color change 
#define NUM 3
	const char* musicList[NUM] = { "D:\\Chuck_Berry_-_Johnny_B_Goode.wav", "D:\\Rammstein_Mutter.wav", "D:\\Carlos_Cipa-Lie_with_Me.wav" };
	PAINTSTRUCT ps;
	static int cntList = 0;
	static HDC hMemBmpDcPlayer;
	static bool isPlaying = false;
	HDC dc;

	static int x, y;
	static int b = 0;

	if (pMaskBits == nullptr) {
		// init bits of the mask
		HBITMAP hMask = (HBITMAP)LoadImage(NULL, L"D:\\mask_ex.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hMask == NULL) {
			MessageBox(nullptr, L"Error! Loading Image", NULL, NULL);
		}
		GetObject(hMask, sizeof(bmMask), &bmMask);
		pMaskBits = (BYTE *)bmMask.bmBits;

		// init dynamic color region
		int pixel;
		int xStart = -1;
		for (int i = 0; i < bmMask.bmHeight; i++) {
			for (int j = 0; j < bmMask.bmWidth; j++) {
				pixel = pMaskBits[i*bmMask.bmWidthBytes + j];
				if (pixel == 7 || pixel == 207) {
					if (xStart == -1) xStart = j;
				} else {
					if (xStart != -1) {
						if (colorReg == 0)
							colorReg = CreateRectRgn(xStart, bmMask.bmHeight - i - 1, j, bmMask.bmHeight - i);/*HRGN CreateRectRgn(
																								  _In_ int nLeftRect,
																								  _In_ int nTopRect,
																								  _In_ int nRightRect,
																								  _In_ int nBottomRect
																								  );*/
						else
							CombineRgn(colorReg, colorReg, CreateRectRgn(xStart,
								bmMask.bmHeight - i - 1, j, bmMask.bmHeight - i), RGN_OR);
						xStart = -1;
					}
				}

			}
		}
	}
		
	switch (message) {
	case WM_CREATE:
		// init all vars
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hBmpPlayer = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBmpPlayer, sizeof(bm), (LPSTR)&bm);
		break;

	case WM_PAINT:
		//debug(b++);
		dc = BeginPaint(hWnd, &ps);
		hMemBmpDcPlayer = CreateCompatibleDC(dc);
		SelectObject(hMemBmpDcPlayer, hBmpPlayer);
		BitBlt(dc, 0, 0, bm.bmWidth, bm.bmHeight,
			hMemBmpDcPlayer, 0, 0, SRCCOPY);
		DeleteObject(hMemBmpDcPlayer);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		GetCursorPos(&MousePnt);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		switch (getIDRegion(x, y, pMaskBits, bm.bmWidth)) {
		// move window
		case 0:	
			dragging = true;
			SetCapture(hWnd);
			break;

		// red button - close 
		case 1: 
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;

		// green or right - next music
		case 2:
			if (isPlaying) {
				USES_CONVERSION;
				cntList = (cntList + 1) % NUM;
				PlaySound(NULL, NULL, SND_ASYNC);
				LPCWSTR w = A2W(musicList[cntList]);
				PlaySound(w, NULL, SND_ASYNC | SND_LOOP);
			}
			break;

		// blue - play/stop
		case 3:
			if (isPlaying) {
				PlaySound(NULL, NULL, SND_ASYNC);
				isPlaying = false;
				KillTimer(hWnd, 1);
				InvalidateRgn(hWnd, colorReg, TRUE);
			} else {
				USES_CONVERSION;
				LPCWSTR w = A2W(musicList[cntList]);
				PlaySound(w, NULL, SND_ASYNC | SND_LOOP);
				isPlaying = true;
				SetTimer(hWnd, 1, PERIOD, NULL);
			}
			break;

		// left - previous music
		case 4:
			if (isPlaying) {
				if (cntList == 0)
					cntList = NUM - 1;
				else
					cntList -= 1;
					
				USES_CONVERSION;
				PlaySound(NULL, NULL, SND_ASYNC);
				LPCWSTR w = A2W(musicList[cntList]);
				PlaySound(w, NULL, SND_ASYNC | SND_LOOP);
			}
			break;

		default:
			MessageBox(NULL, L"Error", L"Error", 0);
			break;
		}
		break;

	case WM_TIMER :
		FillRgn(hDC, colorReg, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
		break;

	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if (getIDRegion(x, y, pMaskBits, bm.bmWidth) != 0) {
			SetCursor(LoadCursor((HINSTANCE)GetWindowLong(hWnd, GWL_EXSTYLE), MAKEINTRESOURCE(IDC_HAND)));
		} else {
			SetCursor(LoadCursor((HINSTANCE)GetWindowLong(hWnd, GWL_EXSTYLE), MAKEINTRESOURCE(IDC_ARROW)));
		}

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
	return 0;
}

int getIDRegion(int x, int y, unsigned char* mask, int width) {
	int ans;
	unsigned char val = mask[(width-y) * width + x];

	debug(val);

	switch (val) {
	case 249:
		ans = 1;
		break;
	case 250:
		ans = 2;
		break;
	case 252:
		ans = 3;
		break;
	case 226:
		ans = 4;
		break;
	default:
		ans = 0;
		break;
	}
	return ans;
}

void debug(int val) {
	wchar_t text_buffer[20] = { 0 }; // temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"%d\n", (int)val); // convert
	OutputDebugString(text_buffer); // print
}