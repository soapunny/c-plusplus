#pragma once
#include "MovingNode.h"
#include <iostream>

class RGBColor; //���漱�� has a ����
class Tank;
class Bullet :public MovingNode //is a ����
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