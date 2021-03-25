#pragma once
#include "GameNode.h"

class RGBColor; //���漱�� has a ����
class Bullet:public GameNode //is a ����
{
	int speed;
	RGBColor* color = nullptr;
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
};

