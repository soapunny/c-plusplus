#pragma once
#include "GameNode.h"
#include <list>

class RGBColor;
class Bullet;
class Tank: public GameNode
{
private:
	static int NAME_NUM;
	RECT shape;
	string name;
	RGBColor* color = nullptr;
	list<Bullet*> bullets;
	int maxHp;
	int currentHp;
	int speed;
	int maxBullet;
	bool isAlive;
	MOVE direction;

	//포신의 시작점, 끝점
	POINT barrelLeft;
	POINT barrelRight;
	POINT barrelLeftTop;
	POINT barrelRightTop;
	int barrelSize;
	float barrelAngle;	//단위	: 도(degree, 0 ~ 360)
						//		: 라디안(radian, 0 ~ 2PIE(6.28))

public:
	inline Tank() {}
	inline Tank(string name) { this->name = name; }
	inline ~Tank() {}

	HRESULT Init();
	void Render(HDC hdc);
	void RenderRandom(HDC hdc);
	void RenderBarrel(HDC hdc);
	void RotateBarrel(float angle);
	void Update();
	void Release();

	void Fire();
	void FlyBullet();
	void RemoveBullet(Tank* tank);
	void Move(MOVE move);
	void MoveRandom();
	void Die();
	void MoveBarrelTo(POINT pos);

	inline void SetBarrelAngle(float barrelAngle) { this->barrelAngle = barrelAngle; }
	void SetShape(POINT pos);
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline bool GetAlive() { return isAlive; }
	void HitByBullet(int damage);
};

