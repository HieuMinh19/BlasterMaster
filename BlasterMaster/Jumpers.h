#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "StaticHelpers.h"
#include "Jason.h"


#define JUMPERS_WALKING_SPEED		0.05f
#define JUMPERS_JUMP_SPEED		0.25f
#define JUMPERS_GRAVITY			0.001f

#define JUMPERS_STATE_INITIAL	0
#define JUMPERS_STATE_WALKING_LEFT	100
#define JUMPERS_STATE_WALKING_RIGHT	200
#define JUMPERS_STATE_JUMP_LEFT	300
#define JUMPERS_STATE_JUMP_RIGHT	400
#define JUMPERS_STATE_JUMP_COLLISION_LEFT	500
#define JUMPERS_STATE_JUMP_COLLISION_RIGHT	600

#define JUMPERS_ANI_WALKING_LEFT	0
#define JUMPERS_ANI_WALKING_RIGHT	1
#define JUMPERS_ANI_JUMP_LEFT	2
#define JUMPERS_ANI_JUMP_RIGHT	3

#define JUMPERS_BBOX_WIDTH  14
#define JUMPERS_BBOX_HEIGHT 24

class CJumpers : public CEnemies
{
	int ani;
	int jump_counter = 0;
	DWORD jumpStartAt;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CJumpers();
	CJumpers(float _vx);
	virtual void SetState(int state);
};