#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "ExitGame.h"

using namespace std;

//�ֿܼ� Ż�����
//10 * 10 ũ���� ���� �����.(2���� �迭��)
//WASD Ű �Է��� ���ؼ� �÷��̾�(O)�� �����¿�� �̵��� �Ѵ�.
//�÷��̾� ����������(0, 0) Ż��������(0, 0)�� �����ϰ� �������� :
//�߰��� ������ �ʴ� ������ �߰��ϸ� "������ �߰��߽��ϴ�!!" ���
//0�� E�� �����ϸ� "Ż�⿡ �����߽��ϴ�~!!" ����ϰ� ����
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
			cout << "������ �߰��߽��ϴ�~!!" << endl;
		}
		if (playerPoint[X] == endPoint[X] && playerPoint[Y] == endPoint[Y]) {
			cout << "Ż�⿡ �����߽��ϴ�~!!" << endl;
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
