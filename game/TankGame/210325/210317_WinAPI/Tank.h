#pragma once
#include "MovingNode.h"
#include <list>

class RGBColor;
class Bullet;

class Tank: public MovingNode
{
private:
	int MOVE_PATTERN;
	static int NAME_NUM;
	RECT shape;
	string name;
	RGBColor* color = nullptr;
	list<Bullet*> bullets;
	int maxHp;
	int currentHp;
	int speed;
	int maxBullet;
	int killCnt;
	bool isAlive;
	MOVE direction;
	BULLET bulletType;

	//포신의 시작점, 끝점
	FPOINT barrelLeft;
	FPOINT barrelRight;
	FPOINT barrelLeftTop;
	FPOINT barrelRightTop;
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
	void Move();
	void Die();
	void MoveBarrelTo(FPOINT pos);

	inline void SetBarrelAngle(float barrelAngle) { this->barrelAngle = barrelAngle; }
	inline void SetBulletType(BULLET bulletType) { this->bulletType = bulletType; }
	void SetShape(FPOINT pos);
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline bool GetAlive() { return isAlive; }
	inline int GetKillCnt() { return killCnt; }
	inline int GetNameNum() { return NAME_NUM; }
	void HitByBullet(int damage);
};

