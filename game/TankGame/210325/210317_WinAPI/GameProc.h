#pragma once
#include "config.h"
#include "Tank.h"

class RGBColor; // has a 관계
class GameProc: public GameNode
{
private:
	HDC hdc;			// handle - DC (Device Context)
	PAINTSTRUCT ps;
	HANDLE hTimer;	// 타이머 객체를 지정하는 핸들
	HANDLE enemyTimer;
	POINT ptMouse{ 0, 0 };
	char szText[128] = "";
	RGBColor* bgColor;

	Tank tank1;
	Tank tank2;
	Tank enemy1;

public:
	HRESULT Init();
	void Render(HDC hdc);
	void Update();
	void Release();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void KeyDown(WPARAM wParam);
	void RunTimer(WPARAM wParam);
};

