// main.cpp
#include "config.h"
#include "GameProc.h"

// ��������
// �ν��Ͻ� : ��ü �޸𸮻� �����ϴ� ��ü
HINSTANCE g_hInstance;		// ���α׷� �ν��Ͻ� �ڵ�
HWND g_hWnd;				// ������ �ڵ�
LPSTR g_lpszClass = (LPSTR)TEXT("�������� ����");
GameProc g_gameProc;

// getRect
RECT GetRect(int left, int top, int width, int height);
RECT GetRectToCenter(int posX, int posY, int width, int height);

// render
void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectToCenter(HDC hdc, int x, int y, int width, int height);
void RenderEllipseToCenter(HDC hdc, int x, int y, int width, int height);

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
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	wndClass.cbClsExtra = 2068;
	wndClass.cbWndExtra = 2068;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(IDI_SHIELD));


	//WNDCLASS wndClass;
	//wndClass.cbClsExtra = 2068;
	//wndClass.cbWndExtra = 2068;
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wndClass.hInstance = g_hInstance;
	//wndClass.lpfnWndProc = WndProc;
	//wndClass.lpszClassName = g_lpszClass;
	//wndClass.lpszMenuName = NULL;
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ���
	RegisterClassEx(&wndClass);
	//RegisterClass(&wndClass);

	// ������ ����
	g_hWnd = CreateWindowEx(/*WS_EX_TOPMOST |*/ WS_EX_TOOLWINDOW, g_lpszClass, g_lpszClass,
		WS_OVERLAPPEDWINDOW,
		50, 50, 1080, 720,
		NULL, NULL, g_hInstance, NULL);

	//g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, 
	//	WS_OVERLAPPEDWINDOW,
	//	50, 50, 1080, 720, 
	//	NULL, NULL, g_hInstance, NULL);

	if (FAILED(g_gameProc.Init())) {//init�� ����������
		MessageBox(g_hWnd, "g_gameProc �ʱ�ȭ ����", "���", MB_OK);
		return 0; //���� ����
	}

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
	return g_gameProc.MainProc(hWnd, iMessage, wParam, lParam);
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
