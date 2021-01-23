#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include "PlayScence.h"

#define BREAKABLE_BBOX_WIDTH  16
#define BREAKABLE_BBOX_HEIGHT 16
#define BREAKABLE_HEALTH 1

class CBreakable : public CGameObject
{
	int bboxWidth = BREAKABLE_BBOX_HEIGHT;
	int bboxHeight = BREAKABLE_BBOX_HEIGHT;
	
public:
	int health;
	CBreakable();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void spawnItem(float x, float y);
};