#pragma once
#include "GameObject.h"

#define WALKING_SPEED		0.15f 
//0.1f
#define JUMP_SPEED_Y		0.3f
#define JUMP_DEFLECT_SPEED 0.2f
#define GRAVITY			0.002f
#define DIE_DEFLECT_SPEED	 0.5f

#define STATE_IDLE			0
#define STATE_WALKING_RIGHT	100
#define STATE_WALKING_LEFT	200
#define STATE_JUMP			300
#define STATE_DIE			400

#define ANI_IDLE_RIGHT		0
#define ANI_IDLE_LEFT			1
#define ANI_SMALL_IDLE_RIGHT		2
#define ANI_SMALL_IDLE_LEFT			3

#define ANI_WALKING_RIGHT			4
#define ANI_WALKING_LEFT			5
#define ANI_SMALL_WALKING_RIGHT		6
#define ANI_SMALL_WALKING_LEFT		7

#define ANI_DIE				8

#define	LEVEL_SMALL	1
#define	LEVEL		2

#define BIG_BBOX_WIDTH  8
#define BIG_BBOX_HEIGHT 16

#define SMALL_BBOX_WIDTH  13
#define SMALL_BBOX_HEIGHT 15

#define UNTOUCHABLE_TIME 1000


class CJason : public CGameObject
{
	int untouchable;
	boolean isDrwal;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CJason(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};