#include "Floaters.h"

CFloaters::CFloaters()
{
}

CFloaters::CFloaters(float _vx, float _vy)
{
	this->vx = _vx;
	this->vy = _vy;
	SetState(FLOATERS_STATE_HORIZONTAL);
}

void CFloaters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOATERS_BBOX_WIDTH;
	bottom = y + FLOATERS_BBOX_HEIGHT;
}

void CFloaters::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOATERS_STATE_HORIZONTAL:
		vx = -vx;
		if (vx > 0.0f)
		{
			ani = FLOATERS_ANI_WALKING_RIGHT;
		}
		else
		{
			ani = FLOATERS_ANI_WALKING_LEFT;
		}
		break;
	case FLOATERS_STATE_VERTICAL:
		vy = -vy;
		if (vx > 0.0f)
		{
			ani = FLOATERS_ANI_WALKING_RIGHT;
		}
		else
		{
			ani = FLOATERS_ANI_WALKING_LEFT;
		}
		break;
	}
}


void CFloaters::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
				if (nx != 0)
				{
					SetState(FLOATERS_STATE_HORIZONTAL);
				}
				if (ny != 0)
				{
					SetState(FLOATERS_STATE_VERTICAL);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFloaters::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

