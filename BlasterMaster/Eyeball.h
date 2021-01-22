#pragma once

#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"

#define EYEBALL_STATE_MOVE	0
#define EYEBALL_STATE_STAND	100

#define EYEBALL_ANI_NORMAL		0
#define EYEBALL_ANI_BULLET	0
#define EYEBALL_ANI_BUMP	2

#define EYEBALL_BULLET_SPEED	0.1f


#define EYEBALL_BBOX_WIDTH			16
#define EYEBALL_BBOX_HEIGHT			16
#define TIME_RELOAD				2500
#define TIME_STAND_TO_FIRE				500

class CEyeball : public CEnemies
{
	DWORD lastFire;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CEyeball();
	virtual void SetState(int state);
	void Fire(float Xp, float Yp, float Xe, float Ye);
};
