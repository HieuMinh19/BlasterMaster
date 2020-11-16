#pragma once
#include "GameObject.h"

#define BREAKABLE_BBOX_WIDTH  16
#define BREAKABLE_BBOX_HEIGHT 16


class CBreakable : public CGameObject
{
	int bboxWidth = BREAKABLE_BBOX_HEIGHT;
	int bboxHeight = BREAKABLE_BBOX_HEIGHT;
	
public:
	int health = 2;
	CBreakable();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};