#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "ExitGame.h"

using namespace std;

//콘솔용 탈출게임
//10 * 10 크기의 맵을 만든다.(2차원 배열로)
//WASD 키 입력을 통해서 플레이어(O)가 상하좌우로 이동을 한다.
//플레이어 시작지점은(0, 0) 탈출지점은(0, 0)을 제외하고 랜덤설정 :
//중간에 보이지 않는 보물을 발견하면 "보물을 발견했습니다!!" 출력
//0가 E에 도착하면 "탈출에 성공했습니다~!!" 출력하고 종료
//O##########
//###########
//###########
//###########
//#######E###
//###########
//###########
//###########
//###########
//###########
ExitGame::ExitGame() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			map[i][j] = '#';
		}
	}
	playerPoint[X] = 0;
	playerPoint[Y] = 0;
	endPoint[X] = rand() % (MAP_SIZE - 1) + 1;//(1~9)
	endPoint[Y] = rand() % (MAP_SIZE - 1) + 1;//(1~9)
	treasurePoint[X] = rand() % (MAP_SIZE - 1) + 1;//(1~9)
	treasurePoint[Y] = rand() % (MAP_SIZE - 1) + 1;//(1~9)
	
	map[playerPoint[Y]][playerPoint[X]] = 'O';
	map[endPoint[Y]][endPoint[X]] = 'E';
}

void ExitGame::printScreen() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			cout << map[i][j]<<" ";
		}
		cout << endl;
	}
}

void ExitGame::startGame() {
	char userInput = ' ';

	while (true) {
		system("cls");
		printScreen();

		if (playerPoint[X] == treasurePoint[X] && playerPoint[Y] == treasurePoint[Y]) {
			cout << "보물을 발견했습니다~!!" << endl;
		}
		if (playerPoint[X] == endPoint[X] && playerPoint[Y] == endPoint[Y]) {
			cout << "탈출에 성공했습니다~!!" << endl;
			break;
		}

		userInput = _getch();
		switch (userInput) {
		case 'w': removeLoc(playerPoint[X], playerPoint[Y]);
				if (playerPoint[Y] == 0) playerPoint[Y] = MAP_SIZE - 1;
				else playerPoint[Y] -= 1;
				break;
		case 'a': removeLoc(playerPoint[X], playerPoint[Y]);
				if (playerPoint[X] == 0) playerPoint[X] = MAP_SIZE - 1;
				else playerPoint[X] -= 1;
				break;
		case 'd': removeLoc(playerPoint[X], playerPoint[Y]); 
				if (playerPoint[X] == MAP_SIZE - 1) playerPoint[X] = 0;
				else playerPoint[X] += 1;
				break; 
		case 's': removeLoc(playerPoint[X], playerPoint[Y]); 
				if (playerPoint[Y] == MAP_SIZE - 1)playerPoint[Y] = 0;
				else playerPoint[Y] += 1;
				break;
		}

		map[playerPoint[Y]][playerPoint[X]] = 'O';

		userInput = ' ';
	}
}

void ExitGame::removeLoc(int x, int y) {
	map[y][x] = '#';
}
