#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM IParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX WndClass;

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
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		500,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	int answer;

	// 색상 대화상자
	CHOOSECOLOR COLOR;
	static COLORREF tmp1[16], tmp2[16], color1, color2;
	HBRUSH hBrush, OldBrush;
	HPEN hPen, oldPen;
	int i;

	// 드래그
	static int startX, startY, oldX, oldY;
	static BOOL Drag;
	int endX, endY;
	static HMENU hMenu, hSubMenu;
	static int LSelect, ESelect, RSelect, LC, FC;

	switch (iMsg) {
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 0);
		LC = FC = FALSE;
		Drag = FALSE;
		RSelect = ESelect = LSelect = FALSE;
		color1 = RGB(255,255,255);
		color2 = RGB(0,0,0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			// 그리기 객체
		case ID_LINE:
			LSelect = TRUE;
			ESelect = RSelect = FALSE;
			break;
		case ID_ELLIPSE:
			ESelect = TRUE;
			LSelect = RSelect = FALSE;
			break;
		case ID_RECTANGLE:
			RSelect = TRUE;
			LSelect = ESelect = FALSE;
			break;

			// 색상
		case ID_PENCOLOR:
			for (i = 0; i < 16; i++)
				tmp2[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp2;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
				color2 = COLOR.rgbResult;
			break;
		case ID_FACECOLOR:
			for (i = 0; i < 16; i++)
				tmp1[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp1;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
				color1 = COLOR.rgbResult;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		oldX = startX; oldY = startY;
		Drag = TRUE;
		break;
	case WM_LBUTTONUP:
		Drag = FALSE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag) {
			SetROP2(hdc, R2_NOTXORPEN);
			SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_PEN));
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			hPen = CreatePen(PS_SOLID, 1, color2);
			oldPen = (HPEN)SelectObject(hdc, hPen);

			if (LSelect == TRUE) {
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, oldX, oldY);
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, endX, endY);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			if (ESelect == TRUE) {
				hBrush = CreateSolidBrush(color1);
				OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Ellipse(hdc, startX, startY, oldX, oldY);
				Ellipse(hdc, startX, startY, endX, endY);
				SelectObject(hdc, OldBrush);
				DeleteObject(hBrush);
			}
			if (RSelect == TRUE) {
				hBrush = CreateSolidBrush(color1);
				OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, startX, startY, oldX, oldY);
				Rectangle(hdc, startX, startY, endX, endY);
				SelectObject(hdc, OldBrush);
				DeleteObject(hBrush);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			oldX = endX; oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}