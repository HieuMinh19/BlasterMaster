#include "Cannon.h"
#include "MonsterBullet.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CCanon::CCanon()
{
	SetState(CANNON_STATE_NORMAL);
	lastFire = GetTickCount();
	dx = 1;
}

void CCanon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANNON_STATE_NORMAL:
		ani = CANNON_ANI_NORMAL;
		break;
	case CANNON_STATE_OX:
		ani = CANNON_ANI_OX;
		break;
	case CANNON_STATE_OY:
		ani = CANNON_ANI_OY;
		break;
	}
}

void CCanon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANNON_BBOX_WIDTH;
	bottom = y + CANNON_BBOX_HEIGHT;
}

void CCanon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (lastFire + TIME_RELOAD < GetTickCount()) {
		Fire();
		lastFire = GetTickCount();
	}

	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
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

			if (dynamic_cast<CPlayer*>(e->obj))
			{
				HandleCollisionPlayer(e, nx, ny);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CCanon::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CCanon::Fire()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	float yy = this->y + (CANNON_BBOX_HEIGHT - BULLET_BBOX_WIDTH) / 2;
	float xx = this->x + (CANNON_BBOX_HEIGHT - BULLET_BBOX_WIDTH) / 2;
	if (state != CANNON_STATE_OX) {
		SetState(CANNON_STATE_OX);
		AddBullet(BULLET_RIGHT, animation_sets,x+ CANNON_BBOX_WIDTH, yy);
		AddBullet(BULLET_LEFT, animation_sets, x, yy);
		dx = 0;
	}
	else {
		SetState(CANNON_STATE_OY);
		AddBullet(BULLET_UP, animation_sets, xx, y);
		AddBullet(BULLET_DOWN, animation_sets, xx, y + CANNON_BBOX_HEIGHT);
		dx = 1;
	}

}

void CCanon::AddBullet(int state, CAnimationSets * animation_sets, float x, float y) {
	CGameObject *obj = NULL;
	obj = new CMonsterBullet(state, 0);

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