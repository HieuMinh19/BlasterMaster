#include "Insect.h"

CInsect::CInsect()
{

}

CInsect::CInsect(float _vx)
{
	this->vx = _vx;
	if (vx < 0.0f)
	{
		SetState(INSECT_STATE_JUMP_LEFT);
	}
	else
	{
		SetState(INSECT_STATE_JUMP_RIGHT);
	}
	jumpStartAt = GetTickCount();
}

void CInsect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + INSECT_BBOX_WIDTH;
	bottom = y + INSECT_BBOX_HEIGHT;
}

void CInsect::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case INSECT_STATE_JUMP_RIGHT:
		vx = INSECT_WALKING_SPEED;
		vy = -INSECT_JUMP_SPEED;
		ani = INSECT_ANI_WALKING_RIGHT;
		break;
	case INSECT_STATE_JUMP_LEFT:
		vx = -INSECT_WALKING_SPEED;
		vy = -INSECT_JUMP_SPEED;
		ani = INSECT_ANI_WALKING_LEFT;
		break;
	case INSECT_STATE_FALLING:
		vy = 0;
		break;
	case INSECT_STATE_JUMP_COLLISION_UP:
		vx = 0;
		vy = INSECT_FALLING_COLLISION_UP;
		break;
	case INSECT_STATE_JUMP_COLLISION_RIGHT:
		vy= 0 ;
		vx = -INSECT_WALKING_SPEED;
		ani = INSECT_ANI_WALKING_LEFT;
		break;
	case INSECT_STATE_JUMP_COLLISION_LEFT:
		vy = 0;
		vx = INSECT_WALKING_SPEED;
		ani = INSECT_ANI_WALKING_RIGHT;
		break;
	}
}

void CInsect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CJason* jason = CJason::GetInstance();
	CGameObject::Update(dt);
	vy += INSECT_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	float min_tx, min_ty, nx = 1, ny;
	float rdx = 0;
	float rdy = 0;

	// TODO: This is a very ugly designed function!!!!
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	// No collision occured, proceed normally
	if (coEventsResult.size() == 0)
	{
		x += dx;
		y += dy;
		HandleWithoutCollision();
	}
	else
	{
		//DebugOut(L"AFTER FILLTER ny: %f \n", ny);
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
				
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CInsect::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CInsect::HandleWithoutCollision()
{
	int newState = NULL;
	DWORD now = GetTickCount();

	if (state == INSECT_STATE_JUMP_RIGHT || state == INSECT_STATE_JUMP_LEFT)
	{
		newState = INSECT_STATE_FALLING;
		jumpStartAt = GetTickCount();
	}
	
	if (now - jumpStartAt >= 1200 && state == INSECT_STATE_FALLING)
	{
		if (vx > 0.0f)
		{
			newState = INSECT_STATE_JUMP_RIGHT;
		}
		else
		{
			newState = INSECT_STATE_JUMP_RIGHT;
		}
	}

	DebugOut(L"sate %d \n", state);

	if (newState != NULL)
	{
		SetState(newState);
	}
}
