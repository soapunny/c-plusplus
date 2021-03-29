#include "Tank.h"
#include "RGBColor.h"
#include "BombBullet.h"
#include "GuidedBullet.h"
#include <ctime>

int Tank::NAME_NUM;

HRESULT Tank::Init()
{
	try{
		srand(time(NULL));
		width = 100;
		height = 100;
		killCnt = 0;
		fireCnt = 0;
		if(name.size() < 1)
			name = "Tank"+to_string(++NAME_NUM);
		movePattern = rand()%4;
		pos.x = rand()%WND_WIDTH;
		pos.y = -WND_HEIGHT/9;
		shape.left = pos.x - width/2;
		shape.top = pos.y - height/2;
		shape.right = pos.x + width / 2;
		shape.bottom = pos.y + height / 2;
		maxHp = 5;
		currentHp = maxHp;
		isAlive = true;
		speed = 10;
		maxBullet = 5;
		direction = MOVE::MOVE_DOWN;
		bulletType1 = BULLET::DEFAULT_BULLET;
		bulletType2 = BULLET::DEFAULT_BULLET;
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
	}
	catch (exception e) {
		return E_FAIL;
	}
	return S_OK;
}

void Tank::Render(HDC hdc)
{
	color->StartBrush(hdc);
	Rectangle(hdc, shape.left, shape.top, shape.right, shape.bottom);
	color->EndBrush(hdc);
	Rectangle(hdc, shape.left+width/4, shape.top+height/4, shape.right-width/4, shape.bottom-height/4);
	
	//HP 남은 개수 표기
	int hpLeft = 0;
	for (int i = 0; i < maxHp - currentHp; i++) {
		hpLeft = (shape.left - 20) - 10 * ((maxHp - i - 1) / 5);
		Rectangle(hdc, hpLeft, shape.top + 10 * (i%5 + 2), hpLeft+10, shape.top + 10 * (i%5 + 3));
	}
	color->StartBrush(hdc, 200, 0, 0);
	for (int i = maxHp - currentHp; i < maxHp; i++) {
		hpLeft = (shape.left - 20) - 10 * ((maxHp - i - 1) / 5);
		Rectangle(hdc, hpLeft, shape.top + 10 * (i%5 + 2), hpLeft+10, shape.top + 10 * (i%5 + 3));
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
	int hpLeft = 0;
	for (int i = 0; i < maxHp - currentHp; i++) {
		hpLeft = (shape.left - 20) - 10 * ((maxHp - i - 1) / 5);
		Rectangle(hdc, hpLeft, shape.top + 10 * (i % 5 + 2), hpLeft + 10, shape.top + 10 * (i % 5 + 3));
	}
	color->StartBrush(hdc, 200, 0, 0);
	for (int i = maxHp - currentHp; i < maxHp; i++) {
		hpLeft = (shape.left - 20) - 10 * ((maxHp - i - 1) / 5);
		Rectangle(hdc, hpLeft, shape.top + 10 * (i % 5 + 2), hpLeft + 10, shape.top + 10 * (i % 5 + 3));
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

//void Tank::RotateBarrel(float angle)
//{
//	barrelAngle += angle;
//}

void Tank::Update()
{
	//Update Bullet's location
	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
		(**iter).Update();
	}

	//Update Barrel's Location
	//x = cos(theta) * h
	//y = sin(theta) * h
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
	else if (bullets.size() > 0) {//리스트에 아직 BombBullet이 남아있으면 더 이상 발사하지 못한다.
		for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end();iter++) {
			if ((*iter)->GetBulletType() == BULLET::BOMB_BULLET)
				return;
		}
	}
	Bullet* bullet = nullptr;
	if(fireCnt%2 == 0){
		if (bulletType1 == BULLET::DEFAULT_BULLET)
			bullet = new Bullet();
		else if (bulletType1 == BULLET::BOMB_BULLET)
			bullet = new BombBullet();
		else
			bullet = new Bullet();
	}
	else {
		if (bulletType2 == BULLET::DEFAULT_BULLET)
			bullet = new Bullet();
		else if (bulletType2 == BULLET::BOMB_BULLET)
			bullet = new BombBullet();
		else
			bullet = new Bullet();
	}
	(*bullet).Init();
	(*bullet).SetFlyingAngle(barrelAngle);
	(*bullet).SetPos({(pos.x + cosf(barrelAngle) * (barrelSize+10)) , (pos.y - sinf(barrelAngle) * barrelSize) });
	bullets.push_back(bullet);
	fireCnt++;
}

void Tank::FireGuidedBullet(MovingNode* target)
{
	if (bullets.size() >= maxBullet) {
		return;
	}
	else if (bullets.size() > 0) {
		for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
			if ((*iter)->GetBulletType() == BULLET::BOMB_BULLET)
				return;
		}
	}
	GuidedBullet* bullet = nullptr; 
	if (fireCnt % 2 == 0) {
		if (bulletType1 == BULLET::GUIDED_BULLET){
			bullet = new GuidedBullet();
		}
	}
	else {
		if (bulletType2 == BULLET::GUIDED_BULLET) {
			bullet = new GuidedBullet();
		}
	}
	bullet->Init();
	bullet->SetFlyingAngle(barrelAngle);
	bullet->SetTarget(target); // target의 주소값을 저장
	bullet->SetPos({ (pos.x + cosf(barrelAngle) * (barrelSize + 10)) , (pos.y - sinf(barrelAngle) * barrelSize) });
	bullets.push_back(bullet);
	fireCnt++;
}

void Tank::TraceBullets(TankNode* tank)
{
	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end();) {
		if ((*iter)->GetBulletType() == BULLET::BOMB_BULLET) {
			((BombBullet*)(*iter))->TraceBullet(this, tank);
			if (((BombBullet*)(*iter))->HasNoBullet()) {
				((BombBullet*)(*iter))->Release();
				iter = bullets.erase(iter);
			}else{
				iter++;
			}
		}
		else if((*iter)->GetPos().x < 0 || (*iter)->GetPos().x > WND_WIDTH || (*iter)->GetPos().y < 0 || (*iter)->GetPos().y > WND_HEIGHT) {
			(*iter)->Release();
			delete (*iter);
			iter = bullets.erase(iter);
		}
		else if (tank != nullptr && tank->GetAlive() && (*iter)->GetPos().x >= tank->GetPos().x - tank->GetWidth() / 2 && (*iter)->GetPos().x <= tank->GetPos().x + tank->GetWidth() / 2
			&& (*iter)->GetPos().y <= tank->GetPos().y + tank->GetHeight() / 2 && (*iter)->GetPos().y >= tank->GetPos().y - tank->GetHeight() / 2){
			tank->HitByBullet((*iter)->GetDamage());
			if (!tank->GetAlive())
				killCnt++;
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

void Tank::Move()
{
	if (pos.y <= 100) {
		pos.y += speed;
	}else{
		srand(time(NULL));
		movePattern = rand()%4;
		switch (movePattern) {
		case 0:
			pos.x += speed;
			if (pos.x >= WND_WIDTH) {
				pos.x -= speed;
				movePattern = 1;
			}
			break;
		case 1:
			pos.x -= speed;
			if (pos.x <= 0) {
				pos.x += speed;
				movePattern = 2;
			}
			break;
		case 2:
			pos.y += speed;
			if (pos.y >= WND_HEIGHT) {
				pos.y -= speed;
				movePattern = 3;
			}
			break;
		case 3:
			pos.y -= speed;
			if (pos.y <= 0) {
				pos.y += speed;
			}
			break;
		}
	}
	shape.left = pos.x - width / 2;
	shape.top = pos.y - height / 2;
	shape.right = pos.x + width / 2;
	shape.bottom = pos.y + height / 2;
}

void Tank::Explode()
{
	for (list<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end();) {
		(*iter)->Release();
		delete (*iter);
		iter = bullets.erase(iter);
	}
	isAlive = false;
}

void Tank::MoveBarrelTo(FPOINT pos)
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

void Tank::SetTankLevel(TANK_LEVEL level)
{
	switch (level) {
	case TANK_LEVEL::WEAKEST_LEVEL:
		color->ChangeColor(255, 255, 255);
		speed = 10;
		SetHp(5);
		SetBulletType(BULLET::DEFAULT_BULLET, BULLET::DEFAULT_BULLET);
		break;
	case TANK_LEVEL::WEAK_LEVEL:
		color->ChangeColor(255, 212, 0);
		speed = 20;
		SetHp(5);
		SetBulletType(BULLET::DEFAULT_BULLET, BULLET::DEFAULT_BULLET);
		break;
	case TANK_LEVEL::NORMAL_LEVEL:
		color->ChangeColor(129, 193, 71);
		speed = 30;
		SetHp(10);
		SetBulletType(BULLET::BOMB_BULLET, BULLET::DEFAULT_BULLET);
		break;
	case TANK_LEVEL::STRONG_LEVEL:
		color->ChangeColor(80, 188, 223);
		speed = 40;
		SetHp(10);
		SetBulletType(BULLET::BOMB_BULLET, BULLET::DEFAULT_BULLET);
		break;
	case TANK_LEVEL::AWFUL_LEVEL:
		color->ChangeColor(255, 0, 0);
		speed = 80;
		SetHp(15);
		SetBulletType(BULLET::BOMB_BULLET, BULLET::BOMB_BULLET);
		break;
	case TANK_LEVEL::HIGHST_LEVEL:
		color->ChangeColor(0, 0, 0);
		speed = 100;
		SetHp(20);
		SetBulletType(BULLET::BOMB_BULLET, BULLET::BOMB_BULLET);
		break;
	}
}

void Tank::SetShape(FPOINT pos)
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
		Explode();
	}
}
