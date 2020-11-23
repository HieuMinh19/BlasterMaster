#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"


#define DOMES_WALKING_SPEED		0.05f
#define DOMES_TRACTION			0.04f
#define DOMES_SPEED_FLY_TO		0.3f

#define DOMES_STATE_INITIAL	0

#define DOMES_STATE_LEFT_UP	100
#define DOMES_STATE_LEFT_LEFT 200
#define DOMES_STATE_LEFT_DOWN 300
#define DOMES_STATE_LEFT_RIGHT	400

#define DOMES_STATE_RIGHT_UP	500
#define DOMES_STATE_RIGHT_RIGHT	600
#define DOMES_STATE_RIGHT_DOWN	700
#define DOMES_STATE_RIGHT_LEFT	800

#define DOMES_STATE_UP	1100
#define DOMES_STATE_LEFT 1200
#define DOMES_STATE_DOWN 1300
#define DOMES_STATE_RIGHT	1400

#define DOMES_ANI_LEFT_UP	0
#define DOMES_ANI_LEFT_LEFT	1
#define DOMES_ANI_LEFT_DOWN	2
#define DOMES_ANI_LEFT_RIGHT	3

#define DOMES_ANI_RIGHT_UP	4
#define DOMES_ANI_RIGHT_RIGHT	5
#define DOMES_ANI_RIGHT_DOWN	6
#define DOMES_ANI_RIGHT_LEFT	7

#define DOMES_BBOX_WIDTH  16
#define DOMES_BBOX_HEIGHT 16

class CDomes : public CEnemies
{
	int direction;
	int ani;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void HandleWithoutObstruction();
	void HandleFlyToJason(float jason_x, float jason_y, float nx, float ny);
	void SetStateAfterFirstCollision(float nx, float ny);
	void CollisionHandleWithBrick(float nx, float ny);

public:
	CDomes();
	CDomes(float _vx, float _vy);
	virtual void SetState(int state);
};