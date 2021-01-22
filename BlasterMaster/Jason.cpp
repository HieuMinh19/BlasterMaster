#include <algorithm>
#include "Utils.h"
#include "Player.h"

#include "Jason.h"
#include "Game.h"
#include "PlayerBullet.h"
#include "Worms.h"
#include "PlayScence.h"

CJason* CJason::__instance = NULL;

CJason::CJason(float x, float y) : CPlayer()
{
	untouchable = 0;
	SetState(JASON_STATE_IDLE);
	isSpecialAni = false;
	alpha = 255;
	health = JASON_MAX_HEALTH;
	inTank = true;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	isDie = 0;
}

void CJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!inTank) {
		vy += JASON_GET_OUT_TANK_SPEED * dt;
	}
	else {
		SetState(JASON_STATE_CRAWL_IN_TANK);
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != JASON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > JASON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
		alpha = 255;
	}
	if (untouchable) {
		if (GetTickCount() - untouchable_start > JASON_DIE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
	}
	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;

	// TODO: This is a very ugly designed function!!!!
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	// No collision occured, proceed normally
	if (isDie) {
		vx = 0;
		vy = 0;
		state = JASON_STATE_DIE;
	
		if (GetTickCount() - die_start > JASON_DIE_TIME)
		{
			CGame::GetInstance()->SwitchScene(END_SCENE);
		}
	}
	if (coEventsResult.size() == 0)
	{
		x += dx;
		y += dy;
		if (dy == 0)
			isJump = false;
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
				// spawnItem(e->obj->x, e->obj->y);
				//e->obj->SetState(OBJECT_STATE_DELETE);

				if (untouchable == 0)
				{
					health--;
					
					//DebugOut(L"[DEBUG] getHealth: %d\n", getHealth());
					if (health > 0)
						StartUntouchable();
					else
						SetState(JASON_STATE_DIE);
				}
			}
			else if (dynamic_cast<CItems*>(e->obj))
			{
				CItems* items = dynamic_cast<CItems*>(e->obj);
				items->hasTaken();
				health++;
			}
			else if (dynamic_cast<CTrap*>(e->obj))
			{
				CTrap* trap = dynamic_cast<CTrap*>(e->obj);
				if (!untouchable && health > 0) {
					DebugOut(L"[INFO] health: %d\n", health);
					health = health - 2;
					untouchable = 1;
					untouchable_start = GetTickCount();
				}
				if (health <= 0 && !isDie) {
					die_start = GetTickCount();
					untouchable = 1;
					health = 0;
					isDie = 1;
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

void CJason::Render()
{
	int ani = 0;
	if (state == JASON_STATE_DIE)
		ani = JASON_ANI_DIE;
	else
		if (isSpecialAni == false)
		{
			if (isJump == true)
			{
				if (nx < 0)
					ani = JASON_ANI_JUMP_LEFT;
				else
					ani = JASON_ANI_JUMP_RIGHT;
			}
			else if (vx == 0)
			{
				if (nx > 0) ani = JASON_ANI_IDLE_RIGHT;
				else ani = JASON_ANI_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = JASON_ANI_WALKING_RIGHT;
			else ani = JASON_ANI_WALKING_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) ani = JASON_ANI_CRAWL_IDLE_RIGHT;
				else ani = JASON_ANI_CRAWL_WALKING_RIGHT;
			}
			else if (vx > 0)
				ani = JASON_ANI_CRAWL_IDLE_LEFT;
			else ani = JASON_ANI_CRAWL_WALKING_LEFT;
		}
	if (untouchable) {
		if (alpha > UNTOUCHABLE_ALPHA)
			alpha = UNTOUCHABLE_ALPHA;
		else 
			alpha = UNTOUCHABLE_ALPHA * 2;
	}

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CJason::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case JASON_STATE_WALKING_RIGHT:
		vx = JASON_WALKING_SPEED;
		nx = 1;
		break;
	case JASON_STATE_WALKING_LEFT:
		vx = -JASON_WALKING_SPEED;
		nx = -1;
		break;
	case JASON_STATE_JUMP:
		vy = -JASON_JUMP_SPEED_Y;
		isJump = true;
		break;
	case JASON_STATE_IDLE:
		vx = 0;
		break;
	case JASON_STATE_DIE:
		vy = -JASON_DIE_DEFLECT_SPEED;
		break;
	case JASON_STATE_CRAWL_WALKING_RIGHT:
		vx = JASON_CRAWL_SPEED;
		nx = 1;
		break;
	case JASON_STATE_CRAWL_WALKING_LEFT:
		vx = -JASON_CRAWL_SPEED;
		nx = -1;
		break;
	case JASON_STATE_CRAWL_IDLE:
		if (!isSpecialAni) {
			y += JASON_CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isSpecialAni = true;
		}
		vx = 0;
		break;
	case JASON_STATE_CRAWL_STAND_UP:
		if (isSpecialAni) {
			y -= JASON_CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isSpecialAni = false;
		}
		vx = 0;
		break;
	case JASON_STATE_CRAWL_IN_TANK:
		y = 100000;
		vy = 0;
	}
}

void CJason::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (!isSpecialAni)
	{
		right = x + JASON_BBOX_WIDTH;
		bottom = y + JASON_BBOX_HEIGHT;
	}
	else
	{
		right = x + JASON_CRAWL_BBOX_WIDTH;
		bottom = y + JASON_CRAWL_BBOX_HEIGHT;
	}
}

/*
Reset Mario status to the beginning state of a scene
*/
void CJason::Reset()
{
	SetState(JASON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CJason::KeyRight()
{
	if (isSpecialAni == false)
		SetState(JASON_STATE_WALKING_RIGHT);
	else
		SetState(JASON_STATE_CRAWL_WALKING_RIGHT);
}	

void CJason::KeyLeft()
{
	if (isSpecialAni == false)
		SetState(JASON_STATE_WALKING_LEFT);
	else
		SetState(JASON_STATE_CRAWL_WALKING_LEFT);
}


void CJason::KeyZ()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	obj = new CBullet(nx, ANI_JASON);

	// General object setup
	obj->SetPosition(x, y + JASON_BULLET_BBOX);
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BULLET);

	obj->SetAnimationSet(ani_set);
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)
		->AddObject(obj);

}

void CJason::spawnItem(float x, float y)
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

CJason* CJason::GetInstance(float x, float y)
{
	//DebugOut(L"[GO OUT] instance\n");
	if (__instance == NULL) {
		//DebugOut(L"[GO IF] instance\n");
		__instance = new CJason(x, y);
	}
	return __instance;
}

CJason* CJason::GetInstance()
{
	//DebugOut(L"[DEBUG] __instance: %d\n", __instance->getHealth());
	//DebugOut(L"[DEBUG] xxxxxxxx");
	if (__instance == NULL) {
		//DebugOut(L"[GO IF]\n");
		__instance = new CJason();
	}
	return __instance;
}

void CJason::KeyDown()
{
	if (state == JASON_STATE_JUMP)
		return;
	SetState(JASON_STATE_CRAWL_IDLE);
}

void CJason::KeyUp()
{
	SetState(JASON_STATE_CRAWL_STAND_UP);
}
void CJason::OnKeyUp()
{

}
void CJason::KeyX()
{
	if (isSpecialAni == false && isJump == false)
		SetState(JASON_STATE_JUMP);
}

void CJason::KeySHIFT()
{
	if (this->state != PLAYER_STATE_IDLE || isSpecialAni == true || isJump == true)
		return;
	CSophia* sophia = dynamic_cast<CSophia*> (
		CSophia::GetInstance()
		);

	if (this->x < sophia->x || this->x > sophia->x + SOPHIA_BBOX_WIDTH - JASON_BBOX_WIDTH || this->y < sophia->y || this->y > sophia->y + SOPHIA_BBOX_HEIGHT)
		return;

	x = sophia->x +(SOPHIA_BBOX_WIDTH - JASON_BBOX_WIDTH) / 2;
	vy = -JASON_JUMP_SPEED_Y;
	inTank = true;
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)->SetPlayer(sophia);

}


void CJason::GetOut()
{
	this->vy = -JASON_JUMP_CHANGE_PLAYER_SPEED;
	isJump = true;
}

void CJason::OnKeyUpLeft()
{

}
void CJason::OnKeyUpRight()
{

}