#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"

#define INSECT_WALKING_SPEED		0.02f
#define INSECT_JUMP_SPEED		0.05f
#define INSECT_FALLING_COLLISION_UP		0.2f
#define INSECT_GRAVITY			0.008f

#define INSECT_STATE_JUMP_RIGHT	100
#define INSECT_STATE_JUMP_LEFT	200
#define INSECT_STATE_FALLING_RIGHT	300
#define INSECT_STATE_FALLING_LEFT	400
#define INSECT_STATE_JUMP_COLLISION_UP	500

#define INSECT_ANI_WALKING_LEFT		0
#define INSECT_ANI_WALKING_RIGHT	1

#define INSECT_BBOX_WIDTH  18
#define INSECT_BBOX_HEIGHT	17

class CInsect : public CEnemies
{
	int ani;
	DWORD jumpStartAt;
	int direction;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void HandleWithoutCollision();
	void CollisionVertical(float ny);
	void CollisionHorizontal(float nx);

public:
	CInsect();
	CInsect(float _vx);
	virtual void SetState(int state);

};

