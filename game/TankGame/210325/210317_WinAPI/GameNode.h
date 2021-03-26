#pragma once
#include "config.h"

class GameNode
{
protected:
	FPOINT pos;
	int width;
	int height;

public:
	GameNode();
	virtual ~GameNode();//가상 소멸자 : 조상 클래스의 소멸자는 가상 소멸자로 작성한다.
	virtual HRESULT Init()=0;
	virtual void Render(HDC hdc)=0;
	virtual void Update()=0;
	virtual void Release()=0;

	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return pos; }
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
};

