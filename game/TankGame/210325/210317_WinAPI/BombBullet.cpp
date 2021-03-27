#include "BombBullet.h"
#include "RGBColor.h"
#include "Tank.h"

HRESULT BombBullet::Init()
{
    bulletType = BULLET::BOMB_BULLET;
    pos.x = 0;
    pos.y = 0;
    width = 20;
    height = 20;
    speed = 10;
    flyingAngle = PI / 2;
    damage = 1;
    color = new RGBColor();
    (*color).ChangeColor(90, 90, 90);
    countDown = 5;
    hasExploded = false;
    maxBulletCount = 18;

    return S_OK;
}

void BombBullet::Render(HDC hdc)
{
    if (hasExploded) {//����������
        color->StartBrush(hdc);
        for (list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end();iter++) {
            iter->Render(hdc);
        }
        color->EndBrush(hdc);
    }
    else {//�ƴϸ�
        color->StartBrush(hdc);
        Ellipse(hdc, pos.x - width / 2, pos.y - height / 2, pos.x + width / 2, pos.y + height / 2);
        color->EndBrush(hdc);
    }
}

void BombBullet::Update()
{
    Move();
}

void BombBullet::Release()
{
    for (list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end();) {
        iter = bullets.erase(iter);
    }
    if (color != nullptr) {
        delete color;
        color = nullptr;
    }
}

void BombBullet::Move()
{
    if (hasExploded) {//����������
        for (list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
            iter->Move();
        }
    }
    else {//�ƴϸ�
        pos.x = pos.x + cosf(flyingAngle) * speed;
        pos.y = pos.y - sinf(flyingAngle) * speed;
        if (--countDown <= 0) {
            hasExploded = true;
            for (int i = 0; i < maxBulletCount; i++) {
                Bullet bullet;
                bullet.Init();
                bullet.SetFlyingAngle(0.1744444f*i*2);
                bullet.SetPos(pos);
                bullets.push_back(bullet);
            }
            //�̻��� ź�δ� �� ������ ������.
            this->pos.x = -900;
            this->pos.y = -900;
        };
    }
}

void BombBullet::TraceBullet(TankNode* tank)
{
    for (list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end();) {
        if ((iter)->GetPos().x < 0 || (iter)->GetPos().x > WND_WIDTH || (iter)->GetPos().y < 0 || (iter)->GetPos().y > WND_HEIGHT) {
            (iter)->Release();
            iter = bullets.erase(iter);
        }
        else if (tank->GetAlive() && (iter)->GetPos().x >= tank->GetPos().x - tank->GetWidth() / 2 && (iter)->GetPos().x <= tank->GetPos().x + tank->GetWidth() / 2
            && (iter)->GetPos().y <= tank->GetPos().y + tank->GetHeight() / 2 && (iter)->GetPos().y >= tank->GetPos().y - tank->GetHeight() / 2) {
            tank->HitByBullet((iter)->GetDamage());
            (iter)->Release();
            iter = bullets.erase(iter);
        }
        else {
            iter++;
        }
    }
}
