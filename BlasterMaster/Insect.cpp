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

		break;
	case INSECT_STATE_FALLING_LEFT:
		vx = -INSECT_WALKING_SPEED;
		vy = INSECT_GRAVITY;
		ani = INSECT_ANI_WALKING_LEFT;
		break;
	case INSECT_STATE_FALLING_RIGHT:
		vx = INSECT_WALKING_SPEED;
		vy = INSECT_GRAVITY;
		ani = INSECT_ANI_WALKING_RIGHT;
		break;
	case INSECT_STATE_JUMP_COLLISION_UP:
		//direction // 0->right //1->left
		if (vx > 0.0f)
		{
			direction = 0;
		}
		else
		{
			direction = 1;
		}
		vy = INSECT_FALLING_COLLISION_UP;
		break;
	}
}

void CInsect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	CGameObject::Update(dt);
	//vy += INSECT_GRAVITY * dt;

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

			if (dynamic_cast<CPlayer*>(e->obj))
			{
				HandleCollisionPlayer(e, nx, ny);
			}

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
				if (ny != 0.0f)
				{
					CollisionVertical(ny);
				}
				if (nx != 0.0f)
				{
					CollisionHorizontal(nx);
				}
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

	if (now - jumpStartAt >= 400)
	{
		if (state == INSECT_STATE_JUMP_RIGHT)
		{
			newState = INSECT_STATE_FALLING_RIGHT;
			jumpStartAt = GetTickCount();
		}
		if (state == INSECT_STATE_JUMP_LEFT)
		{
			newState = INSECT_STATE_FALLING_LEFT;
			jumpStartAt = GetTickCount();
		}
	}


	if (now - jumpStartAt >= 1200)
	{
		if (state == INSECT_STATE_FALLING_LEFT)
		{
			newState = INSECT_STATE_JUMP_LEFT;
			jumpStartAt = GetTickCount();
		}
		if (state == INSECT_STATE_FALLING_RIGHT)
		{
			newState = INSECT_STATE_JUMP_RIGHT;
			jumpStartAt = GetTickCount();
		}
	}

	if (newState != NULL)
	{
		SetState(newState);
	}
}

void CInsect::CollisionVertical(float ny)
{
	DebugOut(L"state: %d \n", state);
	if (ny > 0.0f)
	{
		SetState(INSECT_STATE_JUMP_COLLISION_UP);
	}
	else
	{
		if (direction == 0)
		{
			SetState(INSECT_STATE_JUMP_RIGHT);
		}
		if (direction == 1)
		{
			SetState(INSECT_STATE_JUMP_LEFT);
		}
	}
}

void CInsect::CollisionHorizontal(float nx)
{
	if (nx < 0.0f)
	{
		SetState(INSECT_STATE_FALLING_LEFT);
	}
	else
	{
		SetState(INSECT_STATE_FALLING_RIGHT);
	}
}
