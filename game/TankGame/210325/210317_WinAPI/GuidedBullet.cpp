#include "GuidedBullet.h"
#include "RGBColor.h"

HRESULT GuidedBullet::Init()
{
	bulletType = BULLET::GUIDED_BULLET; 
	pos.x = 100;
	pos.y = 100;
	width = 20;
	height = 20;
	speed = 10;
	flyingAngle = PI / 2; 
	targetAngle = PI / 2;
	damage = 1;
	color = new RGBColor();
	(*color).ChangeColor(90, 90, 90);

	//return E_FAIL;
	return S_OK;
}

void GuidedBullet::Move()
{
	if(targetAngle < PI){//적이 위에 있을때
		if(startAngle )
		if (startAngle - targetAngle > PI || startAngle - targetAngle < 0) {
			/*if (!(targetAngle - PI / 18 < flyingAngle && flyingAngle < targetAngle + PI / 18)) {*/
			if (!(targetAngle < flyingAngle && flyingAngle < targetAngle + 2* PI / 18)) {
				flyingAngle += PI / 90;// 2 degree
			}
		}else{
			//if (!(targetAngle - PI / 18 < flyingAngle && flyingAngle < targetAngle + PI / 18)) {
			if (!(targetAngle - 2 * PI / 18 < flyingAngle && flyingAngle < targetAngle)) {
				flyingAngle -= PI / 90;
			}
		}
	}
	else {
		if (0 < targetAngle - startAngle && targetAngle - startAngle < PI) {
			if (!(targetAngle < flyingAngle && flyingAngle < targetAngle + 2 * PI / 18)) {
				flyingAngle += PI / 90;// 2 degree
			}
		}
		else {
			if (!(targetAngle - 2 * PI / 18 < flyingAngle && flyingAngle < targetAngle)) {
				flyingAngle -= PI / 90;
			}
		}
	}
	if(flyingAngle > 2*PI)
		flyingAngle -= (2 * PI);
	pos.x = pos.x + cosf(flyingAngle) * speed;
	pos.y = pos.y - sinf(flyingAngle) * speed;
}
