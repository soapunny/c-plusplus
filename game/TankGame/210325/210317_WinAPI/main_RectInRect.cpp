// main.cpp

/*
	Win32 API
	: Application Programming Interface
	�ü���� �������α׷� ������ ���� �����ϴ� �Լ��� ����

	�ڵ�(Handle) : �� ������ ��ü�� �����ϴ� �ε��� ��ȣ(����)
	�޽���(Message) : �����쿡�� �߻��ϴ� �̺�Ʈ�� �ü����
		�����쿡 �����ϴ� ������ ����
*/

#include <Windows.h>

// Ÿ�Ӿ���

// ��������
// �ν��Ͻ� : ��ü �޸𸮻� �����ϴ� ��ü
HINSTANCE g_hInstance;		// ���α׷� �ν��Ͻ� �ڵ�
HWND g_hWnd;				// ������ �ڵ�
LPSTR g_lpszClass = (LPSTR)TEXT("�������� ����");

// �ǽ� 1. �ڵ��� �׸���
// �ǽ� 2. a, d Ű�� �ڵ��� �����̱�
/*
	�ǽ� 3. �� �׸��� �Լ��� ����� 
		���콺 Ŭ�� ��ġ�� ���� ��µǵ��� ����.
	�ǽ� 4. ���콺 ���� Ŭ�� + �̵� �� �׸� ���ڸ� �̵����Ѻ���. (�巡�� �� ���)
	�ǽ� 5. ���� �Ǿ��!!
	�ٴڿ� �׸� 10���� �þ����, ���콺 �巡�׸� ���� ���� �̵�
	�̵��� ��ġ�� ���� ��� �ǵ��� ������.
*/

/*
	1. ū ���� �� ��, ���� ���� �� ���� ����
	2. ���� ���ڰ� ����ִ� ū ���ڴ� W,A,S,D Ű�� �����¿�� �̵� ����
	3. ���� ���ڴ� ū ���ڸ� ��� �� ����.
	4. ū ���ڳ��� �ε����� ���� ���ڰ� ��� ���ڿ��� �Ѿ��. (����� �̵�)
*/

enum Box { First, Second, End };

RECT rc[Box::End], rcIn;
Box currentBox = Box::First;
int boxMoveSpeed = 8;

enum Nemo_Shape { Nemo, Star };

struct BeingStar
{
	POINT ptPos;
	int size;
	Nemo_Shape shape;		// 0 : �׸�, 1 : ��
};

// set
void SetBox();
void SetNemo(BeingStar* stars, int count);
// getRect
RECT GetRect(int left, int top, int width, int height);
RECT GetRectToCenter(int posX, int posY, int width, int height);
// render
void RenderBox(HDC hdc);
void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectToCenter(HDC hdc, int x, int y, int width, int height);
void RenderEllipseToCenter(HDC hdc, int x, int y, int width, int height);
void RenderCar(HDC hdc, int posX, int posY);
void RenderStar(HDC hdc, int posX, int posY);
void RenderStar2(HDC hdc, int posX, int posY);
void RenderNemo(HDC hdc, BeingStar* stars, int count);
// collisionCheck
bool PointInRect(POINT ptMouse, RECT rc);
bool PointInRect(POINT ptMouse, POINT rcPos, int rcSize);
bool RectInRect(RECT rc1, RECT rc2);

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

	/*
		h		br		Background
		�ڵ�		�귯��	���
	*/

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

	/*
		���� (�޸��� �������� ����)
		1. �������� (�Լ� ������ ����)
		- ���ÿ����� �޸𸮰� �Ҵ�
		- �Լ��� ����Ǵ� ������ �ڵ����� �޸� ����

		2. �������� (�Լ� �ܺο��� ����)
		- �����Ϳ����� �޸𸮰� �Ҵ�
		- ���α׷� ���� �ÿ� �޸� ����

		3. static ���� (�Լ� ������ ����)
		- �����Ϳ����� �޸𸮰� �Ҵ�
		- ���α׷� ���� �ÿ� �޸� ����
		- �ش� �Լ������� ������ �����ϴ�.
	*/

	// �ǽ�. ������ 2�� ���

	static POINT ptMouse{0, 0};
	static bool isPtInRect = false;
	static int selectedId = -1;
	static BeingStar stars[10];

	char szText[128] = "";

	int testValue = 0;
	int testValue2 = 10;

	switch (iMessage)
	{
	case WM_CREATE:
		SetBox();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'a': case 'A':
			rc[currentBox].left -= boxMoveSpeed;
			rc[currentBox].right -= boxMoveSpeed;

			if (rc[currentBox].right < rcIn.right)
			{
				rcIn.right = rc[currentBox].right;
				rcIn.left = rcIn.right - 80;
			}

			if (RectInRect(rc[Box::First], rc[Box::Second]))
			{
				// ������� �ѱ��
				currentBox = (Box)((currentBox + 1) % Box::End);
				// ���� ���ڸ� �̵���Ų��
				int currentBoxPosX = (rc[currentBox].left + rc[currentBox].right) / 2;
				int currentBoxPosY = (rc[currentBox].top + rc[currentBox].bottom) / 2;
				rcIn = GetRectToCenter(currentBoxPosX, currentBoxPosY, 80, 80);
			}

			InvalidateRect(g_hWnd, NULL, true);
			break;
		case 'd': case 'D':
			rc[currentBox].left += boxMoveSpeed;
			rc[currentBox].right += boxMoveSpeed;

			if (rc[currentBox].left > rcIn.left)
			{
				rcIn.left = rc[currentBox].left;
				rcIn.right = rcIn.left + 80;
			}

			if (RectInRect(rc[Box::First], rc[Box::Second]))
			{
				// ������� �ѱ��
				currentBox = (Box)((currentBox + 1) % Box::End);
				// ���� ���ڸ� �̵���Ų��
				int currentBoxPosX = (rc[currentBox].left + rc[currentBox].right) / 2;
				int currentBoxPosY = (rc[currentBox].top + rc[currentBox].bottom) / 2;
				rcIn = GetRectToCenter(currentBoxPosX, currentBoxPosY, 80, 80);
			}

			InvalidateRect(g_hWnd, NULL, true);
			break;
		case 'w': case 'W':
			rc[currentBox].top -= boxMoveSpeed;
			rc[currentBox].bottom -= boxMoveSpeed;

			if (rc[currentBox].bottom < rcIn.bottom)
			{
				rcIn.bottom = rc[currentBox].bottom;
				rcIn.top = rcIn.bottom - 80;
			}

			if (RectInRect(rc[Box::First], rc[Box::Second]))
			{
				// ������� �ѱ��
				currentBox = (Box)((currentBox + 1) % Box::End);
				// ���� ���ڸ� �̵���Ų��
				int currentBoxPosX = (rc[currentBox].left + rc[currentBox].right) / 2;
				int currentBoxPosY = (rc[currentBox].top + rc[currentBox].bottom) / 2;
				rcIn = GetRectToCenter(currentBoxPosX, currentBoxPosY, 80, 80);
			}

			InvalidateRect(g_hWnd, NULL, true);
			break;
		case 's': case 'S':
			rc[currentBox].top += boxMoveSpeed;
			rc[currentBox].bottom += boxMoveSpeed;

			if (rc[currentBox].top > rcIn.top)
			{
				rcIn.top = rc[currentBox].top;
				rcIn.bottom = rcIn.top + 80;
			}

			if (RectInRect(rc[Box::First], rc[Box::Second]))
			{
				// ������� �ѱ��
				currentBox = (Box)((currentBox + 1) % Box::End);
				// ���� ���ڸ� �̵���Ų��
				int currentBoxPosX = (rc[currentBox].left + rc[currentBox].right) / 2;
				int currentBoxPosY = (rc[currentBox].top + rc[currentBox].bottom) / 2;
				rcIn = GetRectToCenter(currentBoxPosX, currentBoxPosY, 80, 80);
			}

			InvalidateRect(g_hWnd, NULL, true);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);

		for (int i = 0; i < 10; i++)
		{
			isPtInRect = PointInRect(ptMouse, stars[i].ptPos, stars[i].size);
			if (isPtInRect)
			{
				selectedId = i;
				break;
			}
			selectedId = -1;
		}
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_LBUTTONUP:
		if (selectedId != -1)
		{
			stars[selectedId].shape = Nemo_Shape::Star;
			selectedId = -1;
		}
		isPtInRect = false;
		break;
	case WM_RBUTTONDOWN:
		InvalidateRect(g_hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		if (selectedId != -1)
		{
			ptMouse.x = LOWORD(lParam);
			ptMouse.y = HIWORD(lParam);

			stars[selectedId].ptPos = ptMouse;
		}

		InvalidateRect(g_hWnd, NULL, true);
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

		RenderBox(hdc);

		EndPaint(g_hWnd, &ps);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void SetBox()
{
	rc[Box::First] = GetRectToCenter(200, 200, 200, 200);
	rc[Box::Second] = GetRectToCenter(500, 200, 200, 200);
	rcIn = GetRectToCenter(200, 200, 80, 80);
}

void RenderBox(HDC hdc)
{
	for (int i = 0; i < Box::End; i++)
	{
		Rectangle(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
	}

	Rectangle(hdc, rcIn.left, rcIn.top, rcIn.right, rcIn.bottom);
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

	//if (rc.left < ptMouse.x && ptMouse.x < rc.right
	//	&& rc.top < ptMouse.y && ptMouse.y < rc.bottom)
	//{
	//	return true;
	//}

	//return false;
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

void SetNemo(BeingStar* stars, int count)
{
	for (int i = 0; i < count; i++)
	{
		stars[i].ptPos.x = 50 + i * 120;
		stars[i].ptPos.y = 600;
		stars[i].size = 100;
		stars[i].shape = Nemo_Shape::Nemo;
	}
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

void RenderNemo(HDC hdc, BeingStar* stars, int count)
{
	for (int i = 0; i < count; i++)
	{
		switch (stars[i].shape)
		{
		case Nemo_Shape::Nemo:
			RenderRectToCenter(hdc, stars[i].ptPos.x, stars[i].ptPos.y,
				stars[i].size, stars[i].size);
			break;
		case Nemo_Shape::Star:
			RenderStar2(hdc, stars[i].ptPos.x, stars[i].ptPos.y);
			break;
		}
	}
}
