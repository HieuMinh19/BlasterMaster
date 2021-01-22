#include "Items.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"

CItems::CItems(float x, float y)
{
	this->x = x;
	this->y = y;

	srand((unsigned)time(0));
	//int randNum = rand()%(max-min + 1) + min; -> random between min -> max
	this->type = rand() % (5) + 601;
	timeLive = GetTickCount();
}

CItems::CItems(int itemType, float x, float y)
{
	this->x = x;
	this->y = y;
	this->type = itemType;
}

void CItems::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (timeLive + ITEMS_TIME_LIVE < GetTickCount()) {
		SetState(OBJECT_STATE_DELETE);
	}
	DebugOut(L"state: %d \n", state);

	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CItems::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

int CItems::getItemType(){
	return this->type;
}

void CItems::setItemType(int typeItem){
	this->type = typeItem;
}


//void CItems::SetState(int state)
//{
//	CGameObject::SetState(state);
//
//	switch (state){
//		
//	}
//
//}

void CItems::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEMS_BBOX_WIDTH;
	bottom = y + ITEMS_BBOX_HEIGHT;
}
void CItems::hasTaken()
{
	x = ITEMS_HAS_TAKEN_X;
}


