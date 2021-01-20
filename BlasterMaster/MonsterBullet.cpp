#include "MonsterBullet.h"
#include "Utils.h"
#include "PlayScence.h"
#include "math.h"

CMonsterBullet::CMonsterBullet(float state, int ani) : CGameObject()
{
	animation = ani;
	timeDestroy = GetTickCount() + 2500;
	this->x = x;
	this->y = y;
	SetState(state);
}

CMonsterBullet::CMonsterBullet(int ani, float Xp, float Yp, float Xe, float Ye, float Vb) : CGameObject()
{
	animation = ani;
	timeDestroy = GetTickCount() + 250000;
	Xp += 10;
	Yp += 18;
	float T = (Yp - Ye) == 0 ? 0 : (Xp - Xe) / (Yp - Ye); // T = vx / vy
	float _vy = Vb / sqrt((1 + T * T));

	float _vx = abs(T *_vy);

	this->vx = Xp > Xe ? _vx : -_vx;
	this->vy = Yp > Ye ? _vy : -_vy;
}

CMonsterBullet::CMonsterBullet(float state, int ani, float VX, float VY) : CGameObject()
{
	animation = ani;
	timeDestroy = GetTickCount() + 2500;
	this->x = x;
	this->y = y;
	this->vx = VX;
	this->vy = VY;
	SetState(state);
}
void CMonsterBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CMonsterBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (timeDestroy < GetTickCount() && state != OBJECT_STATE_DELETE)
	{
		SetState(OBJECT_STATE_DELETE);
	}

	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//
	if (state == BULLET_MINE)
	{
		vy += BULLET_DOWN_SPEED;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != OBJECT_STATE_DELETE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		vx = 0;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0 || ny != 0)
			SetState(OBJECT_STATE_DELETE);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CMonsterBullet::Render()
{
	animation_set->at(animation)->Render(x, y);
	RenderBoundingBox();
}

void CMonsterBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_RIGHT:
		vx = BULLET_WALKING_SPEED break;
	case BULLET_LEFT:
		vx = -BULLET_WALKING_SPEED break;
	case BULLET_UP:
		vy = -BULLET_WALKING_SPEED break;
	case BULLET_DOWN:
		vy = BULLET_WALKING_SPEED break;
	//case BULLET_DIRECTION:
	//	Setup();
	//	break;
	}
}

//void CMonsterBullet::Setup()
//{
//	CStaticHelpers *helpers = new CStaticHelpers();
//	CPlayer *player = helpers->GetPlayer();
//	float sinn = player->x - this->x;
//	float coss = player->y - this->y;
//	int ox = 1;
//	int oy = 1;  
//	if (sinn < 0)
//	{
//		ox = -1;
//		sinn = fabs(sinn);
//	}
//	if (coss < 0)
//	{
//		oy = -1;
//		coss = fabs(coss);
//	}
//	float v = BULLET_WALKING_SPEED;
//	double result = atan(sinn / coss);
//	this->vx = v * ox * sin(result);
//	this->vy = v * oy * cos(result);
//}