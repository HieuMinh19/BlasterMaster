#include "Teleporter.h"
#include "MonsterBullet.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CTeleporter::CTeleporter()
{
	SetState(TELEPORT_STATE_ARMORL);
	lastArmorl = GetTickCount();
}

void CTeleporter::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELEPORT_STATE_ARMORL:
		ani = TELEPORT_ANI_ARMORL;
		break;
	case TELEPORT_STATE_NORMAL:
		ani = TELEPORT_ANI_NORMAL;
		break;
	}
}

void CTeleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TELEPORT_BBOX_WIDTH;
	bottom = y + TELEPORT_BBOX_HEIGHT;
}

void CTeleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);



	if (lastArmorl + TIME_ARMORL < GetTickCount()) {
		SetState(TELEPORT_STATE_NORMAL);
	}
}

void CTeleporter::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CTeleporter::Teleport()
{

}

