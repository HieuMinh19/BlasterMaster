#include "Bullet.h"
CBullet::CBullet(float playerNX)
{
	nx = playerNX;
	SetState(BULLET_STATE_FLYING);
}

void CBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BULLET_STATE_DIE)
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

	

		if (nx != 0 || ny != 0) SetState(BULLET_STATE_DIE);
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBullet::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_FLYING:
		vx = BULLET_WALKING_SPEED
		if(nx<0)
			vx = -BULLET_WALKING_SPEED;
		break;
	case BULLET_STATE_DIE:
		break;
	}
}
