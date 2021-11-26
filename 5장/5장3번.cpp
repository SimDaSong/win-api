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
		800,
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
	static HBITMAP hBit[100], oldBit[100];
	static RECT rectView;

	switch (iMsg)
	{
	case WM_CREATE:
		for(int i =0;i<100;i++)
			hBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectView);
		memdc = CreateCompatibleDC(hdc);
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				oldBit[j] = (HBITMAP)SelectObject(memdc, hBit[j]);
				StretchBlt(hdc, j * 100, i * 80, rectView.right, rectView.bottom, memdc, 0, 0, 2000, 2000, SRCCOPY);
				SelectObject(memdc, oldBit[j]);
			}
		}
		EndPaint(hwnd, &ps);
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
