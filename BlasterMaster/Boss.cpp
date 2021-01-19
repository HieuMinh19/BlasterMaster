#include "Boss.h"

CBoss::CBoss()
{
	SetState(BOSS_STATE_WALKING_LEFT);
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y + BOSS_BBOX_HEIGHT;
	lastFire = GetTickCount();
}

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_WALKING_LEFT:
		vx = -BOSS_WALKING_SPEED;
		break;
	case BOSS_STATE_WALKING_RIGHT:
		vx = BOSS_WALKING_SPEED;
		break;
	}
}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	CGameObject::Update(dt);
	// Simple fall down
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	float min_tx, min_ty, nx = 0, ny;
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
				
				if (nx < 0) {
					Fire();
					vx = -BOSS_WALKING_SPEED;
				}
				else {
					vx = BOSS_WALKING_SPEED;
				}
			}
		}
		
	}
	
	if (lastFire + TIME_RELOAD < GetTickCount()) {
		x = 1000;
		Fire();
		lastFire = GetTickCount();
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBoss::Render()
{
	int ani;
	if (state = BOSS_STATE_WALKING_RIGHT)
		ani = 0;
	else ani = 1;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CBoss::Fire()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	float yy = this->y + (BOSS_BBOX_HEIGHT - BULLET_BBOX_WIDTH) / 2;
	float xx = this->x + (BOSS_BBOX_HEIGHT - BULLET_BBOX_WIDTH) / 2;
	
		AddBullet(BULLET_UP, animation_sets);
		AddBullet(BULLET_DOWN, animation_sets);
		dx = 1;

}

void CBoss::AddBullet(float fastSpeed, CAnimationSets* animation_sets) {
	CGameObject* obj = NULL;
	obj = new CMonsterBullet(BULLET_MINE, 0, fastSpeed, fastSpeed);

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
