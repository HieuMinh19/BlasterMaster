#include "Breakable.h"

#include "Utils.h"

CBreakable::CBreakable()
{
	this->bboxHeight = BREAKABLE_BBOX_HEIGHT;
	this->bboxWidth = BREAKABLE_BBOX_WIDTH;
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
	}
}