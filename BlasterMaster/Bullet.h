#pragma once
#include "GameObject.h"

#define BULLET_WALKING_SPEED 0.2f;

#define BULLET_BBOX_WIDTH 5
#define BULLET_BBOX_HEIGHT 5

#define BULLET_STATE_FLYING 100
#define BULLET_STATE_DESTROY 200

#define ANI_FLYING 0
#define ANI_DESTROY  1

#define TIME_ANI_DESTROY 150

class CBullet : public CGameObject
{
	int timeDestroy;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CBullet(float nx);
	virtual void SetState(int state);
};