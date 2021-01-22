#include "Skulls.h"

CSkulls::CSkulls()
{
}

CSkulls::CSkulls(float _vx)
{
	this->vx = _vx;
	SetState(SKULLS_STATE_HORIZONTAL);
}

void CSkulls::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKULLS_STATE_HORIZONTAL:
		if (vx >= 0)
		{
			ani = SKULLS_ANI_LEFT;
		}
		else
		{
			ani = SKULLS_ANI_RIGHT;
			vx = -SKULLS_SPEED_HORIZONTAL;
		}
		break;
	case SKULLS_STATE_VERTICAL:
		vy = -SKULLS_SPEED_VERTICAL;
		vx = 0;
		ani = SKULLS_ANI_RIGHT;
		break;
	case SKULLS_STATE_GUN:
		vx = 0;
		vy = 0;
		ani = SKULLS_ANI_GUN_RIGHT;

		break;
	}
}

void CSkulls::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SKULLS_BBOX_WIDTH;
	bottom = y + SKULLS_BBOX_HEIGHT;
}

void CSkulls::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		float distance_x = abs(player->x - x);
		/*if (distance_x < 16.0f)
		{
			if (state == SKULLS_STATE_HORIZONTAL)
			{
				DebugOut(L"Gujnnnnnnnnnnnnnnnnnn \n");
				SetState(SKULLS_STATE_GUN);
			}
			else
			{
				SetState(SKULLS_STATE_VERTICAL);
			}
		}*/
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
				x += dx;
				if (nx != 0)
				{
				}
				if (ny != 0)
				{
				}
			}
			if (dynamic_cast<CPlayer*>(e->obj) || dynamic_cast<CPlayer*>(e->obj))
			{
				isTouchPlayer = 1;
			}
		}
		if (isTouchPlayer) {
			isTouchPlayer = 0;
			x += min_tx * dx;
			y += min_ty * dy;
		}
		if (!isTouchPlayer) {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSkulls::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
