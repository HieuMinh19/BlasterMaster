#pragma once
#include "GameObject.h"

#define BULLET_WALKING_SPEED 0.2f;
#define BULLET_DOWN_SPEED 0.01f;
#define BULLET_MINE_SPEED_FAST 0.05f;
#define BULLET_MINE_SPEED_SLOW 0.02f;

#define BULLET_BBOX_WIDTH 5
#define BULLET_BBOX_HEIGHT 5

#define BULLET_RIGHT 0
#define BULLET_LEFT 100
#define BULLET_UP  200
#define BULLET_DOWN  300
#define BULLET_STATE_DESTROY 400
#define BULLET_MINE 500
#define BULLET_CUSTOM 600

#define ANI_JASON 0
#define ANI_DESTROY  1
#define ANI_SOPHIA_LEFT 2
#define ANI_SOPHIA_RIGHT  3
#define ANI_SOPHIA_TOP  4
#define ANI_SOPHIA_UPDATE_LEFT 5
#define ANI_SOPHIA_UPDATE_RIGHT  6
#define ANI_SOPHIA_UPDATE_TOP  7

class CMonsterBullet : public CGameObject
{
	int timeDestroy;
	int animation;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CMonsterBullet(float state, int animation);
	CMonsterBullet(float state, int animation, float VX, float VY);
	virtual void SetState(int state);
};