#include "Worms.h"

CWorms::CWorms()
{
	SetState(WORMS_STATE_WALKING_LEFT);
}

void CWorms::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORMS_BBOX_WIDTH;
	bottom = y + WORMS_BBOX_HEIGHT;
}

void CWorms::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case WORMS_STATE_WALKING_LEFT:
			vx = -WORMS_WALKING_SPEED;
			break;
	}
}

void CWorms::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	vx = x - mario->x > 0 ? -WORMS_WALKING_SPEED : WORMS_WALKING_SPEED;
	CGameObject::Update(dt);
	// Simple fall down
	vy = WORMS_GRAVITY;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CollisionHandleWithBrick(nx);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CWorms::Render()
{
	int ani = vx > 0 ? WORMS_ANI_WALKING_RIGHT : WORMS_ANI_WALKING_LEFT;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CWorms::CollisionHandleWithBrick(float nx)
{
	if (nx == 1)
	{
		vy = -WORMS_WALKING_SPEED;
	}
}
