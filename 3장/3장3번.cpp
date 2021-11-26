#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

	WNDCLASS WndClass;
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
		_T("Window Title Name"), 
		WS_OVERLAPPEDWINDOW,
		0, 
		00,
		500,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x = 20, y = 20;
	static int centerx = 140, centery = 140;
	static RECT rectView;
	static bool isGoing = false;

	switch (iMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rectView); //윈도우 클라이언트 영역을 계산해 RECT변수에 저장
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT && (x + 20) < 600)
			x += 40;
		else if (wParam == VK_LEFT && (x - 20) > 0)
			x -= 40;
		else if (wParam == VK_UP && (y - 20) > 0)
			y -= 40;
		else if (wParam == VK_DOWN && (y + 20) < 600)
			y += 40;
		if (wParam == VK_RETURN) {
			if (isGoing) isGoing = false;
			else isGoing = true;

			if (isGoing)
				SetTimer(hwnd, 1, 70, NULL);
			else
				KillTimer(hwnd, 1);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		if ((x + 20) < 600) x += 10;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 600, 600);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
