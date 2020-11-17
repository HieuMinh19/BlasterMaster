#pragma once
#include "GameObject.h"
#include "Player.h"

#define WALKING_SPEED		0.15f 


class CJasonOW : public CPlayer
{
	static CJasonOW* __instance;
	int untouchable;
	DWORD untouchable_start;
	int alpha;
	int health;
	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CJasonOW(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
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
	static CJasonOW* GetInstance(float x, float y);
	static CJasonOW* GetInstance();
};