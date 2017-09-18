#include <cstring>
#include <vector>
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
}

int Reversi::convertXY(int x, int y) {
	return (x - 1) * 8 + (y - 1);
}

bool Reversi::isValid(int x, int y) {
	if (x<0 || x>8||y<0||y>8)
		return false;
	else
		return true;
}

int Reversi::check(int x, int y) {
	if (isValid(x, y) && this->mArry[this->convertXY(x, y)] != eEMPTY)
		return 0;
	int eatCount = 0;
	for (int i = 0; i<8; ++i) {
		int newX = x + moveX[i], newY = y + moveY[i];
		int otherCount = 0;
		while (isValid(newX, newY)) {
			char chessNow = this->mArry[this->convertXY(newX, newY)];
			if (chessNow != eEMPTY) {
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

bool Reversi::isEnd() {
	bool oriBW = this->bBW;
	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (this->mArry[this->convertXY(x, y)] == eEMPTY) {
				this->bBW = true;
				if (this->check(x, y) != 0) {
					this->bBW = oriBW;
					return false;
				}
				this->bBW = false;
				if (this->check(x, y) != 0) {
					this->bBW = oriBW;
					return false;
				}
			}

		}
	}
	this->bBW = oriBW;
	return true;
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
			if (chessNow != eEMPTY) {
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
			if (this->mArry[this->convertXY(x, y)] == eEMPTY) {
				if (this->check(x, y) != 0) {
					flag = true;
				}
			}
		}
	}
	if(!flag)this->bBW = oriBW;
}

char Reversi::getBW(int x, int y) {
	return this->mArry[this->convertXY(x, y)];
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
