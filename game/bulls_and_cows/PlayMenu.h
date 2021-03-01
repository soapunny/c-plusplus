#pragma once
#include <iostream>
#include <Windows.h>
#include "P2CPlay.h"
#include "P2PPlay.h"
#include "SleepTime.h"
using namespace std;

class PlayMenu {
public : 
	static const int P2C_TYPE = 0;
	static const int P2P_TYPE = 1;

private:
	int gameType = 0; // 0: P2C, 1: P2P

public:
	PlayMenu(int gameType = P2C_TYPE) {
		//
		this->gameType = gameType;
	}
	void showMenu() {
		int playerChoice = 0;

		while (true) {
			system("cls");
			printMenu();
			playerChoice = selectMenu();

			switch (playerChoice) {
			case 1: startGame(3);
				break;
			case 2: startGame(6);
				break;
			case 3: startGame(9);
				break;
			case 4: return;
				break;
			default: cout << "Choose again!!"; Sleep(SleepTime::MENU_SLEEP_TIME);
				break;
			}
		}
	}
	void setGameType(int gameType=P2C_TYPE) {
		this->gameType = gameType;
	}

private :
	void startGame(int totalInning) {
		if (gameType == P2C_TYPE) {
			P2CPlay p2cPlay(totalInning);
			p2cPlay.playGame();
		}
		else {
			P2PPlay p2pPlay(totalInning); 
			p2pPlay.playGame();
		}
	}

	void printMenu() {
		cout << "######################" << endl;
		cout << "1. 3-Inning" << endl;
		cout << "2. 6-Inning" << endl;
		cout << "3. 9-Inning" << endl;
		cout << "4. Prior Menu" << endl;
		cout << "######################" << endl;

	}

	int selectMenu() {
		int playerChoice = 0;
		cout << "choose a menu : ";
		cin >> playerChoice;

		return playerChoice;
	}
};