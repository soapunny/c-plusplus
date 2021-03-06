#include "Tank.h"
#include "RGBColor.h"
#include "Bullet.h"
#include <ctime>

int Tank::NAME_NUM;


HRESULT Tank::Init()
{
	width = 100;
	height = 100;
	if(name.size() < 1)
		name = "탱크"+to_string(++NAME_NUM);
	pos.x = 100;
	pos.y = WND_HEIGHT*2/3;
	shape.left = pos.x - width/2;
	shape.top = pos.y - height/2;
	shape.right = pos.x + width / 2;
	shape.bottom = pos.y + height / 2;
	maxHp = 5;
	currentHp = 5;
	isAlive = true;
	speed = 10;
	maxBullet = 5;
	direction = MOVE::MOVE_DOWN;
	color = new RGBColor();
	color->ShuffleColor();

	//x = cos(세타) * h
	//y = sin(세타) * h
	barrelSize = 70; 
	barrelAngle = 3.14f*2*3 / 4; // 90도
	barrelLeft.x = pos.x + cosf(barrelAngle + PI / 4) * 10;
	barrelLeft.y = pos.y - sinf(barrelAngle + PI / 4) * 10;
	barrelRight.x = pos.x + cosf(barrelAngle - PI / 4) * 10;
	barrelRight.y = pos.y - sinf(barrelAngle - PI / 4) * 10;
	barrelLeftTop.x = barrelLeft.x + cosf(barrelAngle) * 10;
	barrelLeftTop.y = barrelLeft.y - sinf(barrelAngle) * 10;
	barrelRightTop.x = barrelRight.x  + cosf(barrelAngle) * 10;
	barrelRightTop.y = barrelRight.y - sinf(barrelAngle) * 10;

	//return E_FAIL;
	return S_OK;
}

void Tank::Render(HDC hdc)
{
	color->StartBrush(hdc);
	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	color->EndBrush(hdc);
	Rectangle(hdc, shape.left+width/4, shape.top+height/4, shape.right-width/4, shape.bottom-height/4);
	
	//HP 남은 개수 표기
	for (int i = 0; i < maxHp - currentHp; i++) {
		Rectangle(hdc, shape.left - 20, shape.top + 10 * (i + 2), shape.left - 10, shape.top + 10 * (i + 3));
	}
	color->StartBrush(hdc, 200, 0, 0);
	for (int i = maxHp - currentHp; i < maxHp; i++) {
		Rectangle(hdc, shape.left - 20, shape.top + 10 * (i + 2), shape.left - 10, shape.top + 10 * (i + 3));
	}
	color->EndBrush(hdc);

	//불릿 남은 개수 표기
	for (int i = 0; i < bullets.size(); i++) {
		Rectangle(hdc, shape.right + 10, shape.top + 10 * (i + 2), shape.right + 20, shape.top + 10 * (i + 3));
	}
	color->StartBrush(hdc, 90, 90, 90);
	for (int i = bullets.size(); i < maxBullet; i++) {
		Rectangle(hdc, shape.right + 10, shape.top + 10 * (i + 2), shape.right + 20, shape.top + 10 * (i + 3));
	}
	color->EndBrush(hdc);

	RenderBarrel(hdc);

	SetBkMode(hdc, 1);
	TextOut(hdc, pos.x - strlen(name.c_str())*3, pos.y + 30, name.c_str(), strlen(name.c_str()));

	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
		(*iter)->Render(hdc);
	}
}

void Tank::RenderRandom(HDC hdc)
{
	color->ShuffleColor();
	color->StartBrush(hdc);
	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	color->EndBrush(hdc);
	Rectangle(hdc, shape.left + width / 4, shape.top + height / 4, shape.right - width / 4, shape.bottom - height / 4);

	//HP 남은 개수 표기
	for (int i = 0; i < maxHp - currentHp; i++) {
		Rectangle(hdc, shape.left - 20, shape.top + 10 * (i + 2), shape.left - 10, shape.top + 10 * (i + 3));
	}
	color->StartBrush(hdc, 200, 0, 0);
	for (int i = maxHp - currentHp; i < maxHp; i++) {
		Rectangle(hdc, shape.left - 20, shape.top + 10 * (i + 2), shape.left - 10, shape.top + 10 * (i + 3));
	}
	color->EndBrush(hdc);

	//불릿 남은 개수 표기
	for(int i =0;i<bullets.size();i++){
		Rectangle(hdc, shape.right + 10, shape.top + 10*(i+2), shape.right + 20, shape.top + 10*(i+3));
	}
	color->StartBrush(hdc, 90, 90, 90);
	for (int i = bullets.size(); i < maxBullet; i++) {
		Rectangle(hdc, shape.right + 10, shape.top + 10 * (i + 2), shape.right + 20, shape.top + 10 * (i + 3));
	}
	color->EndBrush(hdc);

	RenderBarrel(hdc);

	SetBkMode(hdc, 1);
	TextOut(hdc, pos.x - strlen(name.c_str()) * 4, pos.y + 30, name.c_str(), strlen(name.c_str()));

	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
		(*iter)->Render(hdc);
	}
}

void Tank::RenderBarrel(HDC hdc)
{
	//
	MoveToEx(hdc, barrelLeftTop.x, barrelLeftTop.y, NULL);
	LineTo(hdc, barrelRightTop.x, barrelRightTop.y);
	LineTo(hdc, barrelRight.x, barrelRight.y);
	LineTo(hdc, barrelLeft.x, barrelLeft.y);
	LineTo(hdc, barrelLeftTop.x, barrelLeftTop.y);

}

void Tank::RotateBarrel(float angle)
{
	barrelAngle += angle;
}

void Tank::Update()
{
	//x = cos(세타) * h
	//y = sin(세타) * h
	//베럴 업데이트
	FlyBullet();
	barrelLeft.x = pos.x + cosf(barrelAngle + PI / 4) * 10;
	barrelLeft.y = pos.y - sinf(barrelAngle + PI / 4) * 10;
	barrelRight.x = pos.x + cosf(barrelAngle - PI / 4) * 10;
	barrelRight.y = pos.y - sinf(barrelAngle - PI / 4) * 10;
	barrelLeftTop.x = barrelLeft.x + cosf(barrelAngle) * barrelSize;
	barrelLeftTop.y = barrelLeft.y - sinf(barrelAngle) * barrelSize;
	barrelRightTop.x = barrelRight.x + cosf(barrelAngle) * barrelSize;
	barrelRightTop.y = barrelRight.y - sinf(barrelAngle) * barrelSize;
}

void Tank::Release()
{
	if (color != nullptr)
		delete color;
	color = nullptr;
}

void Tank::Fire()
{
	if (bullets.size() >= maxBullet) {
		return;
	}
	Bullet* bullet = new Bullet();
	(*bullet).Init();
	(*bullet).SetFlyingAngle(barrelAngle);
	(*bullet).SetPos({ (LONG)(pos.x + cosf(barrelAngle) * (barrelSize+10)) , (LONG)(pos.y - sinf(barrelAngle) * barrelSize) });
	bullets.push_back(bullet);
}

void Tank::FlyBullet()
{
	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end();iter++) {
		(*iter)->Move();
	}
}

void Tank::RemoveBullet(Tank* tank)
{
	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end();) {
		if ((*iter)->GetPos().x < 0 || (*iter)->GetPos().x > WND_WIDTH || (*iter)->GetPos().y < 0 || (*iter)->GetPos().y > WND_HEIGHT) {
			(*iter)->Release();
			delete (*iter);
			iter = bullets.erase(iter);
		}
		else if (tank->GetAlive() && (*iter)->GetPos().x >= tank->GetPos().x - tank->GetWidth() / 2 && (*iter)->GetPos().x <= tank->GetPos().x + tank->GetWidth() / 2
			&& (*iter)->GetPos().y <= tank->GetPos().y + tank->GetHeight() / 2 && (*iter)->GetPos().y >= tank->GetPos().y - tank->GetHeight() / 2){
			tank->HitByBullet((*iter)->GetDamage());
			(*iter)->Release();
			delete (*iter);
			iter = bullets.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void Tank::Move(MOVE move)
{
	switch (move) {
	case MOVE::MOVE_LEFT:
		pos.x -= speed;
		direction = MOVE::MOVE_LEFT;
		break;
	case MOVE::MOVE_UP:
		pos.y -= speed;
		direction = MOVE::MOVE_UP;
		break;
	case MOVE::MOVE_RIGHT:
		pos.x += speed;
		direction = MOVE::MOVE_RIGHT;
		break;
	case MOVE::MOVE_DOWN:
		pos.y += speed;
		direction = MOVE::MOVE_DOWN;
		break;
	case MOVE::MOVE_NOWHERE:
		break;
	}
	
	shape.left = pos.x - width / 2;
	shape.top = pos.y - height / 2;
	shape.right = pos.x + width / 2;
	shape.bottom = pos.y + height / 2;
}

void Tank::MoveRandom()
{
	srand(time(NULL));
	int randNum = (rand() % 4);
	switch (randNum) {
	case 0:
		pos.x += speed * 2;
		if (pos.x >= WND_WIDTH) {
			pos.x -= speed * 2;
			randNum = 1;
		}
		break;
	case 1:
		pos.x -= speed * 2;
		if (pos.x <= 0) {
			pos.x += speed * 2;
			randNum = 2;
		}
		break;
	case 2:
		pos.y += speed * 2;
		if (pos.y >= WND_HEIGHT) {
			pos.y -= speed * 2;
			randNum = 3;
		}
		break;
	case 3:
		pos.y -= speed * 2;
		if (pos.y <= 0) {
			pos.y += speed * 2;
		}
		break;
	}
	shape.left = pos.x - width / 2;
	shape.top = pos.y - height / 2;
	shape.right = pos.x + width / 2;
	shape.bottom = pos.y + height / 2;
}

void Tank::Die()
{
	isAlive = false;
}

void Tank::MoveBarrelTo(POINT pos)
{
	float width = float(pos.x) - this->pos.x;
	if (width < 0) width = -width;
	float distance = sqrt(float(pow(width, 2)) + pow(pos.y - this->pos.y, 2));
	if(pos.y <= this->pos.y && pos.x >= this->pos.x)
		barrelAngle = acos(width / distance);
	else if (pos.y <= this->pos.y && pos.x <= this->pos.x) {
		barrelAngle = PI - acos(width / distance);
	}
	else if (pos.y >= this->pos.y && pos.x >= this->pos.x) {
		barrelAngle = PI*2 - acos(width / distance);
	}
	else if (pos.y >= this->pos.y && pos.x <= this->pos.x) {
		barrelAngle = PI + acos(width / distance);
	}
}

void Tank::SetShape(POINT pos)
{
	this->pos = pos;
	shape.left = pos.x - width / 2;
	shape.top = pos.y - height / 2;
	shape.right = pos.x + width / 2;
	shape.bottom = pos.y + height / 2;
}

void Tank::HitByBullet(int damage)
{
	currentHp -= damage;
	if (currentHp <= 0) {
		isAlive = false;
	}
}
