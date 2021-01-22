#pragma once
#include "GameObject.h"
#include "Player.h"

#define JASON_WALKING_SPEED		0.05f 
#define JASON_CRAWL_SPEED			0.05f 


#define JASON_JUMP_SPEED_Y		0.1f
#define JASON_JUMP_DEFLECT_SPEED 0.2f
#define JASON_STAND_UP 0.17f
#define JASON_JUMP_CHANGE_PLAYER_SPEED 0.1f
#define JASON_GRAVITY			0.0002f
#define JASON_DIE_DEFLECT_SPEED	 0.5f
#define JASON_GET_OUT_TANK_SPEED			0.0002F


#define JASON_STATE_IDLE					0
#define JASON_STATE_WALKING_RIGHT			100
#define JASON_STATE_WALKING_LEFT			200
#define JASON_STATE_JUMP					300
#define JASON_STATE_DIE					400
#define JASON_STATE_CRAWL_IDLE			500
#define JASON_STATE_CRAWL_WALKING_RIGHT	600
#define JASON_STATE_CRAWL_WALKING_LEFT	700
#define JASON_STATE_CRAWL_IN_TANK	800
#define JASON_STATE_CRAWL_STAND_UP	900


#define JASON_ANI_IDLE_RIGHT				0
#define JASON_ANI_IDLE_LEFT				1
#define JASON_ANI_CRAWL_IDLE_RIGHT		2
#define JASON_ANI_CRAWL_IDLE_LEFT			3

#define JASON_ANI_WALKING_RIGHT			4
#define JASON_ANI_WALKING_LEFT			5
#define JASON_ANI_CRAWL_WALKING_RIGHT		6
#define JASON_ANI_CRAWL_WALKING_LEFT		7
#define JASON_ANI_JUMP_RIGHT				8
#define JASON_ANI_JUMP_LEFT				9

#define JASON_ANI_DIE				10

#define JASON_BBOX_WIDTH  8
#define JASON_BBOX_HEIGHT 16
#define JASON_BULLET_BBOX	 6

#define JASON_CRAWL_BBOX_WIDTH  16
#define JASON_CRAWL_BBOX_HEIGHT 8

#define JASON_UNTOUCHABLE_TIME 1000
#define JASON_DIE_TIME	1000
#define JASON_MAX_HEALTH 8


class CJason : public CPlayer
{
	static CJason* __instance;
	int alpha;
	
	float start_x;			// initial position of Mario at scene
	float start_y;

public:
	boolean inTank, isCrawl;
	CJason(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void KeyDown();
	void OnKeyUp();
	void KeyUp();
	void KeyLeft();
	void KeyRight();
	void KeyX();
	void KeyZ();
	void KeySHIFT();
	void Reset();
	void GetOut();
	void OnKeyUpRight();
	void OnKeyUpLeft();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	//
	void spawnItem(float x, float y);
	static CJason* GetInstance(float x, float y);
	static CJason* GetInstance();

	~CJason() {
		__instance = NULL;
	}
};