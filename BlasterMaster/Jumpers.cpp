#include "Jumpers.h"

CJumpers::CJumpers()
{

}

CJumpers::CJumpers(float _vx)
{
	this->vx = _vx;
	jumpStartAt = GetTickCount();
}

void CJumpers::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JUMPERS_BBOX_WIDTH;
	bottom = y + JUMPERS_BBOX_HEIGHT;
}

void CJumpers::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JUMPERS_STATE_WALKING_LEFT:
		vx = -JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_WALKING_LEFT;
		break;
	case JUMPERS_STATE_WALKING_RIGHT:
		vx = JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_WALKING_RIGHT;
		break;
	case JUMPERS_STATE_JUMP_LEFT:
		vy = -JUMPERS_JUMP_SPEED;
		vx = -JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_JUMP_LEFT;
		break;
	case JUMPERS_STATE_JUMP_RIGHT:
		vy = -JUMPERS_JUMP_SPEED;
		vx = JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_JUMP_RIGHT;
		break;
	case JUMPERS_STATE_JUMP_COLLISION_LEFT:
		vy = 0;
		vx = JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_JUMP_RIGHT;
	case JUMPERS_STATE_JUMP_COLLISION_RIGHT:
		vy = 0;
		vx = -JUMPERS_WALKING_SPEED;
		ani = JUMPERS_ANI_JUMP_LEFT;
	}
}

void CJumpers::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CJason* jason = CJason::GetInstance();
	CGameObject::Update(dt);
	vy += JUMPERS_GRAVITY * dt;

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
				int newState = NULL;
				DWORD now = GetTickCount();

				if (vx > 0.0f)
				{
					if (jump_counter == 0)
					{
						newState = JUMPERS_STATE_WALKING_RIGHT;
					}
				}
				else
				{
					if (jump_counter == 0)
					{
						newState = JUMPERS_STATE_WALKING_LEFT;
					}
				}

				if (now - jumpStartAt >= 1000 && ny <= 0.0f)
				{
					if (x - jason->x < 0)
					{
						newState = JUMPERS_STATE_JUMP_RIGHT;
						if (nx < 0.0f && state == JUMPERS_STATE_JUMP_RIGHT)
						{
							newState = JUMPERS_STATE_JUMP_COLLISION_RIGHT;
						}
					}
					else
					{
						newState = JUMPERS_STATE_JUMP_LEFT;
						if (nx > 0.0f && state == JUMPERS_STATE_JUMP_LEFT)
						{
							newState = JUMPERS_STATE_JUMP_COLLISION_RIGHT;
						}
					}
					jump_counter++;
				}


				if (jump_counter >= 4)
				{
					jumpStartAt = now;
					jump_counter = 0;
				}

				if (newState != NULL)
				{
					SetState(newState);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJumpers::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}