#pragma once
#include "GameObject.h"
#include "Player.h"

#define WALKING_SPEED		0.15f 
#define CRAWL_SPEED			0.05f 

//0.1f
#define JUMP_SPEED_Y		0.3f
#define JUMP_DEFLECT_SPEED 0.2f
#define JUMP_CHANGE_PLAYER_SPEED 0.1f
#define GRAVITY			0.002f
#define DIE_DEFLECT_SPEED	 0.5f

#define STATE_IDLE					0
#define STATE_WALKING_RIGHT			100
#define STATE_WALKING_LEFT			200
#define STATE_JUMP					300
#define STATE_DIE					400
#define STATE_CRAWL_IDLE			500
#define STATE_CRAWL_WALKING_RIGHT	600
#define STATE_CRAWL_WALKING_LEFT	700

#define ANI_IDLE_RIGHT				0
#define ANI_IDLE_LEFT				1
#define ANI_CRAWL_IDLE_RIGHT		2
#define ANI_CRAWL_IDLE_LEFT			3

#define ANI_WALKING_RIGHT			4
#define ANI_WALKING_LEFT			5
#define ANI_CRAWL_WALKING_RIGHT		6
#define ANI_CRAWL_WALKING_LEFT		7
#define ANI_JUMP_RIGHT				8
#define ANI_JUMP_LEFT				9

#define ANI_DIE				10

#define BBOX_WIDTH  8
#define BBOX_HEIGHT 16

#define CRAWL_BBOX_WIDTH  16
#define CRAWL_BBOX_HEIGHT 8

#define UNTOUCHABLE_TIME 10000
#define JASON_MAX_HEALTH 8


class CJason : public CPlayer
{
	static CJason* __instance;
	int untouchable;
	DWORD untouchable_start;
	int alpha;
	int health;
	boolean inTank;
	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CJason(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void MoveRight();
	void MoveLeft();
	void KeyDown();
	void KeyUp();
	void KeyLeft();
	void KeyRight();
	void KeyX();
	void KeyZ();
	void KeySHIFT();

	void Reset();
	void GetOut();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void fire(vector<LPGAMEOBJECT> &objects);

	//
	void spawnItem(float x, float y);
	static CJason* GetInstance(float x, float y);
	static CJason* GetInstance();
};