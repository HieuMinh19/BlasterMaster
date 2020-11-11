#pragma once
#include "GameObject.h"
#define BACKGROUND_BBOX_WIDTH	512
#define BACKGROUND_BBOX_HEIGHT	120

class CBackground : public CGameObject
{
public:
	CBackground();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};