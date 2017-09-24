#ifndef UNICODE
#define UNICODE
#endif 
#define DEBUG 0

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <stack>
#include "Reversi.h"

#define CYAN "\x1b[46m"
#define AICOLOR "\x1b[33m"
#define BANGCOLOR "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const string uBLACK = "●", uWHITE = "○", uEMPTY = "－", uPLAYABLE = "＊";
int depth = 0, delayTime = 1000;
Reversi reversi;
bool key_reg[16] = { 0 };
bool isAIMode = false,aiPlayer = true, aiSetting = false;
bool isHello = true;
void paintPanel();
pair<int, int> aiMove();
int aiVal = 0;
const char helloScreen[30][121] = {
	"",
	"",
	"",
	"",
	"                         _______  _______           _______  _______  _______ _________ ",
	"                        (  ____ )(  ____ \\|\\     /|(  ____ \\(  ____ )(  ____ \\\\__   __/",
	"                        | (    )|| (    \\/| )   ( || (    \\/| (    )|| (    \\/   ) (",
	"                        | (____)|| (__    | |   | || (__    | (____)|| (_____    | |",
	"                        |     __)|  __)   ( (   ) )|  __)   |     __)(_____  )   | |",
	"                        | (\\ (   | (       \\ \\_/ / | (      | (\\ (         ) |   | |",
	"                        | ) \\ \\__| (____/\\  \\   /  | (____/\\| ) \\ \\__/\\____) |___) (___",
	"                        |/   \\__/(_______/   \\_/   (_______/|/   \\__/\\_______)\\_______/",
	"                                                         B10515009ChengYao(Ver 1.0 9/24/2017)",
	"",
	"",
	"",
	"",
	"",
	"                                           [Press shift to play]"
};

char aiSettingScreen[30][121] = {
	"          _       _           ____                    ",
	"    /\\   | |     | |         |  _ \\                   ",
	"   /  \\  | |_ __ | |__   __ _| |_) | __ _ _ __   __ _ ",
	"  / /\\ \\ | | '_ \\| '_ \\ / _` |  _ < / _` | '_ \\ / _` |",
	" / ____ \\| | |_) | | | | (_| | |_) | (_| | | | | (_| |",
	"/_/    \_\\_| .__/|_| |_|\\__,_|____/ \\__,_|_| |_|\\__, |",
	"           | |                                   __/ |",
	"           |_|                                  |___/ ",
	"                     Select Level",
	"",
	"                      (1)Easy",
	"                      (2)Middle",
	"                      (3)Hard(not that hard)",
	"                     Select COLOR",
	"",
	"                      (1)●",
	"                      (2)○"
};

stack<Reversi> past, future;

void keyinput() {
	int x = -1, y = -1;
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
	float value = aiPlayer?-inf:inf;
	pair<int, int> move;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (reversi.getBW(x, y) == reversi.ePLAYABLE) {
				tmp.setBW(x, y);
				float itmp = minmax(tmp, depth, -inf, inf);
				if (aiPlayer) {
					if (itmp > value) {
						value = itmp;
						move.first = x;
						move.second = y;
					}
				}
				else {
					if (itmp < value) {
						value = itmp;
						move.first = x;
						move.second = y;
					}
				}
				tmp = reversi;
			}
		}
	}
	aiVal = value;
	return move;
}

void KeyDownEvent(WPARAM wParam)
{
	//==== 英文字母或數字 ====//
	if (wParam >= '1' &&wParam <= '8') {
		key_reg[wParam - '1' +8] = true;
	}
	else if (wParam >= 'A'&&wParam <= 'H') {
		key_reg[wParam - 'A'] = true;
	}
	else if (wParam >= 'a' && wParam <= 'h') {
		key_reg[wParam - 'a'] = true;
	}

	keyinput();
}

void KeyUpEvent(WPARAM wParam)
{
	if (isHello && wParam != VK_SHIFT) {
		return;
	}
	//==== Tab、Enter、Ctrl... ====//
	if (wParam == VK_TAB) {
		// redo
		if (isAIMode) return;
		if (!future.empty()) {
			past.push(reversi);
			reversi = future.top();
			future.pop();
		}

	}
	else if (wParam == VK_BACK) {
		// undo
		if (isAIMode && !past.empty()) {
			past.pop();
			if (!past.empty()) {
				future.push(reversi);
				reversi = past.top();
				past.pop();
			}
		}
		else if (!past.empty()) {
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
		if (!isAIMode && ! aiSetting) {
			aiSetting = true;
			reversi.init();
			//isAIMode = true;
			//
			//ai is black
		}
		else if (isAIMode) {
			isAIMode = false;
			depth = 0;
		}
		else {
			aiSetting = false;
			depth = 0;
		}

	}
	else if (wParam == VK_SHIFT) {
		isHello = false;
	}
	else if (wParam >= '1' &&wParam <= '8') {
		key_reg[wParam - '1' + 8] = false;
		if (aiSetting) {
			if (!depth) {
				if (wParam < '4') {
					depth = 5 + wParam - '1';
				}
			}
			else {
				if (wParam == '1') {
					aiPlayer = true;
				}
				else if (wParam == '2'){
					aiPlayer = false;
				}
				isAIMode = true;
				aiSetting = false;
			}
		}
	}
	else if (wParam >= 'A'&&wParam <= 'H') {
		key_reg[wParam - 'A'] = false;
	}
	else if (wParam >= 'a' && wParam <= 'h') {
		key_reg[wParam - 'a'] = false;
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//==== 註冊視窗類別 ====//
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

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
	freopen("CONOUT$", "wb", stdout);

	ShowWindow(hwnd, nCmdShow);

	//==== 執行訊息迴圈 ====//
	reversi.init();
	reversi.isEnd();
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		system("CLS");
		paintPanel();
		if (isAIMode && reversi.isBW() == aiPlayer) {
			//ai move
			cout << "AI thinking . . ." << endl;
			cout.flush();
			pair<int, int> move = aiMove();
			past.push(reversi);
			reversi.setBW(move.first, move.second);
			system("CLS");
			paintPanel();
		}
		//SetForegroundWindow(hwnd);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;

	case WM_KEYDOWN:
		KeyDownEvent(wParam);
		break;

	case WM_KEYUP:
		KeyUpEvent(wParam);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void paintPanel() {
	bool isEnd = reversi.isEnd();
	if (isHello) {
		for (int i = 0; i <= 18; ++i) {
			cout << helloScreen[i] << endl;
		}
		return;
	}
	else if (aiSetting) {
		cout << BANGCOLOR;
		for (int i = 0; i < 8; ++i) {
			cout << aiSettingScreen[i] << endl;
		}
		cout << COLOR_RESET;
		for (int i = 8; i < 13 &&!depth; ++i) {
			cout << aiSettingScreen[i] << endl;
		}
		for (int i = 13; i < 17 && depth; ++i) {
			cout << aiSettingScreen[i] << endl;
		}
		
		return;
	}

	string coordinate = "ａｂｃｄｅｆｇｈ１２３４５６７８";// 2 char wide for each
	cout << "　" << coordinate.substr(0, 16) << endl;//1~8
	for (int x = 1; x <= 8; ++x) {
		cout << coordinate.substr(16 + 2 * (x - 1), 2);
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
		cout << (key_reg[7 + x] ? "←" : "　") << endl;
	}
	cout << "　";
	for (int i = 0; i < 8; ++i) {
		cout << (key_reg[i] ? "↑" : "　");
	}
	cout << endl;
	cout << "Now: " << (reversi.isBW() ? uBLACK : uWHITE) << endl;
	cout << uBLACK << ": " << reversi.BCount() << "  " << uWHITE << ": " << reversi.WCount() << endl;
	cout << "undo: backspace      redo: tab\nAI Mode(" << (isAIMode ? "off" : "on") << "): space   reset: enter" << endl;
	if (isAIMode) cout << AICOLOR << "AI Mode is on, redo is disable! AI plays " << (aiPlayer? uBLACK:uWHITE ) << COLOR_RESET << endl;
	if (isEnd) {
		int result = reversi.BCount() - reversi.WCount();
		if (result != 0) {
			cout << "Winner is " << (result > 0 ? uBLACK : uWHITE) << "!" << endl;
		}
		else {
			cout << "This game was drawn~" << endl;
		}
	}
	if (DEBUG) cout << AICOLOR << "AI value: " << aiVal << "\nSteps: " << reversi.getSteps() << COLOR_RESET << endl;
}
