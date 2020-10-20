#pragma once
#include "GameObject.h"
#include "Player.h"

#define SOPHIA_WALKING_SPEED		0.15f 
//0.1f
#define SOPHIA_JUMP_SPEED_Y		0.25f
#define SOPHIA_JUMP_DEFLECT_SPEED 0.2f
#define SOPHIA_GRAVITY			0.002f
#define SOPHIA_DIE_DEFLECT_SPEED	 0.5f
#define SOPHIA_JUMP_WHILE_WALK_SPEED_Y		0.25f
#define SOPHIA_JUMP_WHILE_WALK_SPEED_X		0.05f
#define JUMP_GRAVITY			0.0005f

#define SOPHIA_STATE_IDLE			0
#define SOPHIA_STATE_WALKING_RIGHT	100
#define SOPHIA_STATE_WALKING_LEFT	200
#define SOPHIA_STATE_JUMP			300
#define	SOPHIA_WALK_JUMP_TIME		500

#define SOPHIA_STATE_MOVE_UP_RIGHT	400
#define SOPHIA_STATE_STAND_UP_RIGHT	500
#define SOPHIA_STATE_WALK_UP_RIGHT	600
#define SOPHIA_STATE_JUMP_UP_RIGHT	700
#define SOPHIA_STATE_MOVE_UP_LEFT	800
#define SOPHIA_STATE_STAND_UP_LEFT	900
#define SOPHIA_STATE_JUMP_UP_LEFT	1000
#define SOPHIA_STATE_WALK_UP_LEFT	1100
#define SOPHIA_STATE_AFTER_JUMP_RIGHT	1200
#define SOPHIA_STATE_AFTER_JUMP_LEFT	1300
#define SOPHIA_STATE_UNTOUCHABLE	1400
#define SOPHIA_STATE_DIE				9900

#define SOPHIA_ANI_IDLE_RIGHT			0
#define SOPHIA_ANI_IDLE_LEFT			1
#define SOPHIA_ANI_WALKING_RIGHT		2
#define SOPHIA_ANI_WALKING_LEFT			3

#define SOPHIA_ANI_JUMP_RIGHT			4
#define SOPHIA_ANI_JUMP_LEFT			5
#define SOPHIA_ANI_MOVE_UP_RIGHT		6
#define SOPHIA_ANI_STAND_UP_RIGHT		7
#define SOPHIA_ANI_WALK_UP_RIGHT		8
#define SOPHIA_ANI_JUMP_UP_RIGHT		9
#define SOPHIA_ANI_MOVE_UP_LEFT			10
#define SOPHIA_ANI_STAND_UP_LEFT		11
#define SOPHIA_ANI_JUMP_UP_LEFT			12
#define SOPHIA_ANI_WALK_UP_LEFT			13
#define SOPHIA_ANI_AFTER_JUMP_RIGHT		14
#define SOPHIA_ANI_AFTER_JUMP_LEFT		15
#define SOPHIA_ANI_UNTOUCHABLE_RIGHT	16
#define SOPHIA_ANI_UNTOUCHABLE_LEFT		17
#define SOPHIA_ANI_DIE					99


#define	SOPHIA_LEVEL_NORMAL	1
#define	SOPHIA_LEVEL_MOVE_UP	2

#define SOPHIA_BIG_BBOX_WIDTH  13
#define SOPHIA_BIG_BBOX_HEIGHT 20

// #define SOPHIA_SMALL_BBOX_WIDTH  13
// #define SOPHIA_SMALL_BBOX_HEIGHT 15

#define SOPHIA_MOVE_UP_BBOX_WIDTH  13
#define SOPHIA_MOVE_UP_BBOX_HEIGHT 50

#define SOPHIA_UNTOUCHABLE_TIME 5000
#define SOPHIA_JUMP_TIME 1000
#define SOPHIA_MOVEUP_START 450

class CSophia :  public CPlayer
{
	static CSophia* __instance;
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of SOPHIA at scene
	float start_y;
public:
	int hover;
	boolean isJumping, isMoveUp, isStandUp, isJumpingWhileWalk, isWalkAfterJump;
	DWORD jump_start, moveup_start;
	CSophia(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetNx(int a);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void fire(vector<LPGAMEOBJECT>& objects);
	void Reset();
	void Jump();
	void Walk();
	void WalkUp();
	void MoveUpKeyDown();
	void MoveUpKeyUp();
	void SetWalk();
	void MoveRight();
	void MoveLeft();
	void KeyDown();
	void KeyUp();
	void KeyLeft();
	void KeyRight();
	void KeyX();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	static CSophia* GetInstance(float x, float y);
	static CSophia* GetInstance();
};