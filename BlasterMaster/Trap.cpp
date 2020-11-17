#include "Trap.h"

CTrap::CTrap(int bboxHeight, int bboxWidth)
{
	this->bboxHeight = bboxHeight;
	this->bboxWidth = bboxWidth;
}

void CTrap::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->bboxWidth;
	b = y + this->bboxHeight;
}