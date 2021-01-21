#include "Player.h"

void CPlayer::Reset()
{
	SetState(PLAYER_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CPlayer::spawnItem(float x, float y)
{
	// General object setup
	CAnimationSets *animation_sets = CAnimationSets::GetInstance();
	CGameObject *obj = new CItems(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(3);
	obj->SetAnimationSet(ani_set);

	dynamic_cast<CPlayScene *>(
		CGame::GetInstance()
			->GetCurrentScene())
		->AddObject(obj);
}