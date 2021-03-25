// main.cpp

/*
	Win32 API
	: Application Programming Interface
	운영체제가 응용프로그램 개발을 위해 제공하는 함수의 집합

	핸들(Handle) : 각 윈도우 객체를 구분하는 인덱스 번호(정수)
	메시지(Message) : 윈도우에서 발생하는 이벤트를 운영체제가
		윈도우에 전달하는 데이터 모음
*/

#include <Windows.h>

// 타임어택

// 전역변수
// 인스턴스 : 객체 메모리상에 존재하는 객체
HINSTANCE g_hInstance;		// 프로그램 인스턴스 핸들
HWND g_hWnd;				// 윈도우 핸들
LPSTR g_lpszClass = (LPSTR)TEXT("윈메인의 시작");

// 실습 1. 자동차 그리기
// 실습 2. a, d 키로 자동차 움직이기
/*
	실습 3. 별 그리는 함수를 만들고 
		마우스 클릭 위치에 별이 출력되도록 하자.
	실습 4. 마우스 왼쪽 클릭 + 이동 시 네모 상자를 이동시켜보자. (드래그 앤 드랍)
	실습 5. 별이 되어라!!
	바닥에 네모 10개를 늘어놓고, 마우스 드래그를 통해 위로 이동
	이동된 위치에 별로 출력 되도록 만들어보자.
*/

/*
	1. 큰 상자 두 개, 작은 상자 한 개가 존재
	2. 작은 상자가 들어있는 큰 상자는 W,A,S,D 키로 상하좌우로 이동 가능
	3. 작은 상자는 큰 상자를 벗어날 수 없다.
	4. 큰 상자끼리 부딪히면 작은 상자가 상대 상자에게 넘어간다. (제어권 이동)
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
	Nemo_Shape shape;		// 0 : 네모, 1 : 별
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

	// 윈도우를 생성하기 위한 기본셋팅
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
		핸들		브러쉬	배경
	*/

	// 윈도우 클래스 등록
	RegisterClass(&wndClass);

	// 윈도우 생성
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, 
		WS_OVERLAPPEDWINDOW,
		50, 50, 1080, 720, 
		NULL, NULL, g_hInstance, NULL);

	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);

	// 메시지 큐에 있는 메시지 처리
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
		변수 (메모리의 관점에서 보자)
		1. 지역변수 (함수 내에서 선언)
		- 스택영역에 메모리가 할당
		- 함수가 종료되는 시점에 자동으로 메모리 해제

		2. 전역변수 (함수 외부에서 선언)
		- 데이터영역에 메모리가 할당
		- 프로그램 종료 시에 메모리 해제

		3. static 변수 (함수 내에서 선언)
		- 데이터영역에 메모리가 할당
		- 프로그램 종료 시에 메모리 해제
		- 해당 함수에서만 접근이 가능하다.
	*/

	// 실습. 구구단 2단 출력

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
				// 제어권을 넘기고
				currentBox = (Box)((currentBox + 1) % Box::End);
				// 작은 상자를 이동시킨다
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
				// 제어권을 넘기고
				currentBox = (Box)((currentBox + 1) % Box::End);
				// 작은 상자를 이동시킨다
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
				// 제어권을 넘기고
				currentBox = (Box)((currentBox + 1) % Box::End);
				// 작은 상자를 이동시킨다
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
				// 제어권을 넘기고
				currentBox = (Box)((currentBox + 1) % Box::End);
				// 작은 상자를 이동시킨다
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
		// 인사
		TextOut(hdc, 20, 20, "Hello, WinMain!!", strlen("Hello, WinMain!!"));
		// 마우스 좌표
		wsprintf(szText, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
		TextOut(hdc, 200, 20, szText, strlen(szText));
		// 선택된 상자 아이디
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

// 네모를 그리는 함수
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
	// 자동차 상부
	RenderRectToCenter(hdc, posX, posY - 100, 300, 200);

	// 하부
	RenderRectToCenter(hdc, posX, posY + 150, 500, 300);

	// 바퀴
	// 왼쪽 posX - 100, posY + 350, 오른쪽 posX + 100, posY + 350
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
