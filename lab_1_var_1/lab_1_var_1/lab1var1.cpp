// lab1var1.cpp : Defines the entry point for the application.
//
// TODO: add scrolls to the window with image

#include "stdafx.h"
#include "lab1var1.h"
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <windows.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT rect;										// it's a structure //see msdn
long long start, end;
long long  bitblt_diff[10];
long long  pixel_diff[10];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1VAR1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1VAR1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1VAR1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1VAR1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 
	   0, 
	   CW_USEDEFAULT, 
	   0, 
	   nullptr, 
	   nullptr, 
	   hInstance, 
	   nullptr
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);
   UpdateWindow(hWnd);

   return TRUE;
}

long long milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	} else {
		return GetTickCount();
	}
}

bool HDCToFile(const char* FilePath, HDC Context, BITMAP BMP, uint16_t BitsPerPixel = 24) {
	uint32_t Width = BMP.bmWidth;
	uint32_t Height = BMP.bmHeight;

	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = Width;
	Info.bmiHeader.biHeight = Height;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = BitsPerPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	char* Pixels = NULL;
	HDC MemDC = CreateCompatibleDC(Context);
	HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
	HGDIOBJ obj = SelectObject(MemDC, Section);
	BitBlt(MemDC, 0, 0, Width, Height, 
		Context, 0, 0, SRCCOPY);
	DeleteDC(MemDC);
	DeleteObject(obj);
	std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
	if (hFile.is_open()) {
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
		hFile.close();
		DeleteObject(Section);
		return true;
	}
	DeleteObject(Section);
	return false;
}

bool LoadAndBlitBitmap(HWND hWnd, HDC hWinDC) {
	HBITMAP hBMP = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	if (!hBMP) {
		MessageBox(NULL, L"LoadBitmap Failed", L"Error", MB_OK);
		return false;
	}
	HDC memDC = CreateCompatibleDC(hWinDC);
	if (!memDC) {
		MessageBox(NULL, L"CreateCompatibleDC (memDC) Failed", L"Error", MB_OK);
		return false;
	}
	if (!SelectObject(memDC, hBMP)) {
		MessageBox(NULL, L"SelectObject Failed", L"Error", MB_OK);
		return false;
	}

	// get object BITMAP to know the size of image
	BITMAP BMP;
	GetObject(hBMP, sizeof(BITMAP), &BMP);

	// set brush and use it as a filter in bitblt
	HGDIOBJ original = NULL;
	SelectObject(hWinDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hWinDC, RGB(255, 255, 0));
	/*
	for (int i = 0; i < 10; i++) {
		int tmpScale = (i + 1) * 200;
		start = milliseconds_now();
		BOOL res_bitblt = BitBlt(
			hWinDC, 0, 0, tmpScale, tmpScale,
			memDC, 0, 0, MERGECOPY
		);
		end = milliseconds_now();
		bitblt_diff[i] = end - start;

		COLORREF filter = RGB(255, 255, 0);
		start = milliseconds_now();
		for (int x = 0; x < tmpScale; x++) {
			for (int y = 0; y < tmpScale; y++) {
				COLORREF p = GetPixel(memDC, x, y);
				SetPixel(hWinDC, x, y, p & filter);
			}
		}
		end = milliseconds_now();
		pixel_diff[i] = end - start;

		wchar_t text_buffer[40] = { 0 };
		swprintf(text_buffer, _countof(text_buffer), L"%lld\t%lld\n", bitblt_diff[i], pixel_diff[i]);
		OutputDebugString(text_buffer);
	}
	*/
/*
	BOOL res_bitblt = BitBlt(
		hWinDC, 0, 0, BMP.bmWidth, BMP.bmHeight,
		memDC, 0, 0, SRCCOPY
	);

	Sleep(2000);*/

	// it's more useful to invoke stretch_method instead simple bitblt
	// cause we can see whole picture in the window
	SetStretchBltMode(hWinDC, COLORONCOLOR);
	BOOL res_bitblt = StretchBlt(
		hWinDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		memDC, 0, 0, BMP.bmWidth, BMP.bmHeight,
		MERGECOPY
	);
	
/*
	res_bitblt = BitBlt(
		hWinDC, 0, 0, BMP.bmWidth, BMP.bmHeight,
		memDC, 0, 0, MERGECOPY
	);
	*/
	SelectObject(memDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(memDC, RGB(255, 255, 0));
	res_bitblt = BitBlt(
		memDC, 0, 0, BMP.bmWidth, BMP.bmHeight,
		memDC, 0, 0, MERGECOPY
	);

	if (!res_bitblt) {
		MessageBox(NULL, L"BitBlt Failed", L"Error", MB_OK);
		return false;
	}

	HDCToFile("newImage.bmp", memDC, BMP);
	DeleteDC(memDC);
	DeleteObject(hBMP);
	return true;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			GetWindowRect(hWnd, &rect);
			LoadAndBlitBitmap(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
