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

enum MOVE { MOVE_LEFT, MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFTUP, MOVE_RIGHTUP, MOVE_LEFTDOWN, MOVE_RIGHTDOWN, MOVE_NOWHERE };
enum BULLET { DEFAULT_BULLET, BOMB_BULLET, GUIDED_BULLET };
enum TANK_LEVEL { WEAKEST_LEVEL, WEAK_LEVEL, NORMAL_LEVEL, STRONG_LEVEL, AWFUL_LEVEL, HIGHST_LEVEL };
extern HWND g_hWnd;

//�� �� ������ ������ ���ϴ� �Լ�
inline float GetAngle(FPOINT curPos, FPOINT targetPos) {

	float x = targetPos.x - curPos.x;
	float y = targetPos.y - curPos.y;
	//atan2f(y,x) 1��и� 2��и�, 3��и� 4��и��� ���� ���� ������ ���
	float targetAngle = atan2f(-y, x);

	return targetAngle;
}

inline float GetDistance(FPOINT pos1, FPOINT pos2) {
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

