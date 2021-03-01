#pragma once
#include <iostream>
#include <Windows.h>
#include "PlayMenu.h"
#include "SleepTime.h"
using namespace std;

class Menu
{

public:
	Menu() {
		//
	}
	void showMenu() {
		int playerChoice = 0;

		while (true) {
			system("cls");
			printMenu();
			playerChoice = selectMenu();

			switch (playerChoice) {
			case 1: startPlayMenu(PlayMenu::P2C_TYPE); //playMenu1.showMenu();
				break;
			case 2: //startPlayMenu(PlayMenu::P2P_TYPE); //playMenu2.showMenu();
				break;
			case 3: return;
				break;
			default: cout << "Choose again!!"; Sleep(SleepTime::MENU_SLEEP_TIME);
				break;
			}
		}
	}

private:
	void startPlayMenu(int menuType) {
		PlayMenu playMenu(menuType);
		playMenu.showMenu();
	}

	void printMenu() {
		cout << "######################" << endl;
		cout << "1. COM vs Player" << endl;
		cout << "2. Player vs Player(¹Ì±¸Çö)" << endl;
		cout << "3. Exit" << endl;
		cout << "######################" << endl;

	}

	int selectMenu() {
		int playerChoice = 0;
		cout << "choose a menu : ";
		cin >> playerChoice;

		return playerChoice;
	}
};

