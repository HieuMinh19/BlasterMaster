#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"
#include "PlayScence.h"
#include "StaticHelpers.h"

#define BOSS_WALKING_SPEED		0.03f
#define BOSS_MAX_GRAVITY			0.5f
#define BOSS_GRAVITY			0.003f

#define BOSS_STATE_WALKING_RIGHT	100
#define BOSS_STATE_WALKING_LEFT	200

#define BOSS_ANI_WALKING_RIGHT		0
#define BOSS_ANI_WALKING_LEFT			1

#define BOSS_BBOX_WIDTH  17
#define BOSS_BBOX_HEIGHT 9

class CBoss : public CEnemies
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CBoss();
	virtual void SetState(int state);
};

