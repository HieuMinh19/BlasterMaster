#include "Brick.h"

CBrick::CBrick(int bboxHeight, int bboxWidth)
{
	this->bboxHeight = bboxHeight;
	this->bboxWidth = bboxWidth;
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->bboxWidth;
	b = y + this->bboxHeight;
}