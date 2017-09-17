#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <string>
#include <stack>
#include "Reversi.h"

using namespace std;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

const string uBLACK = "●", uWHITE = "○", uEMPTY = "－";
Reversi reversi;
bool key_reg[16] = { 0 };
void paintPanel();

stack<Reversi> past, future;

void keyinput() {
	int x = -1,y=-1;
	for (int i = 0; i < 8; i++) {
		if (key_reg[i]) {
			y = i + 1;
		}
		if (key_reg[i + 8]) {
			x = i + 1;
		}
	}
	if (x != -1 && y != -1) {
		while (!future.empty()) {
			future.pop();
		}
		past.push(reversi);
		if (reversi.check(x, y)) {
			reversi.setBW(x, y);
		}
	}
}

void KeyDownEvent( WPARAM wParam )
{
	//==== 英文字母或數字 ====//
	switch( wParam )
	{
	case '1':
		key_reg[0] = true;
		break;
	case '2':
		key_reg[1] = true;
		break;
	case '3':
		key_reg[2] = true;
		break;
	case '4':
		key_reg[3] = true;
		break;
	case '5':
		key_reg[4] = true;
		break;
	case '6':
		key_reg[5] = true;
		break;
	case '7':
		key_reg[6] = true;
		break;
	case '8':
		key_reg[7] = true;
		break;
	case 'a':
	case 'A':
		key_reg[8] = true;
		break;
	case 'b':
	case 'B':
		key_reg[9] = true;
		break;
	case 'c':
	case 'C':
		key_reg[10] = true;
		break;
	case 'd':
	case 'D':
		key_reg[11] = true;
		break;
	case 'e':
	case 'E':
		key_reg[12] = true;
		break;
	case 'f':
	case 'F':
		key_reg[13] = true;
		break;
	case 'g':
	case 'G':
		key_reg[14] = true;
		break;
	case 'h':
	case 'H':
		key_reg[15] = true;
		break;
	}
	keyinput();
}

void KeyUpEvent( WPARAM wParam )
{
	//==== Tab、Enter、Ctrl... ====//
	if (wParam == VK_TAB) {
		// redo
		if (!future.empty()) {
			past.push(reversi);
			reversi = future.top();
			future.pop();
		}
		
	}
	else if( wParam == VK_BACK){
		// undo
		if (!past.empty()) {
			future.push(reversi);
			reversi = past.top();
			past.pop();
		}
	}
	else if (wParam == VK_RETURN) {
		reversi.init();
	}

	//==== 英文字母或數字 ====//
	switch( wParam )
	{
	case '1':
		key_reg[0] = false;
		break;
	case '2':
		key_reg[1] = false;
		break;
	case '3':
		key_reg[2] = false;
		break;
	case '4':
		key_reg[3] = false;
		break;
	case '5':
		key_reg[4] = false;
		break;
	case '6':
		key_reg[5] = false;
		break;
	case '7':
		key_reg[6] = false;
		break;
	case '8':
		key_reg[7] = false;
		break;
	case 'a':
	case 'A':
		key_reg[8] = false;
		break;
	case 'b':
	case 'B':
		key_reg[9] = false;
		break;
	case 'c':
	case 'C':
		key_reg[10] = false;
		break;
	case 'd':
	case 'D':
		key_reg[11] = false;
		break;
	case 'e':
	case 'E':
		key_reg[12] = false;
		break;
	case 'f':
	case 'F':
		key_reg[13] = false;
		break;
	case 'g':
	case 'G':
		key_reg[14] = false;
		break;
	case 'h':
	case 'H':
		key_reg[15] = false;
		break;
	}
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow )
{
	//==== 註冊視窗類別 ====//
	const wchar_t CLASS_NAME[]  = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass( &wc );

	//==== 創建視窗 ====//
	HWND hwnd = CreateWindowEx(
		0,                      // Optional window styles.
		CLASS_NAME,             // 視窗類別
		L"Reversi",				// 視窗標題
		WS_OVERLAPPEDWINDOW,    // 視窗風格
		0, 0, 5, 5,				// 視窗大小及位置
		NULL,					// Parent 視窗    
		NULL,					// 選單
		hInstance,				// Instance handle
		NULL					// Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	//==== 創建 console 視窗 ====//
	AllocConsole();
	freopen( "CONOUT$", "wb", stdout );

	ShowWindow( hwnd, nCmdShow );

	//==== 執行訊息迴圈 ====//
	reversi.init();
	MSG msg = { };
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		system("CLS");
		paintPanel();
	}

	return 0;
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );
			FillRect( hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
			EndPaint( hwnd, &ps );
		}
		return 0;

	case WM_KEYDOWN:
		KeyDownEvent( wParam );
		break;

	case WM_KEYUP:
		KeyUpEvent( wParam );
		break;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void paintPanel() {
	string coordinate = "１２３４５６７８ａｂｃｄｅｆｇｈ";// 2 char wide for each
	cout << "　" << coordinate.substr(0, 16) << endl;//1~8
	for (int x = 1; x <= 8; ++x) {
		cout << coordinate.substr(16 + 2*(x-1), 2);
		for (int y = 1; y <= 8; ++y) {
			char state = reversi.getBW(x, y);
			if (state == reversi.eBLACK) {
				cout << uBLACK;
			}
			else if (state == reversi.eWHITE) {
				cout << uWHITE;
			}
			else {
				cout << uEMPTY;
			}
		}
		cout << key_reg[7 + x] << endl;
	}
	cout << "　";
	for (int i = 0; i < 8; ++i)
		cout << " " << key_reg[i];
	cout << endl;
	cout << "Ｎｏｗ：" << (reversi.isBW() ? uBLACK : uWHITE) << endl;
	cout << uBLACK << "： " << reversi.BCount() << "　" << uWHITE << "： " << reversi.WCount() << endl;
	cout << "ＵＮＤＯ：ＢＡＣＫＳＰＡＣＥ　　ＲＥＤＯ：ＴＡＢ　　ＲＥＳＥＴ：ＥＮＴＥＲ" << endl;
	if (reversi.isEnd()) {
		int result = reversi.BCount() - reversi.WCount();
		if (result > 0) {
			cout << "Ｗｉｎｎｅｒ　ｉｓ　" << uBLACK << endl;
		}
		else if (result < 0) {
			cout << "Ｗｉｎｎｅｒ　ｉｓ　" << uWHITE << endl;
		}
		else {
			cout << "Ｔｈｅ　ｇａｍｅ　ｗａｓ　ｄｒａｗｎ" << endl;
		}
	}
}
