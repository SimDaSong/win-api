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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_2); // 매크로 함수. 메뉴 ID 등록 : 클래스 파일
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

	// 열기 대화상자 이용하기
	OPENFILENAME OFN;
	TCHAR str[100], lpstrFile[100] = _T("");
	TCHAR filter[] = _T("Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0");

	// 파일 읽기
	static int line = 0;
	FILE *fp;
	TCHAR buffer[500];

	switch (iMsg) {
	case WM_CREATE:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			// 파일
		case ID_FILENEW:
			MessageBox(hwnd, _T("새파일을 열겠습니까 ? "), _T("새파일 선택"), MB_OKCANCEL);
			break;
		case ID_FILEOPEN:

			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 열겠습니까?"), OFN.lpstrFile);
				answer = MessageBox(hwnd, str, _T("열기 선택"), MB_OKCANCEL);

				if (answer == IDOK) {
					hdc = GetDC(hwnd);

					// 파일 열기
#ifdef _UNICODE   	// 문자집합이 유니코드일 때
					_tfopen_s(&fp, OFN.lpstrFile, _T("r, ccs = UNICODE"));
#else   		// 문자집합이 멀티바이트일 때
					_tfopen_s(&fp, OFN.lpstrFile, _T("r"));
#endif
					while (_fgetts(buffer, 100, fp) != NULL) {
						if (buffer[_tcslen(buffer) - 1] == _T('\n'))
							buffer[_tcslen(buffer) - 1] = NULL;
						TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
						line++;
					}
					fclose(fp);
					ReleaseDC(hwnd, hdc);
				}
			}
			break;
		case ID_FILESAVE:
			MessageBox(hwnd, _T("파일을 저장하시겠습니까?"), _T("파일 저장 선택"), MB_OKCANCEL);
			break;
		case ID_EXIT:
			answer = MessageBox(hwnd, _T("파일을 저장하고 끝내겠습니까 ?"),
				_T("끝내기 선택"), MB_YESNOCANCEL);
			if (answer == IDYES)
				PostQuitMessage(0);
			break;

			// 편집
		case ID_EDITUNDO:
			answer = MessageBox(hwnd, _T("취소하시겠습니까??"), _T("취소 여부 선택"), MB_OKCANCEL);

			if (answer == IDYES)
				PostQuitMessage(0);
			break;
		case ID_EDITCOPY:
			answer = MessageBox(hwnd, _T("복사 하시겠습니까?"), _T("복사 여부 선택"), MB_OKCANCEL);

			if (answer == IDYES)
				PostQuitMessage(0);
			break;
		case ID_EDITPASTE:
			MessageBox(hwnd, _T("붙여넣기 하시겠습니까?"), _T("붙여넣기 여부 선택"), MB_OKCANCEL);
			break;

			// 도움말
		case ID_INFORM:
			MessageBox(hwnd, _T("프로그램 정보를 열어보시겠습니까?"), _T("프로그램 정보 선택"), MB_OKCANCEL);
			break;
		case ID_FONTDLG:
			MessageBox(hwnd, _T("폰트 설정을 하시겠습니까?"), _T("폰트 설정"), MB_OKCANCEL);
			break;
		case ID_CHODLG:
			MessageBox(hwnd, _T("색상 설정을 하시겠습니까?"), _T("색상 설정"), MB_OKCANCEL);
			break;
		}

	case WM_PAINT: // 뒤로 와야함!
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}