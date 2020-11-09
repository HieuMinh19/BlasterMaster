#include "Floaters.h"

CFloaters::CFloaters()
{
}

CFloaters::CFloaters(float _vx)
{
	this->vx = _vx;
	if (vx > 0)
	{
		SetState(FLOATERS_STATE_WALKING_RIGHT);
	}
	else
	{
		SetState(FLOATERS_STATE_WALKING_LEFT);
	}
}

void CFloaters::SetState(int state)
{
}


void CFloaters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOATERS_BBOX_WIDTH;
	bottom = y + FLOATERS_BBOX_HEIGHT;
}

void CFloaters::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CFloaters::Render()
{
}

void CFloaters::HandleWithoutCollision()
{
}

