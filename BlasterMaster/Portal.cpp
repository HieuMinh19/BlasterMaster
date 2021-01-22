#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id, float x_player, float y_player)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;

	this->xPlayer = x_player;
	this->yPlayer = y_player;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CPortal::GetPositionPlayer(float& xPlayer, float& yPlayer)
{
	xPlayer = this->xPlayer;
	yPlayer = this->yPlayer;
}

void CPortal::SetPositionPlayer(float xPlayer, float yPlayer)
{
	this->xPlayer = xPlayer;
	this->yPlayer = yPlayer;
}
