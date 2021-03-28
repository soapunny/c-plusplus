#include <ctime>
#include "GameProc.h"
#include "Tank.h"
#include "RGBColor.h"

#define UPDATE 0
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
	try{
		updateTimer = (HANDLE)SetTimer(g_hWnd, UPDATE, 50, NULL);
		enemyCreateTimer = (HANDLE)SetTimer(g_hWnd, ENEMY_CREATE, 10000, NULL);
		maxEnemyCnt = 10;
		enemyTimer = new HANDLE[maxEnemyCnt];
		for (int i = 0; i < maxEnemyCnt; i++) {
			/*enemyTimer[i] = new HANDLE;*/
			enemyTimer[i] = (HANDLE)SetTimer(g_hWnd, ENEMY1 + i, 2000 - 150 * i, NULL);
		}
		color = new RGBColor();
		color->SetR(135);
		color->SetG(206);
		color->SetB(235);
		ptMouse.x = 0;
		ptMouse.y = 0;

		stage = 1;
		isBoss = false;
		hasCleared = false;
		myTank = new Tank("player");
		myTank->Init();
		myTank->SetPos({ WND_WIDTH / 2.0f, WND_HEIGHT*4 / 5.0f });
		myTank->SetShape(myTank->GetPos());
		myTank->SetBarrelAngle(PI / 2);
		//TODO 내 탱크 총알 색상만 다르게
		Tank* enemy = new Tank();
		enemy->Init();
		enemy->SetTankLevel(TANK_LEVEL::WEAKEST_LEVEL);
		enemies.push_back(enemy);
	}
	catch (exception e) {
		//e.what();
		return E_FAIL;
	}
	
	return S_OK;
}

void GameProc::Render(HDC hdc)
{
	hdc = BeginPaint(g_hWnd, &ps);

	//배경 출력
	color->StartBrush(hdc);
	Rectangle(hdc, 0, 0, WND_WIDTH, WND_HEIGHT);
	color->EndBrush(hdc);

	if(myTank->GetAlive())
		myTank->RenderRandom(hdc);
	for (list<Tank*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
		if ((*iter)->GetAlive())
			(*iter)->Render(hdc);
	}

	//킬 카운트
	wsprintf(szText, "Stage %d", stage);
	TextOut(hdc, 20, 20, szText, strlen(szText));
	wsprintf(szText, "[Kill Count : %d]", myTank->GetKillCnt());
	TextOut(hdc, 20, 40, szText, strlen(szText));
	if (isBoss) {
		SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 20, 60, "BOSS", strlen("BOSS"));
		SetTextColor(hdc, RGB(0, 0, 0));
	}
	EndPaint(g_hWnd, &ps);
}

void GameProc::Update()
{
	myTank->Update();
	if (enemies.size() <= 0) {
		myTank->TraceBullets(nullptr);
	}else{
		for (list<Tank*>::iterator iter = enemies.begin(); iter != enemies.end();) {
			(*iter)->Update();
			(*iter)->MoveBarrelTo(myTank->GetPos());
			(*iter)->TraceBullets(myTank);
			myTank->TraceBullets((*iter));
			if (!(*iter)->GetAlive()) {
				(*iter)->Release();
				iter = enemies.erase(iter);
			}
			else {
				iter++;
			}
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

	if (color != nullptr)
		delete color;
	color = nullptr;

	KillTimer(g_hWnd, 0);
	for(int i =0;i<maxEnemyCnt;i++){
		if(enemyTimer[i] != nullptr){
			KillTimer(g_hWnd, (UINT_PTR)ENEMY1+i);
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
		break;
	case WM_TIMER:
		RunTimer(wParam);
		break;
	case WM_KEYDOWN:
		KeyDown(wParam);
		break;
	case WM_LBUTTONDOWN:
		if (myTank->GetBulletType1() == BULLET::GUIDED_BULLET && enemies.size() > 0) {
			myTank->FireGuidedBullet((*enemies.begin())->GetBarrelAngle() < PI ? (*enemies.begin())->GetBarrelAngle() + PI : (*enemies.begin())->GetBarrelAngle() - PI
				, (*enemies.begin())->GetPos());
		}else{
			myTank->Fire();
		}
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
		myTank->SetBulletType(BULLET::DEFAULT_BULLET, BULLET::DEFAULT_BULLET);
		break;
	case '2':
		myTank->SetBulletType(BULLET::BOMB_BULLET, BULLET::BOMB_BULLET);
		break;
	case '3':
		myTank->SetBulletType(BULLET::GUIDED_BULLET, BULLET::GUIDED_BULLET);
		break;
	}
	InvalidateRect(g_hWnd, NULL, true);
}

void GameProc::MoveEnemyByTimer(int timerId) {
	if (enemies.size() > 0) {
		list<Tank*>::iterator iter = enemies.begin();
		for (int i = 0; i < timerId - 1; i++) {
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
}

void GameProc::RunTimer(WPARAM wParam)
{
	switch (wParam) {
	case UPDATE:
		Update();
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case ENEMY1:
		MoveEnemyByTimer(ENEMY1);
		break;
	case ENEMY2:
		MoveEnemyByTimer(ENEMY2);
		break;
	case ENEMY3:
		MoveEnemyByTimer(ENEMY3);
		break;
	case ENEMY4:
		MoveEnemyByTimer(ENEMY4);
		break;
	case ENEMY5:
		MoveEnemyByTimer(ENEMY5);
		break;
	case ENEMY6:
		MoveEnemyByTimer(ENEMY6);
		break;
	case ENEMY7:
		MoveEnemyByTimer(ENEMY7);
		break;
	case ENEMY8:
		MoveEnemyByTimer(ENEMY8);
		break;
	case ENEMY9:
		MoveEnemyByTimer(ENEMY9);
		break;
	case ENEMY10:
		MoveEnemyByTimer(ENEMY10);
		break;
	case ENEMY_CREATE:
		CreateEnemy();
		break;
	}
}

void GameProc::CreateEnemy()
{
	if (enemies.size() < maxEnemyCnt) {
		srand(time(NULL)); 
		Tank* enemy = nullptr;
		//enemy->SetShape(FPOINT{ WND_WIDTH / 2, WND_HEIGHT / 5 });
		if (stage == 1) {
			if (myTank->GetKillCnt() < 3) {
				if (myTank->GetKillCnt() + enemies.size() >= 3) return;
				enemy = new Tank();
				enemy->Init();
				if (rand() % 4 == 3)
					enemy->SetTankLevel(TANK_LEVEL::WEAK_LEVEL);
				else
					enemy->SetTankLevel(TANK_LEVEL::WEAKEST_LEVEL);
				enemies.push_back(enemy);
			}
			else if (!isBoss) {
				//TODO 보스 세팅
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::NORMAL_LEVEL);
				enemies.push_back(enemy);
				isBoss = true;
				hasCleared = true;
			}
			else if (hasCleared) {
				if (enemies.size() > 0) return;
				myTank->SetKillCnt(0);
				isBoss = false;
				hasCleared = false;
				stage++;
			}
		}
		else if (stage == 2) {
			if (myTank->GetKillCnt() < 3) {
				if (myTank->GetKillCnt() + enemies.size() >= 3) return;
				enemy = new Tank();
				enemy->Init();
				if (rand() % 4 == 3)
					enemy->SetTankLevel(TANK_LEVEL::NORMAL_LEVEL);
				else
					enemy->SetTankLevel(TANK_LEVEL::WEAK_LEVEL);
				enemies.push_back(enemy);
			}
			else if (!isBoss) {
				//TODO 보스 세팅
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::STRONG_LEVEL);
				enemies.push_back(enemy);
				isBoss = true;
				hasCleared = true;
			}
			else if (hasCleared) {
				if (enemies.size() > 0) return;
				myTank->SetKillCnt(0);
				isBoss = false;
				hasCleared = false;
				stage++;
			}
		}
		else if (stage == 3) {
			if (myTank->GetKillCnt() < 3) {
				if (myTank->GetKillCnt() + enemies.size() >= 3) return;
				enemy = new Tank();
				enemy->Init();
				if (rand() % 4 == 3)
					enemy->SetTankLevel(TANK_LEVEL::STRONG_LEVEL);
				else
					enemy->SetTankLevel(TANK_LEVEL::NORMAL_LEVEL);
				enemies.push_back(enemy);
			}
			else if (!isBoss) {
				//TODO 보스 세팅
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::AWFUL_LEVEL);
				enemies.push_back(enemy);
				isBoss = true;
				hasCleared = true;
			}
			else if (hasCleared) {
				if (enemies.size() > 0) return;
				myTank->SetKillCnt(0);
				isBoss = false;
				hasCleared = false;
				stage++;
			}
		}
		else if (stage == 4) {
			if (myTank->GetKillCnt() < 3) {
				if (myTank->GetKillCnt() + enemies.size() >= 3) return;
				enemy = new Tank();
				enemy->Init();
				if (rand() % 4 == 3)
					enemy->SetTankLevel(TANK_LEVEL::AWFUL_LEVEL);
				else
					enemy->SetTankLevel(TANK_LEVEL::STRONG_LEVEL);
				enemies.push_back(enemy);
			}
			else if (!isBoss) {
				//TODO 보스 세팅
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::HIGHST_LEVEL);
				enemies.push_back(enemy);
				isBoss = true;
				hasCleared = true;
			}
			else if (hasCleared) {
				if (enemies.size() > 0) return;
				myTank->SetKillCnt(0);
				isBoss = false;
				hasCleared = false;
				stage++;
			}
		}
		else if (stage == 5) {
			if (myTank->GetKillCnt() < 3) {
				if (myTank->GetKillCnt() + enemies.size() >= 3) return;
				enemy = new Tank();
				enemy->Init();
				if (rand() % 4 == 3)
					enemy->SetTankLevel(TANK_LEVEL::HIGHST_LEVEL);
				else
					enemy->SetTankLevel(TANK_LEVEL::AWFUL_LEVEL);
				enemies.push_back(enemy);
			}
			else if (!isBoss) {
				//TODO 보스 세팅
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::HIGHST_LEVEL);
				enemies.push_back(enemy);
				enemy = new Tank();
				enemy->Init();
				enemy->SetTankLevel(TANK_LEVEL::HIGHST_LEVEL);
				enemies.push_back(enemy);
				isBoss = true;
				hasCleared = true;
			}
			else if (hasCleared) {
				if (enemies.size() > 0) return;
				myTank->SetKillCnt(0);
				isBoss = false;
				hasCleared = false;
				stage++;
			}
		}
	}
}