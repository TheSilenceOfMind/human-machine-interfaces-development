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
) {
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

	BITMAP bm;
	HBITMAP hMask = (HBITMAP)LoadImage(NULL, L"D:\\mask_ex_notActive.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hMask == NULL) {
		MessageBox(nullptr, L"Error! Loading Image", NULL, NULL);
	}
	GetObject(hMask, sizeof(bm), &bm);
	BYTE *pMaskBits = (BYTE *)bm.bmBits;
	int pixel;

	int xStart = -1;
	HRGN hRgn = 0;
	for (int i = 0; i < bm.bmHeight; i++) {
		for (int j = 0; j < bm.bmWidth; j++) {
			pixel = pMaskBits[i*bm.bmWidthBytes + j];
			if (!(pixel == 255 )) {
				if (xStart == -1) xStart = j;
			} else {
				if (xStart != -1) {
					if (hRgn == 0)
						hRgn = CreateRectRgn(xStart, bm.bmHeight - i - 1, j, bm.bmHeight - i);/*HRGN CreateRectRgn(
																							  _In_ int nLeftRect,
																							  _In_ int nTopRect,
																							  _In_ int nRightRect,
																							  _In_ int nBottomRect
																							  );*/
					else
						CombineRgn(hRgn, hRgn, CreateRectRgn(xStart,
							bm.bmHeight - i - 1, j, bm.bmHeight - i), RGN_OR);
					xStart = -1;
				}
			}

		}
	}
	SetWindowRgn(hWnd, hRgn, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}