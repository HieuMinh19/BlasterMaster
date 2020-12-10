#include "Domes.h"

CDomes::CDomes()
{

}

CDomes::CDomes(float _vx, float _vy)
{
	SetState(DOMES_STATE_INITIAL);
	this->vx = _vx;
	this->vy = _vy;
}

void CDomes::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DOMES_BBOX_WIDTH;
	bottom = y + DOMES_BBOX_HEIGHT;
}

void CDomes::SetState(int state)
{
	CGameObject::SetState(state);
	this->state = state;
	//DERECTION 1->UP // 2-> LEFT // 3->DOWN // 4->RIGHT
	switch (state)
	{
	case DOMES_STATE_LEFT_UP:
		vx = -DOMES_WALKING_SPEED;
		vy = DOMES_TRACTION;
		direction = 1;
		ani = DOMES_ANI_LEFT_UP;
		break;
	case DOMES_STATE_RIGHT_UP:
		vx = DOMES_WALKING_SPEED;
		vy = DOMES_TRACTION;
		direction = 1;
		ani = DOMES_ANI_RIGHT_UP;
		break;
	case DOMES_STATE_LEFT_LEFT:
		vx = DOMES_TRACTION;
		vy = DOMES_WALKING_SPEED;
		direction = 2;
		ani = DOMES_ANI_LEFT_LEFT;
		break;
	case DOMES_STATE_RIGHT_LEFT:
		vx = DOMES_TRACTION;
		vy = -DOMES_WALKING_SPEED;
		direction = 2;
		ani = DOMES_ANI_RIGHT_LEFT;
		break;
	case DOMES_STATE_RIGHT_DOWN:
		vx = -DOMES_WALKING_SPEED;
		vy = -DOMES_TRACTION;
		direction = 3;
		ani = DOMES_ANI_RIGHT_DOWN;
		break;
	case DOMES_STATE_LEFT_DOWN:
		vx = DOMES_WALKING_SPEED;
		vy = -DOMES_TRACTION;
		direction = 3;
		ani = DOMES_ANI_LEFT_DOWN;
		break;
	case DOMES_STATE_LEFT_RIGHT:
		vx = -DOMES_TRACTION;
		vy = -DOMES_WALKING_SPEED;
		direction = 4;
		ani = DOMES_ANI_LEFT_RIGHT;
		break;
	case DOMES_STATE_RIGHT_RIGHT:
		vx = -DOMES_TRACTION;
		vy = DOMES_WALKING_SPEED;
		direction = 4;
		ani = DOMES_ANI_RIGHT_RIGHT;
		break;
	case DOMES_STATE_UP:
		vx = 0;
		vy = -DOMES_SPEED_FLY_TO;
		break;
	case DOMES_STATE_LEFT:
		vx = -DOMES_SPEED_FLY_TO;
		vy = 0;
		break;
	case DOMES_STATE_DOWN:
		vx = 0.0f;
		vy = DOMES_SPEED_FLY_TO;
		break;
	case DOMES_STATE_RIGHT:
		vx = DOMES_SPEED_FLY_TO;
		vy = 0;
		break;
	}
}

void CDomes::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (state != DOMES_STATE_INITIAL)
		{
			HandleWithoutObstruction();
		}
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
				if (state == DOMES_STATE_INITIAL)
				{
					SetStateAfterFirstCollision(nx, ny);
				}
				else
				{
					CollisionHandleWithBrick(nx, ny);
					HandleFlyToJason(player->x, player->y, nx, ny);
				}

			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CDomes::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CDomes::HandleWithoutObstruction()
{
	//DERECTION 1->UP // 2-> LEFT // 3->DOWN // 4->RIGHT
	int newState = NULL;
	switch (direction) {
	case 1:
		if (state == DOMES_STATE_LEFT_UP) newState = DOMES_STATE_LEFT_LEFT;
		if (state == DOMES_STATE_RIGHT_UP) newState = DOMES_STATE_RIGHT_RIGHT;
		break;
	case 2:
		if (state == DOMES_STATE_LEFT_LEFT) newState = DOMES_STATE_LEFT_DOWN;
		if (state == DOMES_STATE_RIGHT_LEFT) newState = DOMES_STATE_RIGHT_UP;
		break;
	case 3:
		if (state == DOMES_STATE_LEFT_DOWN) newState = DOMES_STATE_LEFT_RIGHT;
		if (state == DOMES_STATE_RIGHT_DOWN) newState = DOMES_STATE_RIGHT_LEFT;
		break;
	case 4:
		if (state == DOMES_STATE_LEFT_RIGHT) newState = DOMES_STATE_LEFT_UP;
		if (state == DOMES_STATE_RIGHT_RIGHT) newState = DOMES_STATE_RIGHT_DOWN;
		break;
	}

	if (newState != NULL) {
		SetState(newState);
		SetState(DOMES_STATE_INITIAL);
	}
}

void CDomes::HandleFlyToJason(float jason_x, float jason_y, float nx, float ny)
{
	float distance_x = abs(jason_x - x);
	float distance_y = abs(jason_y - y);
	int newState = NULL;
	switch (direction)
	{
	case 1:
		if (distance_x < 1.0f && (state == DOMES_STATE_LEFT_UP || state == DOMES_STATE_RIGHT_UP))
		{
			newState = DOMES_STATE_UP;
		}
		break;
	case 2:
		if (distance_y < 1.0f && (state == DOMES_STATE_LEFT_LEFT || state == DOMES_STATE_RIGHT_LEFT))
		{
			newState = DOMES_STATE_LEFT;
		}
		break;
	case 3:
		if (distance_x < 1.0f && (state == DOMES_STATE_LEFT_DOWN || state == DOMES_STATE_RIGHT_DOWN))
		{
			newState = DOMES_STATE_DOWN;
		}
	case 4:
		if (distance_y < 1.0f && (state == DOMES_STATE_LEFT_RIGHT || state == DOMES_STATE_RIGHT_RIGHT))
		{
			newState = DOMES_STATE_RIGHT;
		}
	}

	if (newState != NULL) {
		SetState(newState);
		SetState(DOMES_STATE_INITIAL);
	}
}

void CDomes::SetStateAfterFirstCollision(float nx, float ny)
{
	int newState = NULL;
	if (nx < 0.0f)
	{
		if (vy > 0.0f)
		{
			newState = DOMES_STATE_LEFT_LEFT;
		}
		else
		{
			newState = DOMES_STATE_RIGHT_LEFT;
		}
	}
	if (nx > 0.0f)
	{
		if (vy > 0.0f)
		{
			newState = DOMES_STATE_RIGHT_RIGHT;
		}
		else
		{
			newState = DOMES_STATE_LEFT_RIGHT;
		}
	}

	if (ny > 0.0f)
	{
		if (vx > 0.0f)
		{
			newState = DOMES_STATE_LEFT_DOWN;
		}
		else
		{
			newState = DOMES_STATE_RIGHT_DOWN;
		}

	}
	if (ny < 0.0f)
	{
		if (vx > 0.0f)
		{
			newState = DOMES_STATE_RIGHT_UP;
		}
		else
		{
			newState = DOMES_STATE_LEFT_UP;
		}
	}

	if (newState != NULL) {
		SetState(newState);
	}
}

void CDomes::CollisionHandleWithBrick(float nx, float ny)
{
	//DERECTION 1->UP // 2-> LEFT // 3->DOWN // 4->RIGHT
	int newState = NULL;
	switch (direction) {
	case 1:
		if (state == DOMES_STATE_LEFT_UP && nx > 0.0f)
		{
			newState = DOMES_STATE_LEFT_RIGHT;
		}
		if (state == DOMES_STATE_RIGHT_UP && nx < 0.0f)
		{
			newState = DOMES_STATE_RIGHT_LEFT;
		}
		break;
	case 2:
		if (state == DOMES_STATE_LEFT_LEFT && ny < 0.0f)
		{
			newState = DOMES_STATE_LEFT_UP;
		}
		if (state == DOMES_STATE_RIGHT_LEFT && ny > 0.0f)
		{
			newState = DOMES_STATE_RIGHT_DOWN;
		}
		break;
	case 3:
		if (state == DOMES_STATE_LEFT_DOWN && nx < 0.0f)
		{
			newState = DOMES_STATE_LEFT_LEFT;
		}
		if (state == DOMES_STATE_RIGHT_DOWN && nx > 0.0f)
		{
			newState = DOMES_STATE_RIGHT_RIGHT;
		}
		break;
	case 4:
		if (state == DOMES_STATE_LEFT_RIGHT && ny > 0.0f)
		{
			newState = DOMES_STATE_LEFT_DOWN;
		}
		if (state == DOMES_STATE_RIGHT_RIGHT && ny < 0.0f)
		{
			newState = DOMES_STATE_RIGHT_UP;
		}
		break;
	}

	if (newState != NULL) {
		SetState(newState);
	}
}