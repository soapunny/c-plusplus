#pragma once
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>
#include "SleepTime.h"
using namespace std;

class P2CPlay {
	const int COMPUTER = 0;
	const int PLAYER = 1;
	const int TOP = 0;
	const int BOTTOM = 1;

	int totalInning = 3;
	int comScore = 0;
	int playerScore = 0;
	int currentInning = 1;
	int ballCount = 0;
	int strikeCount = 0;
	int outCount = 0;
	int homeTeam = COMPUTER;//0:com, 1:player
	int topBottom = TOP;
	string priorGuess[9] = { "000","000","000","000","000","000","000","000","000" };
 	boolean firstBase = false;
	boolean secondBase = false;
	boolean thirdBase = false;
	boolean isPlaying = true;

public:
	P2CPlay(int totalInning) {
		this->totalInning = totalInning;
	}

	void playGame() {

		system("cls");
		cout << " P2C Game start!!" << endl;

		while (isPlaying) {

			if (topBottom == TOP) {//Top
				if (homeTeam == COMPUTER) {
					playBatting();
				}
				else {
					playPitching();
				}
			}
			else {//Bottom
				if (homeTeam == COMPUTER) {
					playPitching();
				}
				else {
					playBatting();
				}
			}

			initGame();
		}
	}

private:
	void playPitching() {
		string answerNum = genateRandom();
		const int MAX_TRY = 9;
		int currentTry = 0;

		while (currentTry<MAX_TRY && isPlaying) {
			string myGuess = "";

			Sleep(SleepTime::GAME_SLEEP_TIME);
			printScreen((currentTry <= 0 ? currentTry : currentTry-1));
			strikeCount = 0;
			ballCount = 0;

			myGuess = guessNumber();
			priorGuess[currentTry] = myGuess; //add my guess to priorGuess
			currentTry++;

			matchNum(myGuess, answerNum);

			if (strikeCount >= 3) {
				break;
			}
			else if (currentTry == MAX_TRY) {
				ballCount = 4;
				break;
			}
		}
	}

	void matchNum(string myGuess, string answerNum) {
		if (!isPlaying) {//if it's not playing now
			return;
		}

		if (myGuess[0] == answerNum[0]) {
			strikeCount++;
		}
		else if (myGuess[0] == answerNum[1] || myGuess[0] == answerNum[2]){
			ballCount++;
		}
		if (myGuess[1] == answerNum[1]) {
			strikeCount++;
		}
		else if (myGuess[1] == answerNum[0] || myGuess[1] == answerNum[2]) {
			ballCount++;
		}
		if (myGuess[2] == answerNum[2]) {
			strikeCount++;
		}
		else if (myGuess[2] == answerNum[0] || myGuess[2] == answerNum[1]) {
			ballCount++;
		}
	}

	string guessNumber() {
		string playerChoice = "";

		do {
			cout << "Guess Number(000~999, End-Game:-1): ";
			cin >> playerChoice;
			if (playerChoice.length() != 3) {
				if (playerChoice == "-1") {
					isPlaying = false;
					break;
				}
				playerChoice = "";
				cout << "Not in range. Try again!!" << endl;
				continue;
			}
			for (int i = 0; i < sizeof(priorGuess)/sizeof(priorGuess[0]); i++) {
				if (priorGuess[i] == playerChoice) {
					cout << "You've already guessed the number. Try again!!" << endl;
					playerChoice = "";
					break;
				}
			}
		} while (playerChoice == "");



		return playerChoice;
	}
	void playBatting() {
		string answerNum = writeNum();
		const int MAX_TRY = 9;
		int currentTry = 0;

		while (currentTry < MAX_TRY && isPlaying) {
			string comGuess = "";

			Sleep(SleepTime::GAME_SLEEP_TIME);
			printScreen((currentTry <= 0 ? currentTry : currentTry - 1));
			strikeCount = 0;
			ballCount = 0;

			string firstNum = "";
			string secondNum = "";
			comGuess = guessRandom(firstNum+answerNum[0], secondNum+answerNum[1]);
			cout << "Com's guess: " << comGuess << endl;
			currentTry++;

			matchNum(comGuess, answerNum);

			if (strikeCount >= 3) {
				break;
			}
			else if (currentTry == MAX_TRY) {
				ballCount = 4;
				break;
			}
		}
	}

	string writeNum() {
		string yourNum = "";
		do {
			cout << "Write down a number you want(000~999): ";
			cin >> yourNum;

			if (yourNum[0] == yourNum[1] || yourNum[0] == yourNum[2] || yourNum[1] == yourNum[2]) {
				cout << "Use a different number for each digit!!" << endl;
				yourNum = "";
			}
		} while (yourNum.size() != 3);

		return yourNum;
	}

	string guessRandom(string firstNum, string secondNum) {
		srand((unsigned)time(NULL));
		string thirdNum = "";

		thirdNum = to_string(rand() % 10);

		return firstNum + secondNum + thirdNum;
	}

	void printScreen(int priorTry) {
		system("cls");
		cout << "Total Inning : " << totalInning << endl;
		cout << "Prior Guess : " << priorGuess[priorTry] << endl;
		cout << "######################" << endl;
		cout << (homeTeam != COMPUTER?"COM   :"+to_string(comScore): "Player:"+ to_string(playerScore)) << "\t" << getCurrentInning() << (topBottom==TOP?"бт":"бф") << endl;
		cout << (homeTeam == COMPUTER? "COM   :" + to_string(comScore) : "Player:" + to_string(playerScore)) << "\t" << ballCount << "-" << strikeCount << " " << outCount << endl;
		cout << "######################" << endl;
		cout << "\t" << (secondBase ? "бс" : "бр") << endl << endl;
		cout << (thirdBase ? "бс" : "бр") << "\t\t" << (firstBase ? "бс" : "бр") << endl << endl;
		cout << "\t" << "бр" << endl;
		cout << "######################" << endl;
	}

	void initGame() {
		if (ballCount >= 4) {//base on balls
			cout << "Base on Balls" << endl;
			goOnBase();
		}
		else if (strikeCount >= 3) {//strike out
			cout << "Strike out!!" << endl;
			addOutCount();
		}

		for (string& number : priorGuess) {
			number = "000";//initialize priorGuess array
		}
		strikeCount = 0;
		ballCount = 0;
	}

	void addOutCount() {
		outCount++;

		if (outCount >= 3) {
			changeTopBottom();
			outCount = 0;
		}
	}

	void endGame() {
		isPlaying = false;
		if (comScore > playerScore) {
			cout << "Computer win!!" << endl;
		}
		else if (comScore < playerScore) {
			cout << "You win!!" << endl;
		}
		else {
			cout << "Draw!!" << endl;
		}
		cout << "YOU: " << playerScore << ", COM: " << comScore << endl;
		cout << "Press any key to end: ";
		string tmp = "";
		cin >> tmp;
	}

	void changeTopBottom() {
		if (topBottom == TOP)
			topBottom = BOTTOM;
		else {
			if (currentInning >= totalInning) {//Last-Inning Bottom end
				endGame();
				return;
			}
			else {
				currentInning++;
				topBottom = TOP;
			}
		}
		cout << "Offense Defense Shift!!" << endl;

		firstBase = false;
		secondBase = false;
		thirdBase = false;
	}

	void goOnBase() {
		if (!firstBase) {//if first base is empty
			firstBase = true;
		}
		else {//if first base is occupied
			if (!secondBase) {//if second base is empty
				secondBase = true;
			}
			else {//if second base is occupied
				if (!thirdBase) {//if thirdBase is empty
					thirdBase = true;
				}
				else {//if they are fully occupied
					getScore();
				}
			}
		}
	}

	void getScore() {
		if (topBottom == TOP) {//top
			if (homeTeam == COMPUTER)
				playerScore++;
			else
				comScore++;
		}
		else {//bottom
			if (homeTeam == COMPUTER)
				comScore++;
			else
				playerScore++;
		}
	}

	string genateRandom() {
		srand((unsigned)time(NULL));
		int num1 = 0;
		int num2 = 0;
		int num3 = 0;

		num1 = rand() % 10;
		do {
			num2 = rand() % 10;
		} while (num1 == num2);
		
		do {
			num3 = rand() % 10;
		} while (num3 == num1 || num3 == num2);
		
		return to_string(num1) + to_string(num2) + to_string(num3);
	}

	string getCurrentInning() {
		if (currentInning == 1) {
			return "1st";
		}
		else if (currentInning == 2) {
			return "2nd";
		}
		else {
			return to_string(currentInning) + "th";
		}
	}

};