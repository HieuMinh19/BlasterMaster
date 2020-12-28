#include "UI.h"

CUI::CUI()
{
}
void CUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CGame* game = CGame::GetInstance();
	// CPlayer* player = ((CPlayScene*)scence)->GetPlayer();
	CGame::GetInstance()->GetCamX(x);
	CGame::GetInstance()->GetCamY(y);
	y += LOCATION_Y;
	x += LOCATION_X;
	
	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();

	if (dynamic_cast<CSophia*>(player)) {
		CSophia* sophia = CSophia::GetInstance();
		state = CSophia::GetInstance()->getHealth();
	}
	else {
		CJason* jason = CJason::GetInstance();
		state = CJason::GetInstance()->getHealth();

		//int a = CJason::GetInstance()->getHealth(state);
		//DebugOut(L"[INFO] Jason Health: %d\n", state);
	}
}
void CUI::Render()
{
	int ani = 0;
	
	if (state == UI_STATE_0) {
		ani = ANI_UI_0;
	}
	else if (state == UI_STATE_1) {
		ani = ANI_UI_1;
	}
	else if (state == UI_STATE_2) {
		ani = ANI_UI_2;
	}
	else if (state == UI_STATE_3) {
		ani = ANI_UI_3;
	}
	else if (state == UI_STATE_4) {
		ani = ANI_UI_4;
	}
	else if (state == UI_STATE_5) {
		ani = ANI_UI_5;
	}
	else if (state == UI_STATE_6) {
		ani = ANI_UI_6;
	}
	else if (state == UI_STATE_7) {
		ani = ANI_UI_7;
	}
	else if (state == UI_STATE_8) {
		ani = ANI_UI_8;
	}
	animation_set->at(ani)->Render(x, y);
}

void CUI::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + UI_BBOX_WIDTH;
	b = y + UI_BBOX_HEIGHT;
}
