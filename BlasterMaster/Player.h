#pragma once
#include "GameObject.h"
/*
PLAYER_WALKING_SPEED		0.15f
PLAYER_JUMP_SPEED_Y		0.3f
PLAYER_JUMP_DEFLECT_SPEED		0.2f
PLAYER_GRAVITY		0.002f
PLAYER_STATE_IDLE		0
PLAYER_STATE_WALKING_RIGHT		100
PLAYER_STATE_WALKING_LEFT		200
PLAYER_STATE_JUMP		300
PLAYER_UNTOUCHABLE_TIME		5000
*/
#define PLAYER_WALKING_SPEED		0.15f
#define PLAYER_JUMP_SPEED_Y			0.3f
#define PLAYER_JUMP_DEFLECT_SPEED	0.2f
#define PLAYER_GRAVITY				0.002f
#define PLAYER_UNTOUCHABLE_TIME		5000

#define PLAYER_STATE_IDLE			0
#define PLAYER_STATE_WALKING_RIGHT	100
#define PLAYER_STATE_WALKING_LEFT	200
#define PLAYER_STATE_JUMP			300

#define PLAYER_ANI_IDLE_RIGHT		0
#define PLAYER_ANI_IDLE_LEFT		1

class CPlayer: public CGameObject
{
protected:
	int untouchable;
	boolean isSpecialAni;
	DWORD untouchable_start;
	int health;

	float start_x;			// initial position of Mario at scene
	float start_y;

public:
	CPlayer(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) = 0;
	virtual void Render() = 0;

	virtual void SetState(int state) = 0;
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual void MoveRight() = 0;
	virtual void MoveLeft() = 0;

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	void Fire(vector<LPGAMEOBJECT>& objects);

};

