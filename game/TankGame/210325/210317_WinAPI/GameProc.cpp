#include "GameProc.h"
#include "RGBColor.h"

HRESULT GameProc::Init()
{
	bgColor = new RGBColor();
	bgColor->SetR(135);
	bgColor->SetG(206);
	bgColor->SetB(235);
	tank1 = { "player1" };
	tank1.Init();
	tank1.SetBarrelAngle(PI / 2);
	enemy1.Init();
	enemy1.SetShape(POINT{ WND_WIDTH/2, WND_HEIGHT/5 });

	//return E_FAIL;
	return S_OK;
}

void GameProc::Render(HDC hdc)
{
	hdc = BeginPaint(g_hWnd, &ps);

	// ÀÎ»ç
	//TextOut(hdc, 20, 20, "´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ", strlen("´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ"));
	// ¸¶¿ì½º ÁÂÇ¥
	wsprintf(szText, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
	TextOut(hdc, 200, 20, szText, strlen(szText));

	//¹è°æ Ãâ·Â
	bgColor->StartBrush(hdc);
	Rectangle(hdc, 0, 0, WND_WIDTH, WND_HEIGHT);
	bgColor->EndBrush(hdc);

	if(tank1.GetAlive())
		tank1.RenderRandom(hdc);
	if(enemy1.GetAlive())
		enemy1.Render(hdc);


	EndPaint(g_hWnd, &ps);
}

void GameProc::Update()
{
	tank1.Update();
	enemy1.Update();
	enemy1.MoveRandom();
	enemy1.MoveBarrelTo(tank1.GetPos());
	tank1.RemoveBullet(&enemy1);
	enemy1.RemoveBullet(&tank1);

}

void GameProc::Release()
{
	tank1.Release();
	enemy1.Release();

	if (bgColor != nullptr)
		delete bgColor;
	bgColor = nullptr;

	KillTimer(g_hWnd, 0);
}

LRESULT GameProc::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_CREATE:
		hTimer = (HANDLE)SetTimer(hWnd, 0, 50, NULL);
		enemyTimer = (HANDLE)SetTimer(hWnd, 1, 2000, NULL);
		break;
	case WM_TIMER:
		RunTimer(wParam);
		break;
	case WM_KEYDOWN:
		KeyDown(wParam);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
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
	case VK_LEFT:
		tank1.Move(MOVE::MOVE_LEFT);
		break;
	case VK_RIGHT:
		tank1.Move(MOVE::MOVE_RIGHT);
		break;
	case VK_UP:
		tank1.Move(MOVE::MOVE_UP);
		break;
	case VK_DOWN:
		tank1.Move(MOVE::MOVE_DOWN);
		break;
	case VK_SPACE:
		tank1.Fire();
		break;
	case 'a': case 'A':
		tank1.RotateBarrel(+5 * PI / 180);
		break;
	case 'd': case 'D':
		tank1.RotateBarrel(-5 * PI / 180);
		break;
	}
	InvalidateRect(g_hWnd, NULL, true);
}

void GameProc::RunTimer(WPARAM wParam)
{
	switch (wParam) {
	case 0:
		Update();

		break;
	case 1:
		enemy1.Fire();
		break;
	}
	InvalidateRect(g_hWnd, NULL, true);
}
