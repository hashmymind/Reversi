#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <stack>
#include "Reversi.h"

#define CYAN "\x1b[46m"
#define AICOLOR "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

using namespace std;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

const string uBLACK = "●", uWHITE = "○", uEMPTY = "－", uPLAYABLE = "＊";
const int depth = 4, delayTime = 1000;
Reversi reversi;
bool key_reg[16] = { 0 };
bool isAIMode = false;
void paintPanel();
pair<int, int> aiMove();

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

pair<int, int> aiMove() {
	Reversi tmp;
	tmp = reversi;
	reversi.isEnd();// for playable
	int value = -inf;
	pair<int, int> move;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (reversi.getBW(x, y) == reversi.ePLAYABLE) {
				tmp.setBW(x, y);
				int itmp = minmax(tmp, depth);
				if (itmp > value) {
					value = itmp;
					move.first = x;
					move.second = y;
				}
				tmp = reversi;
			}
		}
	}
	return move;
}

void KeyDownEvent( WPARAM wParam )
{
	//==== 英文字母或數字 ====//
	if (wParam >= '1' &&wParam <= '8') {
		key_reg[wParam - '1'] = true;
	}
	else if (wParam >= 'A'&&wParam <= 'H') {
		key_reg[wParam - 'A' + 8] = true;
	}
	else if (wParam >= 'a' && wParam <= 'h') {
		key_reg[wParam - 'a' + 8] = true;
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
		reversi.isEnd();// for hint
	}
	else if (wParam == VK_SPACE) {
		if (!isAIMode) {
			// need to push in past
			isAIMode = true;
			//
			reversi.init();
			//ai is black
			//下第一步
			pair<int, int> move = aiMove();
			past.push(reversi);
			reversi.setBW(move.first, move.second);
			reversi.isEnd();
		}
		else {
			isAIMode = false;
			reversi.init();
		}

	}
	else if(wParam >= '1' &&wParam <= '8') {
		key_reg[wParam - '1'] = false;
	}
	else if (wParam >= 'A'&&wParam <= 'Z') {
		key_reg[wParam - 'A' + 8] = false;
	}
	else if (wParam >= 'a' && wParam <= 'z') {
		key_reg[wParam - 'a' + 8] = false;
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
	reversi.isEnd();
	MSG msg = { };
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (isAIMode && reversi.isBW()) {
			//ai move
			pair<int, int> move = aiMove();
			past.push(reversi);
			cout << "AI thinking . . ." << endl;
			cout.flush();
			reversi.setBW(move.first, move.second);
		}
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
			if (key_reg[x + 7] || key_reg[y - 1])
				cout << CYAN;
			char state = reversi.getBW(x, y);
			if (state == reversi.eBLACK) {
				cout << uBLACK;
			}
			else if (state == reversi.eWHITE) {
				cout << uWHITE;
			}
			else if (state == reversi.ePLAYABLE) {
				cout << uPLAYABLE;
			}
			else {
				cout << uEMPTY;
			}
			if (key_reg[x + 7] || key_reg[y - 1])
				cout << COLOR_RESET;
		}
		cout << (key_reg[7+x] ? "←" : "　") << endl;
	}
	cout << "　";
	for (int i = 0; i < 8; ++i){
		cout << (key_reg[i]?"↑":"　");
		}
	cout << endl;
	cout << "Ｎｏｗ：" << (reversi.isBW() ? uBLACK : uWHITE) << endl;
	cout << uBLACK << "： " << reversi.BCount() << "　" << uWHITE << "： " << reversi.WCount() << endl;
	cout << "ＵＮＤＯ：ＢＡＣＫＳＰＡＣＥ　　ＲＥＤＯ：ＴＡＢ\nAI MODE（ｏｎ＼ｏｆｆ）：SPACE　　　ＲＥＳＥＴ：ＥＮＴＥＲ" << endl;
	if (isAIMode) cout << AICOLOR << "ＡＩ　Ｍｏｄｅ　ｉｓ　ｏｎ！" << COLOR_RESET << endl;
	if (reversi.isEnd()) {
		int result = reversi.BCount() - reversi.WCount();
		if (result !=0){
			cout << "Ｗｉｎｎｅｒ　ｉｓ　" << (result > 0 ? uBLACK : uWHITE);
		}
		else {
			cout << "Ｔｈｅ　ｇａｍｅ　ｗａｓ　ｄｒａｗｎ" << endl;
		}
	}
}
