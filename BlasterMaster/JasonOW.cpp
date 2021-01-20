#include <algorithm>
#include "Utils.h"
#include "Player.h"
#include "JasonOW.h"
#include "Game.h"
#include "PlayerBullet.h"
#include "Worms.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

CJasonOW* CJasonOW::__instance = NULL;

CJasonOW::CJasonOW(float x, float y) : CPlayer()
{
	untouchable = 0;
	SetState(STATE_IDLE);
	isSpecialAni = false;
	alpha = 255;
	health = JASON_MAX_HEALTH;
	start_x = x;
	start_y = y;
	ny = -1;
	this->x = x;
	this->y = y;
}

void CJasonOW::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != JASON_OW_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
		alpha = 255;
	}
	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	// TODO: This is a very ugly designed function!!!!

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	// No collision occured, proceed normally

	if (coEventsResult.size() == 0)
	{
		x += dx;
		y += dy;
		DebugOut(L"x:::: %f \n", x);
		DebugOut(L"y:::: %f \n", y);
	}
	else
	{
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//start collision with worm
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CEnemies*>(e->obj)) // if e->obj is enemies
			{
				spawnItem(e->obj->x, e->obj->y);
				e->obj->SetPosition(-1000, 0);			//dirty way.
				
				if (untouchable == 0)
				{
					health--;
					if (health > 0)
						StartUntouchable();
					else
						SetState(STATE_DIE);
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				break;
			}
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJasonOW::Render()
{
	int ani = 0;

	if (vx == 0)
	{
		if (ny == 0) {
			if (nx > 0)
				ani = JASON_OW_ANI_IDLE_RIGHT;
			else
				ani = JASON_OW_ANI_IDLE_LEFT;
		}
		else if (vy == 0) 
		{
			if(ny > 0) ani = JASON_OW_ANI_IDLE_DOWN;
			else if (ny < 0) ani = JASON_OW_ANI_IDLE_TOP;
		}
		else if (ny > 0) 
			ani = JASON_OW_ANI_WALKING_DOWN;
		else 
			ani = JASON_OW_ANI_WALKING_TOP;
	}
	else if (vx > 0)
		ani = JASON_OW_ANI_WALKING_RIGHT;
	else
		ani = JASON_OW_ANI_WALKING_LEFT;
	
	if (untouchable) {
		if (alpha > UNTOUCHABLE_ALPHA)
			alpha = UNTOUCHABLE_ALPHA;
		else 
			alpha = UNTOUCHABLE_ALPHA * 2;
	}

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CJasonOW::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case JASON_OW_STATE_WALKING_RIGHT:
		vx = WALKING_SPEED;
		vy = 0;
		nx = 1;
		ny = 0;
		break;
	case JASON_OW_STATE_WALKING_LEFT:
		vx = -WALKING_SPEED;
		vy = 0;
		nx = -1;
		ny = 0;
		break;
	case JASON_OW_STATE_WALKING_TOP:
		vy = -WALKING_SPEED;
		vx = 0;
		ny = -1;
		nx = 0;
		break;
	case JASON_OW_STATE_WALKING_DOWN:
		vy = WALKING_SPEED;
		vx = 0;
		ny = 1;
		nx = 0;
		break;
	case STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	}
}

void CJasonOW::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JASON_OW_BBOX_WIDTH;
	bottom = y + JASON_OW_BBOX_HEIGHT;
}

/*
Reset Mario status to the beginning state of a scene
*/
void CJasonOW::Reset()
{

}

void CJasonOW::KeyRight()
{
	if (vy != 0) return;
	SetState(JASON_OW_STATE_WALKING_RIGHT);
}

void CJasonOW::KeyLeft()
{
	if (vy != 0) return;
	SetState(JASON_OW_STATE_WALKING_LEFT);
}

void CJasonOW::KeyDown()
{
	if (vx != 0) return;
	SetState(JASON_OW_STATE_WALKING_DOWN);
}

void CJasonOW::KeyUp()
{
	if (vx != 0) return;
	SetState(JASON_OW_STATE_WALKING_TOP);
}

void CJasonOW::KeyZ()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	obj = new CBullet(nx, ANI_JASON);

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BULLET);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);

}

void CJasonOW::spawnItem(float x, float y)
{
	// General object setup
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = new CItems(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(3);
	obj->SetAnimationSet(ani_set);
	
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);
}

CJasonOW* CJasonOW::GetInstance(float x, float y)
{
	if (__instance == NULL) __instance = new CJasonOW(x, y);
	return __instance;
}

CJasonOW* CJasonOW::GetInstance()
{
	if (__instance == NULL) __instance = new CJasonOW();
	return __instance;
}


void CJasonOW::KeyX()
{
	if (isSpecialAni == false && isJump == false)
		SetState(STATE_JUMP);
}

void CJasonOW::KeySHIFT()
{
	return;
}


void CJasonOW::GetOut()
{

}
void CJasonOW::OnKeyUp()
{

}