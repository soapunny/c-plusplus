#pragma once
#include "config.h"
#include "Tank.h"

class RGBColor; // has a ����
class GameProc: public GameNode
{
private:
	HDC hdc;			// handle - DC (Device Context)
	PAINTSTRUCT ps;
	HANDLE hTimer;	// Ÿ�̸� ��ü�� �����ϴ� �ڵ�
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

