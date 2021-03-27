#pragma once
#include "MovingNode.h"
#include <list>

class Bullet;
class TankNode : public MovingNode 
{
protected:
//Fields
	string name; //�̸�
	RECT shape;	//��ũ ���
	list<Bullet*> bullets; //��ũ ����
	int maxHp;
	int currentHp;
	int maxBullet;
	int killCnt;
	MOVE direction;
	BULLET bulletType1;
	BULLET bulletType2;
	TANK_LEVEL tankLevel;

	//������ ������, ����
	FPOINT barrelLeft;
	FPOINT barrelRight;
	FPOINT barrelLeftTop;
	FPOINT barrelRightTop;
	int barrelSize;
	float barrelAngle;	//����	: ��(degree, 0 ~ 360)
						//		: ����(radian, 0 ~ 2PIE(6.28f))

//Functions
	inline virtual void Explode() = 0;
	inline virtual void RenderBarrel(HDC hdc) = 0;

public:
	inline virtual void SetTankLevel(TANK_LEVEL level) = 0;
	inline virtual void Fire() = 0;
	inline virtual void MoveBarrelTo(FPOINT targetPos) = 0;
	inline virtual void TraceBullets(TankNode* tank) = 0;
	inline virtual bool GetAlive() { return isAlive; }
	inline virtual void HitByBullet(int damage) = 0;
};