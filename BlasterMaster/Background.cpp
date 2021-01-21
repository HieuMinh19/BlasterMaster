#include "Background.h"
#include "Utils.h"

CBackground::CBackground()
{
}

void CBackground::Render()
{
	DebugOut(L"aaaaaaaaaaaaa%f %f \n", x, y);
	animation_set->at(0)->Render(x, y);
}

void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BACKGROUND_BBOX_WIDTH;
	b = y + BACKGROUND_BBOX_HEIGHT;
}
