#pragma once
#include "GameNode.h"
class MovingNode : public GameNode
{
protected:
	FPOINT pos;
	int width;
	int height;
	int speed;
	bool isAlive;

public:
	virtual void Move() = 0;
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return pos; }
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
};

