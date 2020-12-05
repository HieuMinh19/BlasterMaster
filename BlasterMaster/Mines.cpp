#include "Mines.h"

CMines::CMines()
{
	SetState(MINES_ANI_NORMAL);
}

void CMines::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MINES_STATE_NORMAL:
		vx = 0;
		vy = 0;
		ani = MINES_ANI_NORMAL;
		break;
	}
}

void CMines::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MINES_BBOX_WIDTH;
	bottom = y + MINES_BBOX_HEIGHT;
}

void CMines::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CJason* jason = CJason::GetInstance();
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
				}
				if (ny != 0)
				{
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMines::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
