#pragma once
#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"

#define FLOATERS_STATE_HORIZONTAL	100
#define FLOATERS_STATE_VERTICAL		200
#define FLOATERS_STATE_GUN_LEFT		300
#define FLOATERS_STATE_GUN_RIGHT	400


#define FLOATERS_ANI_WALKING_LEFT	0
#define FLOATERS_ANI_WALKING_RIGHT	1
#define FLOATERS_ANI_GUN_LEFT	2
#define FLOATERS_ANI_GUN_RIGHT	3


#define FLOATERS_BBOX_WIDTH		19
#define FLOATERS_BBOX_HEIGHT	15

class CFloaters : public CEnemies
{
	int ani;
	int direction_x;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CFloaters();
	CFloaters(float _vx, float _vy);
	virtual void SetState(int state);

};


