#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Trap.h"
#include "Utils.h"
#include "StaticHelpers.h"
#include "Jason.h"

#define SKULLS_SPEED_HORIZONTAL		1.0f
#define SKULLS_SPEED_VERTICAL		0.05f

#define SKULLS_STATE_HORIZONTAL		100
#define SKULLS_STATE_VERTICAL		200
#define SKULLS_STATE_GUN			300



#define SKULLS_ANI_LEFT			0
#define SKULLS_ANI_RIGHT		1
#define SKULLS_ANI_GUN_LEFT		2
#define SKULLS_ANI_GUN_RIGHT	3


#define SKULLS_BBOX_WIDTH			19
#define SKULLS_BBOX_HEIGHT			19

class CSkulls : public CEnemies
{
	int ani;
	int direction_x;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CSkulls();
	CSkulls(float _vx);
	virtual void SetState(int state);

};


