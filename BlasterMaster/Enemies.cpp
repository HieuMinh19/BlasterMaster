#include "Enemies.h"

CEnemies::CEnemies(float x, float y)
{
}

void CEnemies::RemoveHealth()
{
}

void CEnemies::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CEnemies::GetBoundingBox(float& left, float& top, float& right, float& bottom, int bbox_width, int bbox_height)
{
	left = x;
	top = y;
	right = x + bbox_width;
	bottom = y + bbox_height;
}