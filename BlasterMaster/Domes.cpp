#include "Domes.h"

CDomes::CDomes()
{

}

CDomes::CDomes(float x, float y, float _vx, float _vy)
{
	SetState(DOMES_STATE_INITIAL);
	this->vx = _vx;
	this->vy = _vy;
	max_coordinates_X = x + DOMES_MAX_JOURNEY_X;
	min_coordinates_X = x - DOMES_MAX_JOURNEY_X;
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
	case DOMES_STATE_BEFORE_FLY_UP:
		fly_start_at = GetTickCount();
		vx = 0.0f;
		ani = DOMES_ANI_BEFORE_FLY_UP;
		break;
	case DOMES_STATE_BEFORE_FLY_LEFT:
		fly_start_at = GetTickCount();
		vy = 0.0f;
		ani = DOMES_ANI_BEFORE_FLY_LEFT;
		break;
	case DOMES_STATE_BEFORE_FLY_DOWN:
		fly_start_at = GetTickCount();
		vx = 0.0f;
		ani = DOMES_ANI_BEFORE_FLY_DOWN;
		break;
	case DOMES_STATE_BEFORE_FLY_RIGHT:
		fly_start_at = GetTickCount();
		vy = 0.0f;
		ani = DOMES_ANI_BEFORE_FLY_RIGHT;
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

			if (dynamic_cast<CPlayer*>(e->obj))
			{
				if (e->ny != 0) {
					y -= ny * 0.4f;
				}
				if (e->nx != 0) {
					x -= nx * 0.4f;
				}
			
			}

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
				float distance_x = abs(player->x - x);
				if (state == DOMES_STATE_INITIAL)
				{
					SetStateAfterFirstCollision(nx, ny);
				}
				else
				{
					CollisionHandleWithBrick(nx, ny);
					if (distance_x <= 100)
					{
						HandleFlyToJason(player->x, player->y, nx, ny);
					}
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

//void CDomes::HandleMaxMinJourneyX(float vx)
//{
//	if (vx > 0)
//	{
//		if (state == DOMES_STATE_RIGHT_UP)
//		{
//			SetState(DOMES_STATE_LEFT_UP);
//		}
//		if (state == DOMES_STATE_LEFT_DOWN)
//		{
//			SetState(DOMES_STATE_RIGHT_DOWN);
//		}
//	}
//	if (vx < 0)
//	{
//		if (state == DOMES_STATE_LEFT_UP)
//		{
//			SetState(DOMES_STATE_RIGHT_UP);
//		}
//		if (state == DOMES_STATE_RIGHT_DOWN)
//		{
//			SetState(DOMES_STATE_LEFT_DOWN);
//		}
//	}
//}

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

void CDomes::HandleFlyToJason(float player_x, float player_y, float nx, float ny)
{
	float distance_x = abs(player_x - x);
	float distance_y = abs(player_y - y);
	int newState = NULL;
	DWORD now = GetTickCount();
	if (vx != 0 && distance_x < 1.0f)
	{
		if (state == DOMES_STATE_LEFT_UP || state == DOMES_STATE_RIGHT_UP)
		{
			SetState(DOMES_STATE_BEFORE_FLY_UP);
		}
		if (distance_y >= 35.0f && (state == DOMES_STATE_LEFT_DOWN || state == DOMES_STATE_RIGHT_DOWN))
		{
			SetState(DOMES_STATE_BEFORE_FLY_DOWN);
		}
	}

	if (vy != 0 && distance_y < 1.0f)
	{
		if (state == DOMES_STATE_LEFT_LEFT || state == DOMES_STATE_RIGHT_LEFT)
		{
			SetState(DOMES_STATE_BEFORE_FLY_LEFT);
		}
		if (state == DOMES_STATE_LEFT_RIGHT || state == DOMES_STATE_RIGHT_RIGHT)
		{
			SetState(DOMES_STATE_BEFORE_FLY_RIGHT);
		}
	}

	if (vx == 0 && now - fly_start_at >= 300)
	{
		if (state == DOMES_STATE_BEFORE_FLY_UP)
		{
			vy = -DOMES_SPEED_FLY_TO;
		}
		if (state == DOMES_STATE_BEFORE_FLY_DOWN)
		{
			vy = DOMES_SPEED_FLY_TO;
		}
	}

	if (vy == 0 && now - fly_start_at >= 300)
	{
		if (state == DOMES_STATE_BEFORE_FLY_LEFT)
		{
			vx = -DOMES_SPEED_FLY_TO;
		}
		if (state == DOMES_STATE_BEFORE_FLY_RIGHT)
		{
			vx = DOMES_SPEED_FLY_TO;
		}
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
		if (state == DOMES_STATE_BEFORE_FLY_UP && ny > 0.0f)
		{
			newState = DOMES_STATE_RIGHT_DOWN;
		}
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
		if (state == DOMES_STATE_BEFORE_FLY_LEFT && nx > 0.0f)
		{
			newState = DOMES_STATE_LEFT_RIGHT;
		}
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
		if (state == DOMES_STATE_BEFORE_FLY_DOWN && ny < 0.0f)
		{
			newState = DOMES_STATE_LEFT_UP;
		}
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
		if (state == DOMES_STATE_BEFORE_FLY_RIGHT && nx < 0.0f)
		{
			newState = DOMES_STATE_RIGHT_LEFT;
		}
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