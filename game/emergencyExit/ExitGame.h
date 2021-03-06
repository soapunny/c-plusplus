#pragma once
class ExitGame
{
public:
	static const int MAP_SIZE = 10;
	static const int X = 0;
	static const int Y = 1;
private:
	char map[MAP_SIZE][MAP_SIZE];
	int playerPoint[2];
	int endPoint[2];
	int treasurePoint[2];

public:
	ExitGame();
	void startGame();
private:
	void printScreen();
	void removeLoc(int x, int y);
};

