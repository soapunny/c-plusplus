#pragma once
#include "Bullet.h"

class GuidedBullet : public Bullet
{
protected:
	float startAngle;
	float targetAngle;
	FPOINT targetPos;
public:
	HRESULT Init();
	void Move();
	inline void SetTargetAngle(float targetAngle) { this->targetAngle = targetAngle; }
	inline void SetFlyingAngle(float flyingAngle) { this->flyingAngle = flyingAngle; this->startAngle = flyingAngle; }
	inline void SetTargetPos(FPOINT targetPos) { this->targetPos = targetPos; }
};

