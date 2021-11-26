#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1000,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit, oldBit;
	static RECT rectView;
	static int x, y;
	static bool xSel, ySel;

	switch (iMsg)
	{
	case WM_CREATE:
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		x = y = 0;
		xSel = ySel = false;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectView);
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		BitBlt(hdc, x, y, 30, 30, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, oldBit);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		SetTimer(hwnd, 1, 70, NULL);
		break;
	case WM_TIMER:
		if (xSel) {
			x -= 10;
			if (x - 30 < rectView.left) {
				xSel = false;
				x += 10;
			}
		}
		else if (!xSel) {
			x += 10;
			if (x + 30 > rectView.right) {
				xSel = true;
				x -= 10;
			}
		}
		if (ySel) {
			y -= 10;
			if (y - 30 < rectView.top) {
				ySel = false;
				y += 10;
			}
		}
		else if (!ySel) {
			y += 10;
			if (y + 30 > rectView.bottom) {
				ySel = true; y -= 10;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rectView);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
