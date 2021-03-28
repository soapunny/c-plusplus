#pragma once
#include "Bullet.h"
#include <list>

class TankNode;
class BombBullet: public Bullet
{
	list<Bullet> bullets;
	int maxBulletCount;
	int countDown;
	bool hasExploded;

public:
	HRESULT Init();
	void Render(HDC hdc);
	void Update();
	void Release();

	void Move();
	void TraceBullet(TankNode* myTank, TankNode* tank);
	bool HasNoBullet() { return hasExploded && bullets.size() == 0;}
};


