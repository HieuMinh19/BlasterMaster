
#pragma once
#include "GameObject.h"
#include "Boss.h"
#define BULLET_WALKING_SPEED 0.2f;

#define BULLET_BBOX_WIDTH 5
#define BULLET_BBOX_HEIGHT 5

#define BULLET_STATE_FLYING 100
#define BULLET_STATE_DESTROY 200
#define BULLET_STATE_RIGHT 300
#define BULLET_STATE_LEFT 400
#define BULLET_STATE_UP 500
#define BULLET_STATE_DOWN 600


#define ANI_JASON 0
#define ANI_DESTROY  1
#define ANI_SOPHIA_LEFT 2
#define ANI_SOPHIA_RIGHT  3
#define ANI_SOPHIA_TOP  4
#define ANI_SOPHIA_UPDATE_LEFT 5
#define ANI_SOPHIA_UPDATE_RIGHT  6
#define ANI_SOPHIA_UPDATE_TOP  7

#define TIME_ANI_DESTROY 150
#define TIME_LIVE 700

class CBullet : public CGameObject
{
	int timeDestroy;
	int animation;
	bool brokenBrick;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CBullet(float nx, int animation);
	CBullet(int state, int animation, bool brokenBrick);
	virtual void SetState(int state);
	void spawnItem(float x, float y);
};
