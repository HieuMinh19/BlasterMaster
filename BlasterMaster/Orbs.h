#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "StaticHelpers.h"
#include "Jason.h"

#define ORBS_SPEED_HORIZONTAL		0.05f
#define ORBS_SPEED_VERTICAL		0.02f

#define ORBS_STATE_WALKING_RIGHT	100
#define ORBS_STATE_WALKING_LEFT		200
#define ORBS_STATE_SHAKE_WALKING_UP	300
#define ORBS_STATE_SHAKE_WALKING_DOWN	400
#define ORBS_STATE_SHAKE_WALKING_RIGHT	500
#define ORBS_STATE_SHAKE_WALKING_LEFT	600
#define ORBS_STATE_COLLISON_LEFT	700
#define ORBS_STATE_COLLISON_RIGHT	800


#define ORBS_ANI_WALKING_LEFT	0
#define ORBS_ANI_WALKING_RIGHT	1
#define ORBS_ANI_SHAKE_COLLISION_LEFT	2
#define ORBS_ANI_SHAKE_COLLISION_RIGHT	3
#define ORBS_ANI_SHAKE_LEFT_TO_RIGHT	4
#define ORBS_ANI_SHAKE_RIGHT_TO_LEFT	5


#define ORBS_BBOX_WIDTH		19
#define ORBS_BBOX_HEIGHT	19

class COrbs : public CEnemies
{
	int ani;
	int species;
	int horizontal_counter = 0;
	float distance_x;
	float distance_y;
	DWORD shake_at;
	DWORD now;
	int direction_x; // 0-> left //1->right
	int direction_y; // 0->up //1->down
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void OrbsNormal(float nx, float ny);
	void OrbsFlyTo(float nx, float ny);
	void HandleWithoutCollision();

public:
	COrbs();
	COrbs(float _vx, float _species);
	virtual void SetState(int state);

};
