#include <iostream>
#include <graphics.h>
#include <math.h>

int chessboardLen = 999;

class ChessBoard {
private:
	char** cb = new char* [3];
	bool isO = true;
	bool isRunning = true;

	// chessboard properties
	int len;
	int radius;

public:
	ChessBoard() : ChessBoard(999) {};

	ChessBoard(int l) : len(l) {
		this->radius = int(len / 3 * 0.35);
		for (int i = 0; i < 3; i++) {
			cb[i] = new char[3];
		}
		clearChessBoard();
	}

	~ChessBoard() {
		for (int i = 0; i < 3; i++) {
			delete[]cb[i];
		}
		delete[] cb;
		std::cout << "destruction called..." << std::endl;
	}

	void clearChessBoard() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cb[i][j] = '-';
			}
		}
	}

	void setChess(int x, int y) {
		// if cb != '-' then return
		if (cb[x][y] != '-')
			return;

		// isO choose whether to put on the chessboard
		isO ? cb[x][y] = 'o' : cb[x][y] = 'x';
		isO = !isO;
	}

	void showChessBoard() {

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				std::cout << cb[j][i] << " ";
			}
			std::cout << std::endl;
		}
	}

	void renderChessBoard() {
		// render the base lines
		for (int i = 0; i < 2; i++) {
			// vertical
			line((i + 1) * len / 3, 0, (i + 1) * len / 3, len);
			line((i + 1) * len / 3, 0, (i + 1) * len / 3, len);

			// horizontal
			line(0, (i + 1) * len / 3, len, (i + 1) * len / 3);
			line(0, (i + 1) * len / 3, len, (i + 1) * len / 3);
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int center_x = i * len / 3 + int(len / 3 / 2);
				int center_y = j * len / 3 + int(len / 3 / 2);

				if (cb[i][j] == 'x') {
					int tmp = int(sqrt(2) * radius / 2);
					// upper left -> lower right
					line(center_x - tmp, center_y - tmp, center_x + tmp, center_y + tmp);

					// upper right -> lower left
					line(center_x - tmp, center_y + tmp, center_x + tmp, center_y - tmp);
				}
				else if (cb[i][j] == 'o') {
					// upper right -> lower left
					circle(center_x, center_y, radius);
				}
				else if (cb[i][j] == '-') continue;
			}
		}
	}

	void judgeWin() {
		auto Win = [=](char chr) {
			if (chr == '-') {
				int re = MessageBox(GetHWnd(), _T("Draw..."), _T("Game Done..."), MB_OK);
			}
			else if (chr == 'o') {
				int re = MessageBox(GetHWnd(), _T("o Win..."), _T("Game Done..."), MB_OK);
			}
			else if (chr == 'x') {
				int re = MessageBox(GetHWnd(), _T("x Win..."), _T("Game Done..."), MB_OK);
			}
			int result = MessageBox(GetHWnd(), _T("Do you want to continue?"), _T("Confirmation"), MB_YESNO | MB_ICONQUESTION);
			isRunning = result == IDYES;
			
			this->showChessBoard();
			this->clearChessBoard();
			};

		bool pingju = true;
		for (int i = 0; i < 3; i++) {
			if (cb[i][0] != '-' && (cb[i][0] == cb[i][1] && cb[i][1] == cb[i][2])) {
				Win(cb[i][0]);
			}
			if (cb[0][i] != '-' && (cb[0][i] == cb[1][i] && cb[1][i] == cb[2][i])) {
				Win(cb[0][i]);
			}
			if (pingju && (cb[i][0] == '-' || cb[i][1] == '-' || cb[i][2] == '-')) {
				pingju = false;
			}
		}
		if (pingju) {
			Win('-');
			return;
		}
		if (cb[0][0] != '-' && (cb[0][0] == cb[1][1] && cb[1][1] == cb[2][2])) {
			Win(cb[0][0]);
		}
		if (cb[0][2] != '-' && (cb[0][2] == cb[1][1] && cb[1][1] == cb[2][0])) {
			Win(cb[0][2]);
		}
	}

	bool gOn() const {
		return this->isRunning;
	}

};

int main() {
	// create the window -> chessboard(w.len = c.len)
	auto window = initgraph(chessboardLen, chessboardLen, 1);
	auto chessboard = new ChessBoard(chessboardLen);

	BeginBatchDraw();
	while (chessboard->gOn()) {
		ExMessage msg;

		while (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN) {
				// get the pos of the mouse
				int x = msg.x;
				int y = msg.y;

				// use a b to index the pos of the chess
				int a = (x < 334) ? 0 : ((x < 667) ? 1 : 2);
				int b = (y < 334) ? 0 : ((y < 667) ? 1 : 2);
				chessboard->setChess(a, b);
			}

		}

		cleardevice();
		chessboard->renderChessBoard();
		chessboard->judgeWin();
		FlushBatchDraw();
	}

	EndBatchDraw();
	delete chessboard;

	return 0;
}
