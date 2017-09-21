// black base
int evalue(Reversi r) {
	int value = r.BCount() - r.WCount();
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
