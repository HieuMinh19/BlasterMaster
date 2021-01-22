#include "Orbs.h"

COrbs::COrbs()
{
}

COrbs::COrbs(float _vx, float _species)
{
	this->vx = _vx;
	this->species = _species;
	if (species == 0)
	{
		if (vx > 0)
		{
			SetState(ORBS_STATE_WALKING_RIGHT);
		}
		else
		{
			SetState(ORBS_STATE_WALKING_LEFT);
		}
	}
	if (species == 1)
	{

	}
}

void COrbs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ORBS_BBOX_WIDTH;
	bottom = y + ORBS_BBOX_HEIGHT;
}

void COrbs::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ORBS_STATE_WALKING_RIGHT:
		vx = ORBS_SPEED_HORIZONTAL;
		vy = 0;
		ani = ORBS_ANI_WALKING_RIGHT;
		break;
	case ORBS_STATE_WALKING_LEFT:
		vx = -ORBS_SPEED_HORIZONTAL;
		vy = 0;
		ani = ORBS_ANI_WALKING_LEFT;
		break;
	case ORBS_STATE_COLLISON_LEFT:
		vx = 0;
		vy = 0;
		ani = ORBS_ANI_SHAKE_COLLISION_LEFT;
		break;
	case ORBS_STATE_COLLISON_RIGHT:
		vx = 0;
		vy = 0;
		ani = ORBS_ANI_SHAKE_COLLISION_RIGHT;
		break;
	case ORBS_STATE_SHAKE_WALKING_UP:
		vx = 0;
		vy = -ORBS_SPEED_VERTICAL;
		if (direction_x == 0)
		{
			ani = ORBS_ANI_SHAKE_LEFT_TO_RIGHT;
		}
		if (direction_x == 1)
		{
			ani = ORBS_ANI_SHAKE_RIGHT_TO_LEFT;
		}
		break;
	case ORBS_STATE_SHAKE_WALKING_DOWN:
		vx = 0;
		vy = ORBS_SPEED_VERTICAL;
		if (direction_x == 0)
		{
			ani = ORBS_ANI_SHAKE_LEFT_TO_RIGHT;
		}
		if (direction_x == 1)
		{
			ani = ORBS_ANI_SHAKE_RIGHT_TO_LEFT;
		}
		break;
	}
}

void COrbs::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	CGameObject::Update(dt);
	//vy += INSECT_GRAVITY * dt;
	now = GetTickCount();

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
		distance_x += fabs(dx);
		distance_y += fabs(dy);
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
				if (species == 0)
				{
					OrbsNormal(nx, ny);
				}
				else
				{
					OrbsFlyTo(nx, ny);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void COrbs::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void COrbs::OrbsNormal(float nx, float ny)
{
	int newState = NULL;
	shake_at = GetTickCount();
	if (nx != 0.0f)
	{
		if (state == ORBS_STATE_WALKING_RIGHT)
		{
			newState = ORBS_STATE_COLLISON_RIGHT;
			direction_x = 0;
			horizontal_counter++;
		}
		if (state == ORBS_STATE_WALKING_LEFT)
		{
			newState = ORBS_STATE_COLLISON_LEFT;
			direction_x = 1;
			horizontal_counter++;
		}
		distance_x = 0;
	}

	if (newState != NULL)
	{
		SetState(newState);
	}
}

void COrbs::OrbsFlyTo(float nx, float ny)
{
}

void COrbs::HandleWithoutCollision()
{
	int newState = NULL;

	if (species == 0)
	{
		if (now - shake_at >= 150)
		{
			if (state == ORBS_STATE_COLLISON_RIGHT)
			{
				newState = ORBS_STATE_WALKING_LEFT;
			}
			if (state == ORBS_STATE_COLLISON_LEFT)
			{
				newState = ORBS_STATE_WALKING_RIGHT;
			}
		}

		if (horizontal_counter >= 4)
		{
			if (distance_x >= 10.0f)
			{
				srand((int)time(0));
				direction_y = rand() % 2;
				if (direction_y == 0)
				{
					newState = ORBS_STATE_SHAKE_WALKING_UP;
				}
				if (direction_y == 1)
				{
					newState = ORBS_STATE_SHAKE_WALKING_DOWN;
				}
				horizontal_counter = 0;
			}
		}
		if (distance_y >= 7.0f)
		{
			if (direction_x == 0)
			{
				newState = ORBS_STATE_WALKING_LEFT;
			}
			if (direction_x == 1)
			{
				newState = ORBS_STATE_WALKING_RIGHT;
			}
			distance_y = 0;
		}
	}
	else
	{

	}

	if (newState != NULL)
	{
		SetState(newState);
	}
}

