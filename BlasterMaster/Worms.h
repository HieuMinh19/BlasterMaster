#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"
#include "PlayScence.h"
#include "StaticHelpers.h"

#define WORMS_WALKING_SPEED		0.03f
#define WORMS_MAX_GRAVITY			0.5f
#define WORMS_GRAVITY			0.003f

#define WORMS_STATE_WALKING_RIGHT	100
#define WORMS_STATE_WALKING_LEFT	200

#define WORMS_ANI_WALKING_RIGHT		0
#define WORMS_ANI_WALKING_LEFT			1

#define WORMS_BBOX_WIDTH  17
#define WORMS_BBOX_HEIGHT 9

class CWorms : public CEnemies
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CollisionHandleWithBrick(float nx);

public: 	
	CWorms();
	virtual void SetState(int state);
};

