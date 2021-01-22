#include "Eyeball.h"
#include "MonsterBullet.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CEyeball::CEyeball()
{
	SetState(EYEBALL_STATE_MOVE);
	lastFire = GetTickCount();
	lastMove = GetTickCount() - 500;
}

void CEyeball::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYEBALL_STATE_MOVE:
		lastFire = GetTickCount();
		break;
	case EYEBALL_STATE_STAND:
		break;
	}
}

void CEyeball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYEBALL_BBOX_WIDTH;
	bottom = y + EYEBALL_BBOX_HEIGHT;
}

void CEyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	CGameObject::Update(dt);
	float res;
	float timerand = rand() % (2000 - 1000 + 1);
	if (lastMove < GetTickCount()) {
		res = rand() % (15 - 1 + 1);
		res -= 15;
		vx = res / 100 *0.4f;
		res = rand() % (15 - 1 + 1);
		res -= 15;
		vy = res / 100 * 0.4f;
	lastMove = GetTickCount() + timerand;
	}

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
	} else {
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

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CTrap*>(e->obj))
			{
				vx *= -1;
				vy *= -1;
			}
		}
	}

	


	if (lastFire + TIME_RELOAD < GetTickCount()) {
		Fire(player->x, player->y, x, y);
		lastFire = GetTickCount();
	}


	
	//DebugOut(L"teltelteltletl ltetleltletle : %i \n", res);

}

void CEyeball::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CEyeball::Fire(float Xp, float Yp, float Xe, float Ye)
{
	//CMonsterBullet(int ani, float Xp, float Yp, float Xe, float Ye, float Vb)
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	obj = new CMonsterBullet(EYEBALL_ANI_BULLET, EYEBALL_ANI_BUMP, Xp, Yp, Xe, Ye, EYEBALL_BULLET_SPEED);
	// General object setup
	obj->SetPosition(x + EYEBALL_BBOX_WIDTH / 2, y + EYEBALL_BBOX_HEIGHT / 2);
	LPANIMATION_SET ani_set = animation_sets->Get(6);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}