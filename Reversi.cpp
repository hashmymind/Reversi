#include <cstring>
#include <vector>
#include <algorithm>
#include "Reversi.h"

using namespace std;

int moveX[] = { -1,-1,-1, 0, 0, 1, 1, 1 };
int moveY[] = { -1, 0, 1,-1, 1,-1, 0, 1 };

Reversi::Reversi(void) {

}

Reversi::~Reversi(void) {

}

void Reversi::init() {
	memset(this->mArry, eEMPTY, sizeof(this->mArry));
	this->mArry[27] = eWHITE;
	this->mArry[28] = eBLACK;
	this->mArry[35] = eBLACK;
	this->mArry[36] = eWHITE;
	this->bBW = true;
	this->steps = 0;
}

int Reversi::convertXY(int x, int y) {
	return (x - 1) * 8 + (y - 1);
}

bool Reversi::isValid(int x, int y) {
	if (x<1 || x>8 || y<1 || y>8)
		return false;
	else
		return true;
}

int Reversi::check(int x, int y) {
	if (isValid(x, y) && this->mArry[this->convertXY(x, y)] != eEMPTY && this->mArry[this->convertXY(x, y)] != ePLAYABLE)
		return 0;
	int eatCount = 0;
	for (int i = 0; i<8; ++i) {
		int newX = x + moveX[i], newY = y + moveY[i];
		int otherCount = 0;
		while (isValid(newX, newY)) {
			char chessNow = this->mArry[this->convertXY(newX, newY)];
			if (chessNow != eEMPTY && chessNow != ePLAYABLE) {
				if ((this->bBW && chessNow == eBLACK) || (!this->bBW && chessNow == eWHITE)) {
					eatCount += otherCount;
					break;
				}
				else {
					++otherCount;
					newX += moveX[i];
					newY += moveY[i];
				}
			}
			else {
				break;
			}
		}
	}
	return eatCount;
}

Reversi Reversi::operator=(Reversi rhs) {
	this->bBW = rhs.bBW;
	memcpy(this->mArry, rhs.mArry, sizeof(this->mArry));
	return *this;
}

int Reversi::avaMoving() {
	int result = 0;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (this->mArry[this->convertXY(x, y)] == eEMPTY || this->mArry[this->convertXY(x, y)] == ePLAYABLE) {
				if (this->check(x, y) != 0) {
					++result;
				}
			}
		}
	}
	return result;
}

bool Reversi::isEnd() {
	bool flag = true;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			// can be better
			if (this->mArry[this->convertXY(x, y)] == eEMPTY || this->mArry[this->convertXY(x, y)] == ePLAYABLE) {
				if (this->check(x, y) != 0) {
					flag = false;
					this->mArry[this->convertXY(x, y)] = ePLAYABLE;
				}
				else {
					if (this->mArry[this->convertXY(x, y)] == this->ePLAYABLE)
						this->mArry[this->convertXY(x, y)] = eEMPTY;
				}
				this->bBW = !this->bBW;
				if (this->check(x, y) != 0) {
					flag = false;
				}
				this->bBW = !this->bBW;
			}

		}
	}
	return flag;
}

bool Reversi::isBW() {
	/**/
	return this->bBW;
}

void Reversi::setBW(int x, int y) {
	//flip
	for (int i = 0; i<8; ++i) {
		int newX = x + moveX[i], newY = y + moveY[i];
		vector<int> flipList;
		while (isValid(newX, newY)) {
			char chessNow = this->mArry[this->convertXY(newX, newY)];
			if (chessNow != eEMPTY && chessNow != ePLAYABLE) {
				if ((this->bBW && chessNow == eBLACK) || (!this->bBW && chessNow == eWHITE)) {
					//fliping
					for (int k = 0; k < flipList.size(); ++k) {
						this->mArry[flipList[k]] = this->mArry[flipList[k]] == eBLACK ? eWHITE : eBLACK;
					}
					break;
				}
				else {
					//add to list
					flipList.push_back(this->convertXY(newX, newY));
					newX += moveX[i];
					newY += moveY[i];
				}
			}
			else {
				break;
			}
		}
	}
	//
	this->mArry[this->convertXY(x, y)] = this->bBW ? eBLACK : eWHITE;
	bool oriBW = this->bBW;
	this->bBW = !oriBW;
	bool flag = false;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (this->mArry[this->convertXY(x, y)] == eEMPTY || this->mArry[this->convertXY(x, y)] == ePLAYABLE) {
				if (this->check(x, y) != 0) {
					flag = true;
				}
			}
		}
	}
	if (!flag)this->bBW = oriBW;
	++this->steps;
}

char Reversi::getBW(int x, int y) {
	return this->mArry[this->convertXY(x, y)];
}

int Reversi::getSteps() {
	return this->steps;
}

int Reversi::BCount() {
	int result = 0;
	for (int i = 0; i < 64; ++i)
		if (this->mArry[i] == eBLACK)
			++result;
	return result;
}

int Reversi::WCount() {
	int result = 0;
	for (int i = 0; i < 64; ++i)
		if (this->mArry[i] == eWHITE)
			++result;
	return result;
}

// black base
float evalue(Reversi r) {
	float value = r.BCount() - r.WCount();
	//角佔領判斷
	for (int i = 0; i < 4; ++i) {
		char chessNow = r.getBW(corner[i][0], corner[i][1]);
		if (chessNow == r.eBLACK) value += cornerVal;
		else if (chessNow == r.eWHITE) value -= cornerVal;
	}
	//星位佔領判斷
	for (int i = 0; i < 4; ++i) {
		char chessNow = r.getBW(xsquare[i][0], xsquare[i][1]);
		if (chessNow == r.eBLACK) value -= xsquareVal;
		else if (chessNow == r.eWHITE) value += xsquareVal;
	}
	//c位佔領判斷
	for (int i = 0; i < 8; ++i) {
		char chessNow = r.getBW(csquare[i][0], csquare[i][1]);
		if (chessNow == r.eBLACK) value -= csquareVal;
		else if (chessNow == r.eWHITE) value += csquareVal;
	}
	//邊佔領判斷
	for (int i = 3; i <= 6; ++i) {
		char chessNow = r.getBW(1, i);
		if (chessNow == r.eBLACK) value += sideVal;
		else if (chessNow == r.eWHITE) value -= sideVal;
		chessNow = r.getBW(8, i);
		if (chessNow == r.eBLACK) value += sideVal;
		else if (chessNow == r.eWHITE) value -= sideVal;
	}
	for (int i = 3; i <= 6; ++i) {
		char chessNow = r.getBW(i, 1);
		if (chessNow == r.eBLACK) value += sideVal;
		else if (chessNow == r.eWHITE) value -= sideVal;
		chessNow = r.getBW(i, 8);
		if (chessNow == r.eBLACK) value += sideVal;
		else if (chessNow == r.eWHITE) value -= sideVal;
	}
	//
	return value;
}

// ai is black
float minmax(Reversi r, int depth, float alpha, float beta) {
	if (r.isEnd() || depth == 0) {
		return evalue(r);
	}
	// isEnd執行過　代表playable正確
	Reversi tmp;
	bool flag = true;
	for (int x = 1; x <= 8 && flag; ++x) {
		for (int y = 1; y <= 8 && flag; ++y) {
			if (r.getBW(x, y) != r.ePLAYABLE)continue;
			tmp = r;
			tmp.setBW(x, y);
			if (r.isBW()) {
				alpha = std::max(minmax(tmp, depth - 1, alpha, beta), alpha);
				if (beta <= alpha)
					flag = false;
			}
			else {
				beta = std::min(minmax(tmp, depth - 1, alpha, beta), beta);
				if (beta <= alpha)
					flag = false;
			}

		}
	}
	return (r.isBW() ? alpha : beta);
}
