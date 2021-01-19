#include "Teleporter.h"
#include "MonsterBullet.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CTeleporter::CTeleporter(float mx, float my)
{
	SetState(TELEPORT_STATE_ARMORL);
	lastTele = lastArmorl = GetTickCount();
	maxX = mx;
	maxY = my;
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
		
	if (state == TELEPORT_STATE_ARMORL) {
		this->SetHealth(TELEPORT_HEALTH);
		if (lastArmorl + TIME_ARMORL < GetTickCount()) {
			SetState(TELEPORT_STATE_NORMAL);
		}
	}
	if (state == TELEPORT_STATE_NORMAL) {
		if (lastTele + TIME_RELOAD < GetTickCount()) {
			lastTele = GetTickCount();
			Teleport();
		}
	}
}

void CTeleporter::Render()
{
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CTeleporter::Teleport()
{
	
	int res = rand() % (4 - 1 + 1) + 1;
	switch (res)
	{
	case 1:
		x = x + 32;
		break;
	case 2:
		x = x - 32;
		break;
	case 3:
		y = y + 32;
		break;
	case 4:
		y = y - 32;
		break;
	}
	DebugOut(L"teltelteltletl ltetleltletle : %i \n", res);

}

