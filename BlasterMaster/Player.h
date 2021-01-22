#pragma once
#include "GameObject.h"

#define PLAYER_WALKING_SPEED 0.15f
#define PLAYER_JUMP_SPEED_Y 0.3f
#define PLAYER_JUMP_DEFLECT_SPEED 0.2f
#define PLAYER_GRAVITY 0.002f
#define PLAYER_UNTOUCHABLE_TIME 5000

#define PLAYER_STATE_IDLE 0
#define PLAYER_STATE_WALKING_RIGHT 100
#define PLAYER_STATE_WALKING_LEFT 200
#define PLAYER_STATE_JUMP 300

#define PLAYER_ANI_IDLE_RIGHT 0
#define PLAYER_ANI_IDLE_LEFT 1

#define UNTOUCHABLE_ALPHA 64

class CPlayer : public CGameObject
{
protected:
	
	boolean isSpecialAni;

	DWORD untouchable_start;
	int health;

	float start_x; // initial position of Mario at scene
	float start_y;

public:
	int untouchable;
	int OBJECT_ID;
	boolean isJump, isDie;
	DWORD die_start;
	boolean resetPosition = false;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL) = 0;
	virtual void Render() = 0;
	virtual void SetState(int state) = 0;
	void StartUntouchable()
	{
		untouchable = 1;
		untouchable_start = GetTickCount();
	}
	virtual void KeyRight() = 0;
	virtual void KeyLeft() = 0;
	virtual void KeyUp() = 0;
	virtual void OnKeyUp() = 0;
	virtual void KeyDown() = 0;
	virtual void KeyX() = 0;
	virtual void KeyZ() = 0;
	virtual void KeySHIFT() = 0;
	virtual void OnKeyUpRight() = 0;
	virtual void OnKeyUpLeft() = 0;

	void Reset();
	int getHealth() { return this->health; }
	void setHealth(int health) { this->health = health; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	void Fire(vector<LPGAMEOBJECT> &objects);

};
