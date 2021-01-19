#pragma once

#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"

#define EYEBALL_STATE_NORMAL	0
#define EYEBALL_STATE_OX	100
#define EYEBALL_STATE_OY	200


#define EYEBALL_ANI_NORMAL		0
#define EYEBALL_ANI_OX			1
#define EYEBALL_ANI_OY		2

#define EYEBALL_BBOX_WIDTH			26
#define EYEBALL_BBOX_HEIGHT			26
#define TIME_RELOAD				500

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
