#include "GameProc.h"
#include "Tank.h"
#include "RGBColor.h"

#define H_TIMER 0
#define ENEMY1 1
#define ENEMY2 2
#define ENEMY3 3
#define ENEMY4 4
#define ENEMY5 5
#define ENEMY6 6
#define ENEMY7 7
#define ENEMY8 8
#define ENEMY9 9
#define ENEMY10 10
#define ENEMY_CREATE 20

HRESULT GameProc::Init()
{
	maxEnemyCnt = 10;
	enemyTimer = new HANDLE[maxEnemyCnt];
	hTimer = (HANDLE)SetTimer(g_hWnd, H_TIMER, 50, NULL);
	enemyCreateTimer = (HANDLE)SetTimer(g_hWnd, ENEMY_CREATE, 10000, NULL);
	for (int i = 0; i < maxEnemyCnt; i++) {
			/*enemyTimer[i] = new HANDLE;*/
			enemyTimer[i] = (HANDLE)SetTimer(g_hWnd, i+1, 2000 - 100*i ,NULL);
	}

	ptMouse.x = 0;
	ptMouse.y = 0;
	bgColor = new RGBColor();
	bgColor->SetR(135);
	bgColor->SetG(206);
	bgColor->SetB(235);
	myTank = new Tank("player");
	myTank->Init();
	myTank->SetPos({ WND_WIDTH / 2.0f, WND_HEIGHT / 2.0f });
	myTank->SetShape(myTank->GetPos());
	myTank->SetBarrelAngle(PI / 2);
	/*Tank* enemy = new Tank();
	enemy->Init();
	enemies.push_back(enemy);*/
	

	//return E_FAIL;
	return S_OK;
}

void GameProc::Render(HDC hdc)
{
	hdc = BeginPaint(g_hWnd, &ps);

	// ÀÎ»ç
	//TextOut(hdc, 20, 20, "´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ", strlen("´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ"));
	// ¸¶¿ì½º ÁÂÇ¥
	//wsprintf(szText, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
	//TextOut(hdc, 200, 20, szText, strlen(szText));

	//¹è°æ Ãâ·Â
	bgColor->StartBrush(hdc);
	Rectangle(hdc, 0, 0, WND_WIDTH, WND_HEIGHT);
	bgColor->EndBrush(hdc);

	if(myTank->GetAlive())
		myTank->RenderRandom(hdc);
	for (list<Tank*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
		if ((*iter)->GetAlive())
			(*iter)->Render(hdc);
	}

	EndPaint(g_hWnd, &ps);
}

void GameProc::Update()
{
	myTank->Update();
	for (list<Tank*>::iterator iter = enemies.begin(); iter != enemies.end();) {
		(*iter)->Update();
		//(*iter)->Move();
		(*iter)->MoveBarrelTo(myTank->GetPos());
		(*iter)->RemoveBullet(myTank);
		myTank->RemoveBullet((*iter));
		if (!(*iter)->GetAlive()) {
			(*iter)->Release();
			iter = enemies.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void GameProc::Release()
{
	myTank->Release();
	for (list<Tank*>::iterator iter = enemies.begin(); iter != enemies.end();) {
		(*iter)->Release();
		iter = enemies.erase(iter);
	}

	if (bgColor != nullptr)
		delete bgColor;
	bgColor = nullptr;

	KillTimer(g_hWnd, 0);
	for(int i =0;i<maxEnemyCnt;i++){
		if(enemyTimer[i] != nullptr){
			KillTimer(g_hWnd, i+1);
			delete enemyTimer[i];
		}
	}
	delete enemyTimer;
	KillTimer(g_hWnd, ENEMY_CREATE);
	
}

LRESULT GameProc::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_CREATE:
		Init();
		break;
	case WM_TIMER:
		RunTimer(wParam);
		break;
	case WM_KEYDOWN:
		KeyDown(wParam);
		break;
	case WM_LBUTTONDOWN:
		myTank->Fire();
		break;
	case WM_MOUSEMOVE:
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		myTank->MoveBarrelTo(ptMouse);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_PAINT:
		Render(hdc);
		break;
	case WM_DESTROY:
		Release();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void GameProc::KeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_SPACE:
		break;
	case 'w':case 'W':
		myTank->Move(MOVE::MOVE_UP);
		break;
	case 'a': case 'A':
		myTank->Move(MOVE::MOVE_LEFT);
		break;
	case 's':case 'S':
		myTank->Move(MOVE::MOVE_DOWN);
		break;
	case 'd': case 'D':
		myTank->Move(MOVE::MOVE_RIGHT);
		break;
	case '1':
		myTank->SetBulletType(BULLET::DEFAULT_BULLET);
		break;
	case '2':
		myTank->SetBulletType(BULLET::DYNAMITE_BULLET);
		break;
	}
	InvalidateRect(g_hWnd, NULL, true);
}

void GameProc::RunTimer(WPARAM wParam)
{
	switch (wParam) {
	case H_TIMER:
		Update();
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case ENEMY1:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[0] != nullptr) {
					KillTimer(g_hWnd, ENEMY1);
					delete enemyTimer[0];
					enemyTimer[0] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY2:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY2 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[1] != nullptr) {
					KillTimer(g_hWnd, ENEMY2);
					delete enemyTimer[1];
					enemyTimer[1] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY3:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY3 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[2] != nullptr) {
					KillTimer(g_hWnd, ENEMY3);
					delete enemyTimer[2];
					enemyTimer[2] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY4:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY4 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[3] != nullptr) {
					KillTimer(g_hWnd, ENEMY4);
					delete enemyTimer[3];
					enemyTimer[3] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY5:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY5 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[4] != nullptr) {
					KillTimer(g_hWnd, ENEMY5);
					delete enemyTimer[4];
					enemyTimer[4] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY6:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY6 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[5] != nullptr) {
					KillTimer(g_hWnd, ENEMY6);
					delete enemyTimer[5];
					enemyTimer[5] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY7:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY7 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[6] != nullptr) {
					KillTimer(g_hWnd, ENEMY7);
					delete enemyTimer[6];
					enemyTimer[6] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY8:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY8 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[7] != nullptr) {
					KillTimer(g_hWnd, ENEMY8);
					delete enemyTimer[7];
					enemyTimer[7] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY9:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY9 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else {
				if (enemyTimer[8] != nullptr) {
					KillTimer(g_hWnd, ENEMY9);
					delete enemyTimer[8];
					enemyTimer[8] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY10:
		if (enemies.size() > 0) {
			list<Tank*>::iterator iter = enemies.begin();
			for (int i = 0; i < ENEMY10 - 1; i++) {
				iter++;
				if (iter == enemies.end())
					return;
			}
			if ((*iter)->GetAlive()) {
				(*iter)->Move();
				(*iter)->Fire();
			}
			/*else{
				if(enemyTimer[9] != nullptr){
					KillTimer(g_hWnd, ENEMY10);
					delete enemyTimer[9];
					enemyTimer[9] = nullptr;
				}
			}*/
		}
		break;
	case ENEMY_CREATE:
		if(enemies.size() < maxEnemyCnt){
			Tank* enemy = new Tank();
			enemy->Init();
			//enemy->SetShape(FPOINT{ WND_WIDTH / 2, WND_HEIGHT / 5 });
			if (myTank->GetNameNum() % 4 == 3)
				enemy->SetBulletType(BULLET::DYNAMITE_BULLET);
			enemies.push_back(enemy);

		}
		break;
	}
}