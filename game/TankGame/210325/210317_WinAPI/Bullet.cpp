#include "Bullet.h"
#include "RGBColor.h"

HRESULT Bullet::Init()
{
	pos.x = 100;
	pos.y = 100;
	width = 20;
	height = 20;
	speed = 10; 
	flyingAngle = PI/2;
	damage = 1;
	color = new RGBColor();
	(*color).ChangeColor(90, 90, 90);

	//return E_FAIL;
	return S_OK;
}

void Bullet::Render(HDC hdc)
{
	color->StartBrush(hdc);
	Ellipse(hdc, pos.x - width / 2, pos.y - height / 2, pos.x + width / 2, pos.y + height / 2);
	color->EndBrush(hdc);
}

void Bullet::Update()
{

}

void Bullet::Release()
{
	if(color != nullptr)
		delete color;
	color = nullptr;
}

void Bullet::Move()
{
	pos.x = pos.x + cosf(flyingAngle) * speed;
	pos.y = pos.y - sinf(flyingAngle) * speed;
}

