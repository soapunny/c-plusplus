#include "GuidedBullet.h"
#include "RGBColor.h"

HRESULT GuidedBullet::Init()
{
	bulletType = BULLET::GUIDED_BULLET;
	target = nullptr;
	targetAngle = 0.0f;
	pos.x = 100;
	pos.y = 100;
	width = 20;
	height = 20;
	speed = 10;
	flyingAngle = PI / 2;
	damage = 1;
	color = new RGBColor();
	(*color).ChangeColor(90, 90, 90);

	//return E_FAIL;
	return S_OK;
}

void GuidedBullet::Move()
{
	//atan2f�� �̿��� ���
	//if (target) {
	//	//atan2f(y,x) 1��и� 2��и�, 3��и� 4��и��� ���� ���� ������ ���
	//	targetAngle = GetAngle(pos, target->GetPos());
	//	distance = GetDistance(pos, target->GetPos());
	//	float angleGap = (targetAngle - flyingAngle) * (float)speed * 6 / distance;

	//	if (-0.05f < angleGap && angleGap < 0.05f)
	//	{
	//		angleGap = targetAngle;
	//	}
	//	else if (angleGap < 0)
	//	{
	//		angleGap += 0.05f;
	//	}
	//	else if (angleGap > 0)
	//	{
	//		angleGap -= 0.05f;
	//	}
	//	flyingAngle += angleGap;
	//	pos.x += cosf(flyingAngle) * speed;
	//	pos.y -= sinf(flyingAngle) * speed;
	//}
	//else {
	//	pos.x += speed;
	//}

	//atanf�� �̿��� ���
	if(target){
		targetAngle = atanf(sqrt(pow(pos.y - target->GetPos().y, 2)) / sqrt(pow(pos.x - target->GetPos().x, 2)));
		if (pos.x > target->GetPos().x && pos.y > target->GetPos().y) {
			targetAngle = PI - targetAngle ;
		}
		else if (pos.x > target->GetPos().x && pos.y < target->GetPos().y) {
			targetAngle = PI + targetAngle;
		}
		else if (pos.x < target->GetPos().x && pos.y < target->GetPos().y) {
			targetAngle = 3*PI/2 + targetAngle;
		}
		distance = GetDistance(pos, target->GetPos());
		float angleGap = 0;

		//1��и�
		if (0 <= targetAngle && targetAngle < PI / 2){
			if (targetAngle+PI <= flyingAngle && flyingAngle < 2*PI || 0 <= flyingAngle && flyingAngle < targetAngle) {
				//angleGap = - (float)speed + 6 / distance;
				angleGap = +0.05f;
			}
			else {
				//angleGap = (float)speed + 6 / distance;
				angleGap = -0.05f;
			}
		}//2��и�
		else if (PI/2 <= targetAngle && targetAngle < PI){
			if (targetAngle+PI <= flyingAngle && flyingAngle < 2*PI || 0 <= flyingAngle && flyingAngle < targetAngle) {
				//angleGap = -(float)speed + 6 / distance;
				angleGap = +0.05f;
			}
			else {
				angleGap = -0.05f;
			}
		}//3��и�
		else if (PI <= targetAngle && targetAngle < 3 * PI / 2) {
			if (targetAngle - PI <= flyingAngle && flyingAngle < targetAngle) {
				//angleGap = -(float)speed + 6 / distance;
				angleGap = +0.05f;
			}
			else {
				angleGap = -0.05f;
			}
		}//4��и�
		else if (PI*3/2 <= targetAngle && targetAngle < 2*PI) {
			if (targetAngle - PI <= flyingAngle && flyingAngle < targetAngle) {
				//angleGap = -(float)speed + 6 / distance;
				angleGap = +0.05f;
			}
			else {
				angleGap = -0.05f;
			}
		}

		if (-0.05f < targetAngle - flyingAngle && targetAngle - flyingAngle < 0.05f)
		{
			flyingAngle = targetAngle;
		}
		else {
			flyingAngle += angleGap;
		}

		if (flyingAngle >= 2 * PI) flyingAngle -= 2 * PI;
		else if (flyingAngle < 0) flyingAngle += 2 * PI;

		pos.x += cosf(flyingAngle) * speed;
		pos.y -= sinf(flyingAngle) * speed;
	}

}
