#include "RGBColor.h"
#include <ctime>

RGBColor::RGBColor()
{
	srand(time(NULL));
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
}

RGBColor::~RGBColor()
{

}

void RGBColor::ShuffleColor()
{
	srand(time(NULL));
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
}

void RGBColor::StartBrush(HDC hdc, int r, int g, int b)
{
	hBrush = CreateSolidBrush(RGB(r, g, b));
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
}

void RGBColor::StartBrush(HDC hdc)
{
	hBrush = CreateSolidBrush(RGB(r, g, b));
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
}

void RGBColor::EndBrush(HDC hdc)
{
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}

void RGBColor::StartPen(HDC hdc)
{
	hPen = CreatePen(PS_SOLID, 3, RGB(r, g, b));
	hOldPen = (HPEN)SelectObject(hdc, hPen);
}

void RGBColor::EndPen(HDC hdc)
{
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}
