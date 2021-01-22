#pragma once
#include "Brick.h"
#include "GameObject.h"
#include "Trap.h"

#define BULLET_WALKING_SPEED	0.2f
#define BULLET_DOWN_SPEED		0.01f
#define BULLET_MINE_SPEED_FAST	0.05f
#define BULLET_MINE_SPEED_SLOW	0.02f
#define TIME_BUMP				10
#define BULLET_GRAVITY			0.0005f

#define BULLET_BBOX_WIDTH		5
#define BULLET_BBOX_HEIGHT		5

#define BULLET_RIGHT			100
#define BULLET_LEFT				200
#define BULLET_UP				300
#define BULLET_DOWN				400
#define BULLET_STATE_DESTROY	500
#define BULLET_MINE				600
#define BULLET_STATE_BUMP_NOW	700
#define BULLET_STATE_BUMP_AWAIT	800

class CMonsterBullet : public CGameObject
{
	int timeDestroy;
	int animation;
	int animation_bump;
	int typeBullet;
	int countCollision = 0;
	DWORD timeBump;
	DWORD timeLife;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();


public:
	CMonsterBullet(float state, int animation);
	CMonsterBullet(int ani_bullet,int ani_bump, float Xp, float Yp, float Xe, float Ye, float Vb);
	CMonsterBullet(float state, int animation, float VX, float VY);
	CMonsterBullet(int ani_bullet, int ani_bump, float Xp, float Yp, float Xe, float Ye, float Vx, float Vy);
	virtual void SetState(int state);
};