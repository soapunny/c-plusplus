#pragma once
#include "config.h"
#include "GameNode.h"
#include <list>

class Tank;
class GameProc: public GameNode
{
private:
	HDC hdc;			// handle - DC (Device Context)
	PAINTSTRUCT ps;
	HANDLE updateTimer;	// 타이머 객체를 지정하는 핸들
	HANDLE* enemyTimer;
	HANDLE enemyCreateTimer;
	FPOINT ptMouse;
	char szText[128] = "";

	int stage;
	Tank* myTank;
	list<Tank*> enemies;
	int maxEnemyCnt;
	bool isBoss;
	bool hasCleared;

	//private function
	void Update();
	void Release();
	void Render(HDC hdc);

	void KeyDown(WPARAM wParam);
	void RunTimer(WPARAM wParam);
	void MoveEnemyByTimer(int timerId);

	void CreateEnemy();

public:
	HRESULT Init();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

