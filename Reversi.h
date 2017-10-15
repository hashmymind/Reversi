#pragma once
#include <vector>

//
const float sideVal = 4, csquareVal = 6, xsquareVal = 7, cornerVal = 30;
const int corner[4][2] = { { 1,1 },{ 1,8 },{ 8,1 },{ 8,8 } };
const int xsquare[4][2] = { { 2,2 },{ 2,7 },{ 7,2 },{ 7,7 } };
const int csquare[8][2] = { { 1,2 },{ 1,7 },{ 2,1 },{ 2,8 },{ 7,1 },{ 7,8 },{ 8,2 },{ 8,7 } };
const int inf = 1000000;
//

class Reversi {
private:
	bool bBW; // b 1, w 0
	char mArry[64];
	int steps;
public:
	enum { eEMPTY, eBLACK, eWHITE, ePLAYABLE };
	int numWhite, numBlack;

	void init();
	int convertXY(int x, int y);
	int check(int x, int y);
	int BCount();
	int WCount();
	bool isValid(int x, int y);
	bool isEnd();
	bool isBW();
	void setBW(int x, int y);
	char getBW(int x, int y);
	int getSteps();
	int avaMoving();
	Reversi operator=(Reversi rhs);

	Reversi(void);
	~Reversi(void);
};

float evalue(Reversi);
float minmax(Reversi, int, float, float);