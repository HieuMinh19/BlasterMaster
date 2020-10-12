#include "Intro.h"

CIntro::CIntro()
{
	state = 0;
}

void CIntro::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CIntro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BG_BBOX_WIDTH;
	b = y + BG_BBOX_HEIGHT;
}