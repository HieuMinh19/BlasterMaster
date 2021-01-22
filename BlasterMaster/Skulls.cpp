#include "Skulls.h"

CSkulls::CSkulls()
{
	this->vx = -SKULLS_SPEED_HORIZONTAL;
	this->state = SKULLS_STATE_WALKING_LEFT;
}

void CSkulls::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKULLS_STATE_WALKING_LEFT:
		ani = SKULLS_ANI_LEFT;
		vx = -SKULLS_SPEED_HORIZONTAL;
		vy = 0;
		break;
	case SKULLS_STATE_WALKING_RIGHT:
		ani = SKULLS_ANI_RIGHT;
		vx = SKULLS_SPEED_HORIZONTAL;
		vy = 0;
		break;
	case SKULLS_STATE_WALKING_UP:
		vy = -SKULLS_SPEED_VERTICAL;
		vx = 0;
		ani = SKULLS_ANI_RIGHT;
		break;
	case SKULLS_STATE_GUN:
		gunAt = GetTickCount();
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

	DebugOut(L"state:::::::: %d \n", state);

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

		int distance_x = abs(player->x - x);
		if (distance_x < 1 && (state == SKULLS_STATE_WALKING_LEFT || state == SKULLS_STATE_WALKING_RIGHT))
		{
			SetState(SKULLS_STATE_GUN);
			Fire(player->x, player->y, x, y);
		}
		if (state == SKULLS_STATE_GUN && GetTickCount() - gunAt > 100)
		{
			gunAt = 0;
			SetState(SKULLS_STATE_WALKING_UP);
		}
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
				if (e->ny != 0) {
					y -= ny * 0.4f;
				}
				if (e->nx != 0) {
					x -= nx * 0.4f;
				}
			}

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
				int distance_x = abs(player->x - x);
				if (ny > 0 && state == SKULLS_STATE_WALKING_UP)
				{
					if (distance_x < 1)
					{
						if (GetTickCount() - gunTime > 2000)
						{
							gunTime = GetTickCount();
							SetState(SKULLS_STATE_GUN);
							Fire(player->x, player->y, x, y);
						}
					}
				}
			}
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

void CSkulls::Fire(float Xp, float Yp, float Xe, float Ye)
{
	//CMonsterBullet(int ani, float Xp, float Yp, float Xe, float Ye, float Vb)
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	if (Xp < Xe)
	{
		obj = new CMonsterBullet(SKULLS_ANI_BULLET_LEFT, SKULLS_ANI_BUMP, Xp, Yp, Xe, Ye, -SKULLS_SPEED_BULLET_X, SKULLS_SPEED_BULLET_Y);
	}
	else
	{
		obj = new CMonsterBullet(SKULLS_ANI_BULLET_RIGHT, SKULLS_ANI_BUMP, Xp, Yp, Xe, Ye, SKULLS_SPEED_BULLET_X, SKULLS_SPEED_BULLET_Y);
	}
	// General object setup
	obj->SetPosition(x + SKULLS_BBOX_WIDTH / 2, y + SKULLS_BBOX_HEIGHT + 1);
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_FLOATERS);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}

