#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Trap.h"
#include "Player.h"
#include "Utils.h"
#include "MonsterBullet.h"
#include "StaticHelpers.h"
#include "Jason.h"

#define SKULLS_SPEED_HORIZONTAL		0.1f
#define SKULLS_SPEED_VERTICAL		0.1f
#define SKULLS_SPEED_BULLET_X		0.02f
#define SKULLS_SPEED_BULLET_Y		0.1f

#define SKULLS_STATE_WALKING_LEFT	100
#define SKULLS_STATE_WALKING_RIGHT	200
#define SKULLS_STATE_WALKING_UP		300
#define SKULLS_STATE_GUN			400

#define SKULLS_ANI_LEFT			0
#define SKULLS_ANI_RIGHT		1
#define SKULLS_ANI_GUN_LEFT		2
#define SKULLS_ANI_GUN_RIGHT	3
#define SKULLS_ANI_BULLET_LEFT	4
#define SKULLS_ANI_BULLET_RIGHT	5
#define SKULLS_ANI_BUMP			6


#define SKULLS_BBOX_WIDTH			19
#define SKULLS_BBOX_HEIGHT			19

class CSkulls : public CEnemies
{
	int ani;
	int direction_x;
	DWORD gunAt;
	DWORD gunTime = GetTickCount();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CSkulls();
	virtual void SetState(int state);
	void Fire(float Xp, float Yp, float Xe, float Ye);
};


