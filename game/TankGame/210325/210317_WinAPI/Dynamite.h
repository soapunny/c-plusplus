#pragma once
#include "Bullet.h"
#include <list>

class Tank;
class Dynamite: public Bullet
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
	inline void SetFlyingAngle(float flyingAngle) { this->flyingAngle = flyingAngle; }
	inline int GetDamage() { return damage; }
	void RemoveBullet(Tank* tank);
	bool HasNoBullet() { return hasExploded && bullets.size() == 0;}
};


