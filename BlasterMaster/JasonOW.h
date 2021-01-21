#pragma once
#include "GameObject.h"
#include "Player.h"

#define WALKING_SPEED		0.15f
#define JASON_OW_BBOX_WIDTH  25
#define JASON_OW_BBOX_HEIGHT 32

#define JASON_OW_STATE_IDLE					0
#define JASON_OW_STATE_WALKING_RIGHT		100
#define JASON_OW_STATE_WALKING_LEFT			200
#define JASON_OW_STATE_WALKING_TOP			300
#define JASON_OW_STATE_WALKING_DOWN			400
#define JASON_OW_STATE_DIE					900

#define JASON_OW_ANI_IDLE_RIGHT				0
#define JASON_OW_ANI_IDLE_LEFT				1
#define JASON_OW_ANI_IDLE_TOP				2
#define JASON_OW_ANI_IDLE_DOWN				3
#define JASON_OW_ANI_WALKING_RIGHT			4
#define JASON_OW_ANI_WALKING_LEFT			5
#define JASON_OW_ANI_WALKING_TOP			6
#define JASON_OW_ANI_WALKING_DOWN			7


class CJasonOW : public CPlayer
{
	static CJasonOW* __instance;
	int alpha;
	float start_x;
	float start_y;
	int direction;
	int ani;
	bool brokenBrick;
public:
	CJasonOW(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void KeyDown();
	void KeyUp();
	void KeyLeft();
	void KeyRight();
	void KeyX();
	void KeyZ();
	void KeySHIFT();
	void Reset();
	void GetOut();
	void OnKeyUp();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	//
	void spawnItem(float x, float y);
	static CJasonOW* GetInstance(float x, float y);
	static CJasonOW* GetInstance();
};