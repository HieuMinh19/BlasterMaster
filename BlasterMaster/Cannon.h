#pragma once

#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"

#define CANNON_STATE_NORMAL	0
#define CANNON_STATE_OX	100
#define CANNON_STATE_OY	200


#define CANNON_ANI_NORMAL		0
#define CANNON_ANI_OX			1
#define CANNON_ANI_OY		2

#define CANNON_BBOX_WIDTH			26
#define CANNON_BBOX_HEIGHT			26
#define TIME_RELOAD				500
#define TIME_ANI_FIRE			250

class CCanon : public CEnemies
{
	int ani;
	DWORD lastFire;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CCanon();
	virtual void SetState(int state);
	void Fire();
	void AddBullet(int state, CAnimationSets * animation_sets, float x, float y);
};
