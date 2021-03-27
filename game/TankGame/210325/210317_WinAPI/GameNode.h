#pragma once
#include "config.h"

class RGBColor; // has a ����
class GameNode
{
protected:
	RGBColor* color;//����

public:
	GameNode() { color = nullptr; }
	virtual ~GameNode() {};//���� �Ҹ��� : ���� Ŭ������ �Ҹ��ڴ� ���� �Ҹ��ڷ� �ۼ��Ѵ�.
	virtual HRESULT Init()=0;
	virtual void Render(HDC hdc)=0;
	virtual void Update()=0;
	virtual void Release()=0;
};

