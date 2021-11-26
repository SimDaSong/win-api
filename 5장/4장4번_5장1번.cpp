#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX WndClass;
	HACCEL hAcc;
	hAcc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // 매크로 함수. 메뉴 ID 등록 : 클래스 파일
	WndClass.lpszClassName = _T("Window Class Name");
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		1000,
		700,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAcc, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	// 커맨드 메세지
	int answer;

	// 메뉴 활성/비활성
	static HMENU hMenu, hSubMenu;

	// 원 10개
	static int centerX[10], centerY[10];
	static int circleCount = 0;
	static int mX, mY;
	static bool isClick = false;
	static bool isCopy = false;
	static bool inCircle = false;

	switch (iMsg) {
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		centerX[0] = 20;
		centerY[0] = 20;
		hSubMenu = GetSubMenu(hMenu, 1);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT: // 뒤로 와야함!
		hdc = BeginPaint(hwnd, &ps);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, isClick ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, isCopy ? MF_ENABLED : MF_GRAYED);
		for (int i = 0; i <= circleCount; i++)
			Ellipse(hdc, centerX[i] - 20, centerY[i] - 20, centerX[i] + 20, centerY[i] + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDITUNDO:
			if (circleCount > 0) circleCount--;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_EDITCOPY:
			isCopy = true;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_EDITPASTE:
			isCopy = false;
			isClick = false;
			if (circleCount < 9)
				circleCount++;
			centerX[circleCount] = centerX[circleCount - 1] + 50;
			centerY[circleCount] = centerY[circleCount - 1] + 50;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		for (int i = 0; i <= circleCount; i++)
		{
			if (pow(mX - centerX[i], 2) + pow(mY - centerY[i], 2) < 400)
			{
				isClick = true;
				break;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}