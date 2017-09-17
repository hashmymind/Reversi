#pragma once

class Reversi {
private:
	bool bBW; // b 1, w 0
	char mArry[64];
public:
	enum { eEMPTY, eBLACK, eWHITE };
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

	Reversi operator=(Reversi rhs);

	Reversi(void);
	~Reversi(void);
};
