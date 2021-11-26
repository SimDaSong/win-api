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
	static TCHAR str[100]; // 2���� �迭 ���� �ʿ� ���� ����� ������ str[0]�� NULL �־��ָ� ��
	static int count, line, numOfChar;
	static SIZE size;
	RECT rt = { 0,0,1000,1000 };

	switch (iMsg) {
	case WM_CREATE:
		count = line = numOfChar = 0;
		break;
		// 1�������� ���ڿ��� ���	
	case WM_CHAR:
		// �� ���� �ִ� 99��, 99�� �Ѿ�� ���� �ǵ���, �ִ� 10����

		if (wParam == VK_BACK && count > 0) { // numofchar�� �ƴ� count
			count--;
			if (numOfChar > 0) numOfChar--;// �齺���̽� �ν� // �� !numofchar�� �ȵǰ� >0�� �Ǵ���
			if (str[count] == '\n') {
				line--;
				count--;
			}
		}
		else if (wParam == VK_BACK) break;
		else {
			if ((wParam == VK_RETURN) || numOfChar >= 10) { // ����
				str[count++] = '\n';
				if (++line == 10) exit(1);
				if (numOfChar >= 9) str[count++] = wParam;
				numOfChar = 1; // 0�� �ƴ϶� 1!!
			}
			else {
				str[count++] = wParam;  		// ���� ����
				numOfChar++;
			}
		}
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE); 	// WM_PAINT �޽��� �߻�, TRUE�� FALSE��!
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