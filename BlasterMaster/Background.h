#pragma once
#include "GameObject.h"
#define BACKGROUND_BBOX_WIDTH	512
#define BACKGROUND_BBOX_HEIGHT	120

class CBackground : public CGameObject
{
	float bgWidht, bgHeight;
public:
	CBackground();
	float GetBGWidth() { return this->bgWidht; }
	float GetBGHeight() { return this->bgHeight; }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};