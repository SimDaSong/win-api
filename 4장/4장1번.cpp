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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_2); // ��ũ�� �Լ�. �޴� ID ��� : Ŭ���� ����
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

	switch (iMsg) {
	case WM_CREATE:
		break;
			
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			// ����
		case ID_FILENEW:
			MessageBox(hwnd, _T("�������� ���ڽ��ϱ� ? "), _T("������ ����"), MB_OKCANCEL);
			break;
		case ID_FILEOPEN:
			MessageBox(hwnd, _T("���� ������ ���ڽ��ϱ�?"), _T("���� ���� ����"), MB_OKCANCEL);
			break;
		case ID_FILESAVE:
			MessageBox(hwnd, _T("������ �����Ͻðڽ��ϱ�?"), _T("���� ���� ����"), MB_OKCANCEL);
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd, _T("������ �����ϰ� �����ڽ��ϱ� ?"),
				_T("������ ����"), MB_YESNOCANCEL);
			if (answer == IDYES)
				PostQuitMessage(0);
			break;

			// ����
		case ID_EDITUNDO:
			answer = MessageBox(hwnd, _T("����Ͻðڽ��ϱ�??"), _T("��� ���� ����"), MB_OKCANCEL);
			
			if (answer == IDYES)
				PostQuitMessage(0);
			break;
		case ID_EDITCOPY:
			answer = MessageBox(hwnd, _T("���� �Ͻðڽ��ϱ�?"), _T("���� ���� ����"), MB_OKCANCEL);

			if (answer == IDYES)
				PostQuitMessage(0);
			break;
		case ID_EDITPASTE:
			MessageBox(hwnd, _T("�ٿ��ֱ� �Ͻðڽ��ϱ�?"), _T("�ٿ��ֱ� ���� ����"), MB_OKCANCEL);
			break;

			// ����
		case ID_INFORM:
			MessageBox(hwnd, _T("���α׷� ������ ����ðڽ��ϱ�?"), _T("���α׷� ���� ����"), MB_OKCANCEL);
			break;
		case ID_FONTDLG:
			MessageBox(hwnd, _T("��Ʈ ������ �Ͻðڽ��ϱ�?"), _T("��Ʈ ����"), MB_OKCANCEL);
			break;
		case ID_CHODLG:
			MessageBox(hwnd, _T("���� ������ �Ͻðڽ��ϱ�?"), _T("���� ����"), MB_OKCANCEL);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}