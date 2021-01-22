#include "Mines.h"
#include "MonsterBullet.h"
#include "PlayScence.h"


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

	case MINES_STATE_EXPLOSIVE:
		Explosive();
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

			if (dynamic_cast<CPlayer*>(e->obj))
			{
				HandleCollisionPlayer(e, nx, ny);
				SetState(MINES_STATE_EXPLOSIVE);
			}

			if (dynamic_cast<CBrick*>(e->obj))
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

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

void CMines::Explosive()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	float fastSpeed = BULLET_MINE_SPEED_FAST;
	float slowSpeed = BULLET_MINE_SPEED_SLOW
	AddBullet(fastSpeed, animation_sets);
	AddBullet(-fastSpeed, animation_sets);
	AddBullet(slowSpeed, animation_sets);
	AddBullet(-slowSpeed, animation_sets);
	SetState(OBJECT_STATE_DELETE);
}

void CMines::AddBullet(float fastSpeed, CAnimationSets * animation_sets) {
	CGameObject *obj = NULL;
	obj = new CMonsterBullet(BULLET_MINE, BULLET_MINE_ANI, fastSpeed, BULLET_MINE_UP);

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BULLET);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}