#include "Eyeball.h"
#include "MonsterBullet.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CEyeball::CEyeball()
{
	SetState(EYEBALL_STATE_MOVE);
	lastFire = GetTickCount();
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



	if (lastFire + TIME_RELOAD < GetTickCount()) {
		Fire(player->x, player->y, x, y);
		lastFire = GetTickCount();
	}
}

void CEyeball::Render()
{
	animation_set->at(EYEBALL_ANI_NORMAL)->Render(x, y);
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
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_EYEBAll);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}