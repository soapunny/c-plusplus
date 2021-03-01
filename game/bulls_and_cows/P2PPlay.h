#pragma once
#include <iostream>
#include <Windows.h>
#include "SleepTime.h"
using namespace std;

class P2PPlay {
	int totalInning = 3;

public:
	P2PPlay(int totalInning) {
		this->totalInning = totalInning;
	}

	void playGame() {
		system("cls");
		cout << " P2P Game start!!";

		while (true) {
			Sleep(SleepTime::GAME_SLEEP_TIME);
			printScreen();
		}
	}

	void printScreen() {
		system("cls");
		cout << " Total Inning : " << totalInning << endl;
		
	}
};