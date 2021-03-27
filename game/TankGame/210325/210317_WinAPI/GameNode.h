#pragma once
#include "config.h"

class RGBColor; // has a 관계
class GameNode
{
protected:
	RGBColor* color;//색상

public:
	GameNode() { color = nullptr; }
	virtual ~GameNode() {};//가상 소멸자 : 조상 클래스의 소멸자는 가상 소멸자로 작성한다.
	virtual HRESULT Init()=0;
	virtual void Render(HDC hdc)=0;
	virtual void Update()=0;
	virtual void Release()=0;
};

