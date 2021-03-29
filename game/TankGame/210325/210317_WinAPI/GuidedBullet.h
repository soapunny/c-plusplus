#pragma once
#include "Bullet.h"

class TankNode;
class GuidedBullet : public Bullet
{
protected:
	float startAngle;
	float targetAngle;
	float distance;
	MovingNode* target;
	FPOINT startPos;
public:
	HRESULT Init();
	void Move();
	inline void SetTargetAngle(float targetAngle) { this->targetAngle = targetAngle; }
	inline void SetFlyingAngle(float flyingAngle) { this->flyingAngle = flyingAngle; this->startAngle = flyingAngle;}
	inline void SetTarget(MovingNode* movingNodePtr) { this->target = movingNodePtr; }
	inline void SetPos(FPOINT pos) { this->pos = pos; startPos = pos; }
};

