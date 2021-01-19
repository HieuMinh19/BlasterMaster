#pragma once

#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"

#define TELEPORT_STATE_ARMORL	0
#define TELEPORT_STATE_NORMAL	100


#define TELEPORT_ANI_ARMORL		0
#define TELEPORT_ANI_NORMAL		1
#define TELEPORT_ANI_SS		2

#define TELEPORT_BBOX_WIDTH		24
#define TELEPORT_BBOX_HEIGHT	32

#define TELEPORT_HEALTH			10

#define TIME_ARMORL				1500
#define TIME_RELOAD				1000

class CTeleporter : public CEnemies
{
	int ani;
	float maxX;
	float maxY;
	DWORD lastArmorl;
	DWORD lastTele;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CTeleporter(float mx, float m);
	virtual void SetState(int state);
	void Teleport();
};
