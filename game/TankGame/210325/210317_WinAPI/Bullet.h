#pragma once
#include "MovingNode.h"
#include <iostream>

class RGBColor; //전방선언 has a 관계
class Tank;
class Bullet :public MovingNode //is a 관계
{
protected:
	int speed;
	RGBColor* color = nullptr;
	BULLET bulletType;
	float flyingAngle;
	int damage;

public:
	HRESULT Init();
	void Render(HDC hdc);
	void Update();
	void Release();

	void Move();
	inline void SetFlyingAngle(float flyingAngle) { this->flyingAngle = flyingAngle; }
	inline int GetDamage() { return damage; }
	inline BULLET GetBulletType() { return bulletType; }
	inline void RemoveBullet(Tank* tank) {}
	bool HasNoBullet() { return false; }
};