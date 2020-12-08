#include "MonsterBullet.h"
#include "Utils.h"
#include "PlayScence.h"
CMonsterBullet::CMonsterBullet(float state, int ani) : CGameObject()
{
	animation = ani;
	SetState(state);
	timeDestroy = GetTickCount() + 2500;
	this->x = x;
	this->y = y;
}

CMonsterBullet::CMonsterBullet(float state, int ani, float VX,float VY) : CGameObject()
{
	animation = ani;
	SetState(state);
	timeDestroy = GetTickCount() + 2500;
	this->x = x;
	this->y = y;
	this->vx = VX;
	this->vy = VY;
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
	if (timeDestroy < GetTickCount() && state != OBJECT_STATE_DELETE) {
		SetState(OBJECT_STATE_DELETE);
	}

	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (state == BULLET_MINE) {
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

		if (nx != 0 || ny != 0) SetState(OBJECT_STATE_DELETE);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		vx = BULLET_WALKING_SPEED
		break;
	case BULLET_LEFT:
		vx = -BULLET_WALKING_SPEED
		break;
	case BULLET_UP:
		vy = -BULLET_WALKING_SPEED
			break;
	case BULLET_DOWN:
		vy = BULLET_WALKING_SPEED
			break;
	}
}
