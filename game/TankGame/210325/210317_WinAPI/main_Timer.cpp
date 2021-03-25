// main.cpp
#include <Windows.h>

// ��������
// �ν��Ͻ� : ��ü �޸𸮻� �����ϴ� ��ü
HINSTANCE g_hInstance;		// ���α׷� �ν��Ͻ� �ڵ�
HWND g_hWnd;				// ������ �ڵ�
LPSTR g_lpszClass = (LPSTR)TEXT("�������� ����");

struct Ball
{
	RECT shape;
	POINT pos;
};
Ball ball;
POINT ground;

// set
void SetBallAndGround();
//void SetBox();
//void SetNemo(BeingStar* stars, int count);
// getRect
RECT GetRect(int left, int top, int width, int height);
RECT GetRectToCenter(int posX, int posY, int width, int height);
// render
//void RenderBox(HDC hdc);
void RenderBallAndGround(HDC hdc);
void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectToCenter(HDC hdc, int x, int y, int width, int height);
void RenderEllipseToCenter(HDC hdc, int x, int y, int width, int height);
void RenderCar(HDC hdc, int posX, int posY);
void RenderStar(HDC hdc, int posX, int posY);
void RenderStar2(HDC hdc, int posX, int posY);
//void RenderNemo(HDC hdc, BeingStar* stars, int count);
// collisionCheck
bool PointInRect(POINT ptMouse, RECT rc);
bool PointInRect(POINT ptMouse, POINT rcPos, int rcSize);
bool RectInRect(RECT rc1, RECT rc2);
// calculator
float GetHeightFromParabolaMoving(float x/*, POINT currentPos*/);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance,
	LPSTR _lpszCmdParam, int nCmdShow)
{
	g_hInstance = _hInstance;

	// �����츦 �����ϱ� ���� �⺻����
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ���
	RegisterClass(&wndClass);

	// ������ ����
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, 
		WS_OVERLAPPEDWINDOW,
		50, 50, 1080, 720, 
		NULL, NULL, g_hInstance, NULL);

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);

	// �޽��� ť�� �ִ� �޽��� ó��
	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;			// handle - DC (Device Context)
	PAINTSTRUCT ps;

	// Ÿ�̸� ��ü�� �����ϴ� �ڵ�
	static HANDLE hTimer;

	static POINT ptMouse{0, 0};
	static bool isPtInRect = false;
	static int selectedId = -1;

	char szText[128] = "";

	switch (iMessage)
	{
	case WM_CREATE:
		hTimer = (HANDLE)SetTimer(hWnd, 0, 10, NULL);

		/*
			�ǽ� 1. �÷��̾�, �� �׸� �ϳ��� �����.
			�÷��̾�� W, SŰ�� ���Ϸ� �̵�
			���� �ڵ����� ���Ϸ� �̵�

			�ǽ� 2. ���콺 ���� ��ư�� ������ �̻��� �߻�
			���� ����Ÿ�ֿ̹� �̻����� �߻�

			�ǽ� 3. ���� �̻��Ͽ� ������ �������.
			(������ �°ų� ȭ�� ������ ����� �ٽ� ������)
		*/

		SetBallAndGround();
		break;
	case WM_TIMER:
		ball.pos.x += 1;
		ball.pos.y = 550 + GetHeightFromParabolaMoving(ball.pos.x - 100/*, ball.pos*/);

		ball.shape = GetRectToCenter(ball.pos.x, ball.pos.y, 100, 100);

		InvalidateRect(g_hWnd, NULL, true);

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			ball.pos.x += 15;
			ball.pos.y = 550 + GetHeightFromParabolaMoving(ball.pos.x - 100/*, ball.pos*/);

			ball.shape = GetRectToCenter(ball.pos.x, ball.pos.y, 100, 100);

			InvalidateRect(g_hWnd, NULL, true);
			break;
		}
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
		hdc = BeginPaint(g_hWnd, &ps);
		// �λ�
		TextOut(hdc, 20, 20, "Hello, WinMain!!", strlen("Hello, WinMain!!"));
		// ���콺 ��ǥ
		wsprintf(szText, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
		TextOut(hdc, 200, 20, szText, strlen(szText));
		// ���õ� ���� ���̵�
		wsprintf(szText, "Selected ID : %d", selectedId);
		TextOut(hdc, 400, 20, szText, strlen(szText));	

		RenderBallAndGround(hdc);

		EndPaint(g_hWnd, &ps);

		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

//void SetBox()
//{
//	rc[Box::First] = GetRectToCenter(200, 200, 200, 200);
//	rc[Box::Second] = GetRectToCenter(500, 200, 200, 200);
//	rcIn = GetRectToCenter(200, 200, 80, 80);
//}
//
//void RenderBox(HDC hdc)
//{
//	for (int i = 0; i < Box::End; i++)
//	{
//		Rectangle(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
//	}
//
//	Rectangle(hdc, rcIn.left, rcIn.top, rcIn.right, rcIn.bottom);
//}

void RenderBallAndGround(HDC hdc)
{
	MoveToEx(hdc, 0, ground.y, NULL);
	LineTo(hdc, ground.x, ground.y);

	Ellipse(hdc, ball.shape.left, ball.shape.top, ball.shape.right, ball.shape.bottom);
}

// �׸� �׸��� �Լ�
void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

void RenderRectToCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc,
		x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2));
}

void RenderEllipseToCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc,
		x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2));
}

void RenderCar(HDC hdc, int posX, int posY)
{
	// �ڵ��� ���
	RenderRectToCenter(hdc, posX, posY - 100, 300, 200);

	// �Ϻ�
	RenderRectToCenter(hdc, posX, posY + 150, 500, 300);

	// ����
	// ���� posX - 100, posY + 350, ������ posX + 100, posY + 350
	RenderEllipseToCenter(hdc, posX - 100, posY + 350, 100, 100);
	RenderEllipseToCenter(hdc, posX + 100, posY + 350, 100, 100);
}

void RenderStar(HDC hdc, int posX, int posY)
{
	MoveToEx(hdc, posX - 40, posY - 17, NULL);
	LineTo(hdc, posX + 40, posY - 17);
	LineTo(hdc, posX - 23, posY + 38);
	LineTo(hdc, posX, posY -50);
	LineTo(hdc, posX + 23, posY + 38);
	LineTo(hdc, posX - 40, posY - 17);
}

void RenderStar2(HDC hdc, int posX, int posY)
{
	int size = 100;
	MoveToEx(hdc, posX/* + (-0.6f * size)*/, posY + (-1.0f * size), NULL);
	LineTo(hdc, posX + (-0.6f * size), posY + (0.8f * size));
	LineTo(hdc, posX + (0.9f * size), posY + (-0.5f * size));
	LineTo(hdc, posX + (-0.9f * size), posY + (-0.5f * size));
	LineTo(hdc, posX + (0.6f * size), posY + (0.8f * size));
	LineTo(hdc, posX, posY + (-1.0f * size));
}

bool PointInRect(POINT ptMouse, RECT rc)
{
	if (ptMouse.x < rc.left || ptMouse.x > rc.right
		||ptMouse.y < rc.top || ptMouse.y > rc.bottom)
	{
		return false;
	}

	return true;
}

bool PointInRect(POINT ptMouse, POINT rcPos, int rcSize)
{
	if (ptMouse.x < rcPos.x - (rcSize / 2)
		|| ptMouse.x > rcPos.x + (rcSize / 2)
		|| ptMouse.y < rcPos.y - (rcSize / 2)
		|| ptMouse.y > rcPos.y + (rcSize / 2))
	{
		return false;
	}

	return true;
}

bool RectInRect(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left || rc1.left > rc2.right
		|| rc1.top > rc2.bottom || rc1.bottom < rc2.top)
		return false;

	return true;
}

float GetHeightFromParabolaMoving(float x/*, POINT currentPos*/)
{
	// y = a(x-h)^2 + k
	float y = 0.01f * ((x - 200.0f) * (x - 200.0f)) - 400.0f;
	if (y > 0)
		y = 0.0f;
	return /*currentPos.y + */y;
}

//void SetNemo(BeingStar* stars, int count)
//{
//	for (int i = 0; i < count; i++)
//	{
//		stars[i].ptPos.x = 50 + i * 120;
//		stars[i].ptPos.y = 600;
//		stars[i].size = 100;
//		stars[i].shape = Nemo_Shape::Nemo;
//	}
//}

void SetBallAndGround()
{
	ground.x = 1080;
	ground.y = 600;

	ball.pos.x = 100;
	ball.pos.y = 550;
	ball.shape = GetRectToCenter(ball.pos.x, ball.pos.y, 100, 100);
}

RECT GetRect(int left, int top, int width, int height)
{
	RECT rc{ left, top, left + width, top + height };
	return rc;
}

RECT GetRectToCenter(int posX, int posY, int width, int height)
{
	RECT rc{ posX - (width / 2), posY - (height / 2), posX + (width / 2), posY + (height / 2) };
	return rc;
}

//void RenderNemo(HDC hdc, BeingStar* stars, int count)
//{
//	for (int i = 0; i < count; i++)
//	{
//		switch (stars[i].shape)
//		{
//		case Nemo_Shape::Nemo:
//			RenderRectToCenter(hdc, stars[i].ptPos.x, stars[i].ptPos.y,
//				stars[i].size, stars[i].size);
//			break;
//		case Nemo_Shape::Star:
//			RenderStar2(hdc, stars[i].ptPos.x, stars[i].ptPos.y);
//			break;
//		}
//	}
//}
