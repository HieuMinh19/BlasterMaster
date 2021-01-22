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

CMonsterBullet::CMonsterBullet(int ani_bullet, int ani_bump, float Xp, float Yp, float Xe, float Ye, float Vb) : CGameObject()
{
	animation = ani_bullet;
	animation_bump = ani_bump;
	typeBullet = 1;
	timeDestroy = GetTickCount() + 2500;
	float T = (Yp - Ye) == 0 ? 0 : (Xp - Xe) / (Yp - Ye); // T = vx / vy
	float _vy = Vb / sqrt((1 + T * T));

	float _vx = abs(T * _vy);

	this->vx = Xp > Xe ? _vx : -_vx;
	this->vy = Yp > Ye ? _vy : -_vy;
}

CMonsterBullet::CMonsterBullet(int ani_bullet, int ani_bump, float Xp, float Yp, float Xe, float Ye, float Vx, float Vy) : CGameObject()
{
<<<<<<< HEAD
	animation = ani;
	timeDestroy = GetTickCount() + 2500;
	this->vx = VX;
	this->vy = VY;
	SetState(state);
=======
	animation = ani_bullet;
	animation_bump = ani_bump;
	typeBullet = 2;
	timeDestroy = GetTickCount() + 5000;
	this->vx = Vx;
	this->vy = -Vy;
>>>>>>> origin/skullssssss
}

void CMonsterBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	switch (typeBullet)
	{
	case 1:
		right = x + BULLET_BBOX_WIDTH;
		bottom = y + BULLET_BBOX_HEIGHT;
	case 2:
		right = x + BULLET_BBOX_WIDTH + 5;
		bottom = y + BULLET_BBOX_HEIGHT + 5;
	default:
		break;
	}
}

void CMonsterBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if ((state == BULLET_STATE_BUMP_NOW || state == BULLET_STATE_BUMP_AWAIT) && GetTickCount() - timeBump >= TIME_BUMP)
	{
		SetState(OBJECT_STATE_DELETE);
	}

	if (timeDestroy < GetTickCount() && state != OBJECT_STATE_DELETE)
	{
		SetState(OBJECT_STATE_DELETE);
	}

	CGameObject::Update(dt, coObjects);

	if (typeBullet == 2 && countCollision <= 1)
	{
		vy += BULLET_GRAVITY * dt;
	}
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

		if (GetTickCount() - timeLife > 2000 && countCollision == 2)
		{
			countCollision = 0;
			SetState(BULLET_STATE_BUMP_AWAIT);
		}
	}
	else
	{
<<<<<<< HEAD
		if (state != BULLET_MINE)
			vx = 0;
=======
		//vx = 0;
>>>>>>> origin/skullssssss

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
				if (e->ny != 0) {
					y -= ny * 0.4f;
				}
				if (e->nx != 0) {
					x -= nx * 0.4f;
				}
				SetState(BULLET_STATE_BUMP_AWAIT);
			}

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
<<<<<<< HEAD
				if (state != BULLET_MINE) {
					if (nx != 0 || ny != 0)
					{
						SetState(BULLET_STATE_BUMP);
					}
				}
				else {
					x += dx;
					y += dy;
=======
				if (nx != 0 || ny != 0)
				{
					countCollision++;
					switch (typeBullet)
					{
					case 1:
						SetState(BULLET_STATE_BUMP_NOW);
						break;
					case 2:
						if (countCollision == 1)
						{
							vy = -0.08f;
							if (nx != 0)
							{
								vy = 0.08f;
							}
						}
						if (countCollision == 2)
						{
							timeLife = GetTickCount();
							vy = 0;
							if (nx != 0)
							{
								vy = 0.08f;
							}
						}
						break;
					default:
						break;
					}
>>>>>>> origin/skullssssss
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
		vx = BULLET_WALKING_SPEED;
		break;
	case BULLET_LEFT:
		vx = -BULLET_WALKING_SPEED;
		break;
	case BULLET_UP:
		vy = -BULLET_WALKING_SPEED;
		break;
	case BULLET_DOWN:
		vy = BULLET_WALKING_SPEED;
		break;
	case BULLET_STATE_BUMP_NOW:
		timeBump = GetTickCount();
		animation = animation_bump;
		break;
	case BULLET_STATE_BUMP_AWAIT:
		timeBump = GetTickCount();
		animation = animation_bump;
		break;
	}
}