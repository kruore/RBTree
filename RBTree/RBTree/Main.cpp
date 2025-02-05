﻿// RBTree.cpp : 애플리케이션에 대한 진입점을 정의합니다.


#include "RedBlackTree.h"
#include "resource.h"
#include "framework.h"
#include <vector>
#include <random>

#define MAX_LOADSTRING 100

HWND hwndInput;
HWND hwndOutput;
HWND hwndInsertButton;
HWND hwndDeleteButton;
std::vector<int> datas;
//테스트용 버튼
HWND hwndTestInsertButton;
HWND hwndTestDeleteButton;


RBTree rbTree;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void randomInsert(RBTree& tree, HWND hwnd) {
	int insertValue = rand() % 100;
	datas.push_back(insertValue);
	rbTree.Insert(insertValue);
	InvalidateRect(hwnd, NULL, TRUE);
}

void randomDelete(RBTree& tree, HWND hwnd) {

	if (datas.size() <= 0)
		return;
	std::random_device rd;
	std::mt19937 gen(rd());

	// 벡터 인덱스 범위에 대한 균등 분포 정의
	std::uniform_int_distribution<> dis(0, datas.size() - 1);

	// 랜덤 인덱스 생성
	int random_index = dis(gen);
	rbTree.remove(datas[random_index]);
	datas.erase(datas.begin() + random_index);
	InvalidateRect(hwnd, NULL, TRUE);
}





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RBTREE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RBTREE));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RBTREE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RBTREE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:

		hwndInput = CreateWindowW(L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			10, 10, 200, 25,
			hWnd, (HMENU)1, NULL, NULL);

		hwndInsertButton = CreateWindowW(L"BUTTON", L"Insert",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			220, 10, 80, 25,
			hWnd, (HMENU)2, NULL, NULL);

		hwndDeleteButton = CreateWindowW(L"BUTTON", L"Delete",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			310, 10, 80, 25,
			hWnd, (HMENU)3, NULL, NULL);

		hwndOutput = CreateWindowW(L"STATIC", L"",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			10, 50, 380, 25,
			hWnd, (HMENU)4, NULL, NULL);

		hwndTestInsertButton = CreateWindowW(L"BUTTON", L"TESTINSERT",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			610, 10, 100, 25,
			hWnd, (HMENU)5, NULL, NULL);
		hwndTestDeleteButton = CreateWindowW(L"BUTTON", L"TESTDELETE",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			810, 10, 100, 25,
			hWnd, (HMENU)6, NULL, NULL);

		return 0;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 5)
		{
			for (int i = 0;i < 100;i++)
			{
				randomInsert(rbTree, hWnd);
			}
		}
		else if (LOWORD(wParam) == 6)
		{
			for (int i = 0;i < 100;i++)
			{
				randomDelete(rbTree, hWnd);
			}
		}
		if (LOWORD(wParam) == 2 || LOWORD(wParam) == 3) {
			wchar_t buffer[256];
			GetWindowTextW(hwndInput, buffer, 256);
			int value = _wtoi(buffer);

			if (LOWORD(wParam) == 2) {
				// Insert 버튼 클릭
				rbTree.Insert(value);
				SetWindowTextW(hwndOutput, L"Value inserted.");
			}
			else if (LOWORD(wParam) == 3) {
				// Delete 버튼 클릭
				rbTree.remove(value);
				SetWindowTextW(hwndOutput, L"Value deleted.");
			}
			// 입력 필드 초기화
			SetWindowTextW(hwndInput, L"");
			InvalidateRect(hWnd, NULL, false);
		}


		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		RECT rect;
		//배경 컬러를 회색으로
		GetClientRect(hWnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));
		//트리 그리기
		FillRect(hdc, &rect, hBrush);
		rbTree.draw(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}