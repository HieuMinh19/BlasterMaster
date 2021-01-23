#include "Breakable.h"
#include <algorithm>
#include <assert.h>

CBreakable::CBreakable()
{
	this->bboxHeight = BREAKABLE_BBOX_HEIGHT;
	this->bboxWidth = BREAKABLE_BBOX_WIDTH;
	health = BREAKABLE_HEALTH;
}

void CBreakable::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBreakable::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->bboxWidth;
	b = y + this->bboxHeight;
}

void CBreakable::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0) {
		state = OBJECT_STATE_DELETE;
		spawnItem(x,y);
				
	}

}

void CBreakable::spawnItem(float x, float y)
{
	// General object setup
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = new CItems(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(3);
	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}