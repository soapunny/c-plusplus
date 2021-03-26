#pragma once
#include <Windows.h>
#include <string>

using namespace std;

#define WND_WIDTH 1080
#define WND_HEIGHT 720
#define PI 3.14f

typedef struct FloatPoint {
	float x;
	float y;
} FPOINT, * LPFPOINT;

enum MOVE{MOVE_LEFT, MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFTUP, MOVE_RIGHTUP, MOVE_LEFTDOWN, MOVE_RIGHTDOWN, MOVE_NOWHERE};
enum BULLET{DEFAULT_BULLET, DYNAMITE_BULLET};
extern HWND g_hWnd;
