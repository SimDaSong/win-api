#define _CRT_SECURE_NO_WARNINGS
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	// 메모장
	static TCHAR str[100]; // char 말고!
	static int count;
	RECT rt = { 0,0,500,500 };

	// 커맨드 메세지
	int answer;

	// 파일 저장하기
	OPENFILENAME OFN, SFN;
	TCHAR fStr[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0");
	FILE *fPtr;
	TCHAR buffer[500];

	switch (iMsg) {
	case WM_CREATE:
		count = 0;
		break;
	case WM_CHAR:
		if (wParam == VK_BACK && count > 0) count--;
		else str[count++] = wParam;
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT: // 뒤로 와야함!
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, str, _tcslen(str), &rt, DT_TOP | DT_LEFT);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILEOPEN:
			break;
		case ID_FILESAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0) {
				_stprintf_s(fStr, _T("%s 파일로 저장하시겠습니까?"), SFN.lpstrFile);
				answer = MessageBox(hwnd, fStr, _T("저장하기 선택"), MB_OK);
				if (answer == IDOK) {
					_tfopen_s(&fPtr, SFN.lpstrFile, _T("wt"));
					if (fPtr == NULL)
					{
						MessageBox(hwnd, _T("파일저장에러"), _T("파일저장에러"), MB_OK);
						break;
					}
					_tcscpy(buffer,str);
					for (int i = 0; i < 256; i++)
					{
						_fputts(&buffer[i], fPtr);
						if (feof(fPtr) != 0)
							break;
					}
					fclose(fPtr);
				}
			}
			break;
		case ID_EDITUNDO:
			break;
		case ID_EDITCOPY:
			break;
		case ID_EDITPASTE:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}