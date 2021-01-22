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

CMonsterBullet::CMonsterBullet(int ani_bullet, int ani_bump, float Xp, float Yp, float Xe, float Ye, float Vb) : CGameObject()
{
	animation = ani_bullet;
	animation_bump = ani_bump;
	timeDestroy = GetTickCount() + 2500;
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
	if (state == BULLET_STATE_BUMP && GetTickCount() - timeBump >= TIME_BUMP)
	{
		SetState(OBJECT_STATE_DELETE);
	}

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
		if (state != BULLET_MINE)
			vx = 0;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CPlayer*>(e->obj))
			{
				x += min_tx * dx;
				y += min_ty * dy;
			}

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
				if (state != BULLET_MINE) {
					if (nx != 0 || ny != 0)
					{
						SetState(BULLET_STATE_BUMP);
					}
				}
				else {
					x += dx;
					y += dy;
				}
				
			}
		}
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
	case BULLET_STATE_BUMP:
		timeBump = GetTickCount();
		animation = animation_bump;
		break;
	}
}