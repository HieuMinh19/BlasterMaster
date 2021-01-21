#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Trap.h"
#include "Utils.h"
#include "StaticHelpers.h"
#include "MonsterBullet.h"
#include "Jason.h"

#define FLOATERS_SPEED_BULLET		0.1f
#define FLOATERS_MAX_DISTANCE		90

#define FLOATERS_STATE_HORIZONTAL	100
#define FLOATERS_STATE_VERTICAL		200


#define FLOATERS_ANI_WALKING_LEFT	0
#define FLOATERS_ANI_WALKING_RIGHT	1
#define FLOATERS_ANI_GUN_LEFT		2
#define FLOATERS_ANI_GUN_RIGHT		3
#define FLOATERS_ANI_BULLET_LEFT	4
#define FLOATERS_ANI_BULLET_RIGHT	5


#define FLOATERS_BBOX_WIDTH		19
#define FLOATERS_BBOX_HEIGHT	15
#define FLOATERS_BULLET_BBOX_WIDTH	19
#define FLOATERS_BULLET_BBOX_HEIGHT	15


class CFloaters : public CEnemies
{
	int ani;
	int direction_x;
	int count_gun = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD fireAt = GetTickCount();
public:
	CFloaters();
	CFloaters(float _vx, float _vy);
	virtual void SetState(int state);
	void Fire(float Xp, float Yp, float Xe, float Ye);
};


