#pragma once
#include "TankNode.h"

class Tank: public TankNode
{
private:
//Fields
	int fireCnt;
	static int NAME_NUM;
	int movePattern;

//Functions
protected:
	void Explode();

public:
	inline Tank() {}
	inline Tank(string name) { this->name = name; }
	inline ~Tank() {}

	HRESULT Init();
	void Render(HDC hdc);
	void RenderRandom(HDC hdc);
	void RenderBarrel(HDC hdc);
	void Update();
	void Release();

	void Fire();
	void FireGuidedBullet(MovingNode* target);
	void TraceBullets(TankNode* tank);
	void Move(MOVE move);
	void Move();
	void MoveBarrelTo(FPOINT targetPos);
	//void RotateBarrel(float angle);

	inline void SetBarrelAngle(float barrelAngle) { this->barrelAngle = barrelAngle; }
	inline void SetBulletType(BULLET bulletType1, BULLET bulletType2) { this->bulletType1 = bulletType1; this->bulletType2 = bulletType2; }
	inline void SetHp(int maxHp) { this->maxHp = maxHp; currentHp = maxHp; }
	inline void SetKillCnt(int killCnt) { this->killCnt = killCnt; }
	void SetTankLevel(TANK_LEVEL level);
	void SetShape(FPOINT pos);
	inline bool GetAlive() { return isAlive; }
	inline int GetKillCnt() { return killCnt; }
	inline int GetNameNum() { return NAME_NUM; }
	inline BULLET GetBulletType1() { return bulletType1; }
	void HitByBullet(int damage);
};

