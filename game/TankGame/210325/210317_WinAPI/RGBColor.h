#pragma once
#include <Windows.h>

class RGBColor
{
	int r;
	int g;
	int b;
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;

public:
	RGBColor();
	~RGBColor();
	inline void SetR(int r) { this->r = r; }
	inline void SetG(int g) { this->g = g; }
	inline void SetB(int b) { this->b = b; }
	inline int GetR() { return r; }
	inline int GetG() { return g; }
	inline int GetB() { return b; }

	inline void ChangeColor(int r, int g, int b) { this->r = r; this->g = g; this->b = b; }
	void ShuffleColor();
	void StartBrush(HDC hdc, int r, int g, int b);
	void StartBrush(HDC hdc);
	void EndBrush(HDC hdc);
	void StartPen(HDC hdc);
	void StartPen(HDC hdc, int r, int g, int b);
	void EndPen(HDC hdc);
};

