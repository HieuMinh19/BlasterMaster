#include "Trap.h"

void CTrap::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TRAP_BBOX_WIDTH;
	b = y + TRAP_BBOX_WIDTH;
}