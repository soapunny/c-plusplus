#pragma once
#include "config.h"
#include "GameNode.h"
#include <list>

class RGBColor; // has a 관계
class Tank;
class GameProc: public GameNode
{
private:
	HDC hdc;			// handle - DC (Device Context)
	PAINTSTRUCT ps;
	HANDLE hTimer;	// 타이머 객체를 지정하는 핸들
	HANDLE* enemyTimer;
	HANDLE enemyCreateTimer;
	FPOINT ptMouse;
	char szText[128] = "";
	RGBColor* bgColor;
	int maxEnemyCnt;

	Tank* myTank;
	list<Tank*> enemies;

public:
	HRESULT Init();
	void Render(HDC hdc);
	void Update();
	void Release();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void KeyDown(WPARAM wParam);
	void RunTimer(WPARAM wParam);
};

