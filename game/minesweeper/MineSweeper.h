#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class MineSweeper
{
	//=======지뢰찾기 초기세팅========
	static const int BOARD_SIZE = 10;//The board's row and col size
	static const int MINE_CNT = 30; //The count of mine
	static const int MINE_LOCATION = 9;//The location of mine
	static const char BOARD_BLOCK = '?';//The block that an user never opened
	static const char MINE_CHAR = 'X';//The Character of mine that user can see
	char userBoard[BOARD_SIZE][BOARD_SIZE] = {};//The board your can see in game
	int mineBoard[BOARD_SIZE][BOARD_SIZE] = {};//The board behine
	int userSelectCnt = 0;//사용자가 연 블락 수
	int userSelectMine = 0;//사용자가 연 지뢰 수


public:
	MineSweeper() {
		initializeBoards();
	}

private:
	void initializeBoards() {
		// initializing
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				userBoard[i][j] = BOARD_BLOCK;
				mineBoard[i][j] = 0;
			}
		}

		//arrange the mines and numbers
		int cnt = 0;
		while (cnt < MINE_CNT) {
			int row = rand() % BOARD_SIZE;
			int col = rand() % BOARD_SIZE;
			if (mineBoard[row][col] != MINE_LOCATION) {
				mineBoard[row][col] = MINE_LOCATION;//지뢰 배치
				//지뢰 옆 숫자표기
				//NorthWest
				if (row > 0 && col > 0 && mineBoard[row - 1][col - 1] != MINE_LOCATION) {
					mineBoard[row - 1][col - 1] += 1;
				}
				//North
				if (row > 0 && mineBoard[row - 1][col] != MINE_LOCATION) {
					mineBoard[row - 1][col] += 1;
				}
				//NorthEast
				if (row > 0 && col < BOARD_SIZE - 1 && mineBoard[row - 1][col + 1] != MINE_LOCATION) {
					mineBoard[row - 1][col + 1] += 1;
				}
				//West
				if (col > 0 && mineBoard[row][col - 1] != MINE_LOCATION) {
					mineBoard[row][col - 1] += 1;
				}
				//East
				if (col < BOARD_SIZE - 1 && mineBoard[row][col + 1] != MINE_LOCATION) {
					mineBoard[row][col + 1] += 1;
				}
				//SouthWest
				if (row < BOARD_SIZE - 1 && col > 0 && mineBoard[row + 1][col - 1] != MINE_LOCATION) {
					mineBoard[row + 1][col - 1] += 1;
				}
				//South
				if (row < BOARD_SIZE - 1 && mineBoard[row + 1][col] != MINE_LOCATION) {
					mineBoard[row + 1][col] += 1;
				}
				//SouthEast
				if (row < BOARD_SIZE - 1 && col < BOARD_SIZE - 1 && mineBoard[row + 1][col + 1] != MINE_LOCATION) {
					mineBoard[row + 1][col + 1] += 1;
				}

				cnt++;//if you put one mine, cnt grow
			}
		}

	}
public:
	void playMinesweeper() {
		int userRow = 0; //사용자에게 입력받는 행
		int userCol = 0; //사용자에게 입력받는 열

		//=======인게임=======
		srand((unsigned)time(NULL));

		while (true) {
			userRow = 0;
			userCol = 0;
			userSelectCnt = 0;

			// print a board
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					cout << userBoard[i][j] << " ";
					if (userBoard[i][j] != BOARD_BLOCK)//Check the block user broke
						userSelectCnt++;
					if (userBoard[i][j] == MINE_CHAR)//if the user touched the mine
						userSelectMine++;
				}
				cout << endl;
			}
			//when you win
			if (userSelectCnt >= BOARD_SIZE * BOARD_SIZE - MINE_CNT) {
				cout << "You win!!" << endl;
				break;
			}
			//when you lose
			if (userSelectMine > 0) {
				cout << "You lose!!" << endl;
				break;
			}

			//사용자 입력 받기
			cout << "(row, col) choose one point(exit:-1 -1): ";
			cin >> userRow >> userCol;
			//잘못 입력 or 종료 시
			if (userRow >= BOARD_SIZE || userCol >= BOARD_SIZE
				|| userRow < 0 || userCol < 0) {
				if (userRow == -1 && userCol == -1) {//Exit the game
					cout << "Program Exit!!" << endl;
					break;
				}
				cout << "You got the wrong choice. Try again!!" << endl;
				continue;
			}
			//이전에 선택한 블록을 다시 선택 시
			if (userBoard[userRow][userCol] != BOARD_BLOCK) {
				cout << "You've already chose that point. Try again!!" << endl;
				continue;
			}

			//Change the block into the numbers or mine
			int mineBoardNum = mineBoard[userRow][userCol];
			switch (mineBoardNum) {
			case 0: userBoard[userRow][userCol] = '0';
				break;
			case 1: userBoard[userRow][userCol] = '1';
				break;
			case 2: userBoard[userRow][userCol] = '2';
				break;
			case 3: userBoard[userRow][userCol] = '3';
				break;
			case 4: userBoard[userRow][userCol] = '4';
				break;
			case 5: userBoard[userRow][userCol] = '5';
				break;
			case 6: userBoard[userRow][userCol] = '6';
				break;
			case 7: userBoard[userRow][userCol] = '7';
				break;
			case 8: userBoard[userRow][userCol] = '8';
				break;
			case 9: userBoard[userRow][userCol] = MINE_CHAR;
				break;
			}
		}
	}
};

