#pragma once

#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"

#define MINES_STATE_NORMAL	100
#define MINES_STATE_EXPLOSIVE	200

#define MINES_ANI_NORMAL		0
#define BULLET_MINE_ANI		0
#define BULLET_MINE_UP		-0.05f
#define MINES_BBOX_WIDTH			19
#define MINES_BBOX_HEIGHT			19

class CMines : public CEnemies
{
	int ani;
	int direction_x;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CMines();
	virtual void SetState(int state);
	void Explosive();
	void AddBullet(float fastSpeed, CAnimationSets * animation_sets);
};


