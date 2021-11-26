#include <windows.h>
#include <tchar.h>
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
	WndClass.lpszMenuName = NULL;
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
	static TCHAR str[100]; // 2차원 배열 만들 필요 없이 개행될 때마다 str[0]에 NULL 넣어주면 됨
	static int count, line, numOfChar;
	static SIZE size;
	RECT rt = { 0,0,1000,1000 };

	switch (iMsg) {
	case WM_CREATE:
		count = line = numOfChar = 0;
		break;
		// 1차적으로 문자열을 출력	
	case WM_CHAR:
		// 한 줄은 최대 99자, 99자 넘어가면 개행 되도록, 최대 10라인

		if (wParam == VK_BACK && count > 0) { // numofchar가 아닌 count
			count--;
			if (numOfChar > 0) numOfChar--;// 백스페이스 인식 // 왜 !numofchar는 안되고 >0은 되는지
			if (str[count] == '\n') {
				line--;
				count--;
			}
		}
		else if (wParam == VK_BACK) break;
		else {
			if ((wParam == VK_RETURN) || numOfChar >= 10) { // 개행
				str[count++] = '\n';
				if (++line == 10) exit(1);
				if (numOfChar >= 9) str[count++] = wParam;
				numOfChar = 1; // 0이 아니라 1!!
			}
			else {
				str[count++] = wParam;  		// 문자 저장
				numOfChar++;
			}
		}
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE); 	// WM_PAINT 메시지 발생, TRUE를 FALSE로!
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, str, _tcslen(str), &rt, DT_TOP | DT_LEFT);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}