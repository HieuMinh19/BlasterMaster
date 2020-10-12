#pragma once
#include "GameObject.h"


#define BG_BBOX_WIDTH  250
#define BG_BBOX_HEIGHT 220

class CIntro : public CGameObject
{
public:
	CIntro();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};