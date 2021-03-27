#pragma once
#include "MovingNode.h"

class TankNode;
class BulletNode : public MovingNode {
protected:
	BULLET bulletType;
	float flyingAngle;
	int damage;

public:
	inline virtual void SetFlyingAngle(float flyingAngle) = 0;
	inline virtual int GetDamage() = 0;
	inline virtual BULLET GetBulletType() = 0;
	bool HasNoBullet() { return true; }
};
