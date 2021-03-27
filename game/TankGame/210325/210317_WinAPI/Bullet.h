#pragma once
#include "BulletNode.h"
#include <iostream>

class TankNode;
class Bullet :public BulletNode //is a °ü°è
{
protected:

public:
	HRESULT Init();
	void Render(HDC hdc);
	void Update();
	void Release();

	void Move();
	inline void SetFlyingAngle(float flyingAngle) { this->flyingAngle = flyingAngle; }
	inline int GetDamage() { return damage; }
	inline BULLET GetBulletType() { return bulletType; }
	bool HasNoBullet() { return true; }
};