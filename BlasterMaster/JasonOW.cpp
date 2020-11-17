#include <algorithm>
#include "Utils.h"
#include "Player.h"

#include "JasonOW.h"
#include "Game.h"
#include "PlayerBullet.h"
#include "Worms.h"
#include "PlayScence.h"

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
	this->x = x;
	this->y = y;
}

void CJasonOW::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != STATE_DIE)
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
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJasonOW::Render()
{
	int ani = 0;
	if (state == STATE_DIE)
		ani = ANI_DIE;
	else
		if (isSpecialAni == false)
		{
			if (isJump == true)
			{
				if (nx < 0)
					ani = ANI_JUMP_LEFT;
				else
					ani = ANI_JUMP_RIGHT;
			}
			else if (vx == 0)
			{
				if (nx > 0) ani = ANI_IDLE_RIGHT;
				else ani = ANI_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = ANI_WALKING_RIGHT;
			else ani = ANI_WALKING_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) ani = ANI_CRAWL_IDLE_RIGHT;
				else ani = ANI_CRAWL_WALKING_RIGHT;
			}
			else if (vx > 0)
				ani = ANI_CRAWL_IDLE_LEFT;
			else ani = ANI_CRAWL_WALKING_LEFT;
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

void CJasonOW::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATE_WALKING_RIGHT:
		vx = WALKING_SPEED;
		nx = 1;
		break;
	case STATE_WALKING_LEFT:
		vx = -WALKING_SPEED;
		nx = -1;
		break;
	case STATE_JUMP:
		vy = -JUMP_SPEED_Y;
		isJump = true;
		break;
	case STATE_IDLE:
		vx = 0;
		break;
	case STATE_DIE:
		vy = -DIE_DEFLECT_SPEED;
		break;
	case STATE_CRAWL_WALKING_RIGHT:
		vx = CRAWL_SPEED;
		nx = 1;
		break;
	case STATE_CRAWL_WALKING_LEFT:
		vx = -CRAWL_SPEED;
		nx = -1;
		break;
	case STATE_CRAWL_IDLE:
		if (isSpecialAni) {
			y -= CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isSpecialAni = false;
		}
		else {
			y += CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isSpecialAni = true;
		}
		vx = 0;
		break;
	}
}

void CJasonOW::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (!isSpecialAni)
	{
		right = x + BBOX_WIDTH;
		bottom = y + BBOX_HEIGHT;
	}
	else
	{
		right = x + CRAWL_BBOX_WIDTH;
		bottom = y + CRAWL_BBOX_HEIGHT;
	}
	DebugOut(L"[ERROR] Máu %i \n", top);

	DebugOut(L"[ERROR] Máu %i \n", bottom);

}

/*
Reset Mario status to the beginning state of a scene
*/
void CJasonOW::Reset()
{
	SetState(STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CJasonOW::KeyRight()
{
	if (isSpecialAni == false)
		SetState(STATE_WALKING_RIGHT);
	else
		SetState(STATE_CRAWL_WALKING_RIGHT);
}

void CJasonOW::KeyLeft()
{
	if (isSpecialAni == false)
		SetState(STATE_WALKING_LEFT);
	else
		SetState(STATE_CRAWL_WALKING_LEFT);
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
	if (__instance == NULL) __instance = new CJason(x, y);
	return __instance;
}

CJasonOW* CJasonOW::GetInstance()
{
	if (__instance == NULL) __instance = new CJason();
	return __instance;
}

void CJasonOW::KeyDown()
{
	if (state == STATE_JUMP)
		return;
	SetState(STATE_CRAWL_IDLE);
}

void CJasonOW::KeyUp()
{

}

void CJasonOW::KeyX()
{
	if (isSpecialAni == false && isJump == false)
		SetState(STATE_JUMP);
}

void CJasonOW::KeySHIFT()
{
	if (this->state != PLAYER_STATE_IDLE || isSpecialAni == true || isJump == true)
		return;
	CSophia* sophia = dynamic_cast<CSophia*> (
		CSophia::GetInstance()
		);

	if (this->x < sophia->x || this->x > sophia->x + SOPHIA_BBOX_WIDTH - BBOX_WIDTH || this->y < sophia->y || this->y > sophia->y + SOPHIA_BBOX_HEIGHT)
		return;

	x = sophia->x +(SOPHIA_BBOX_WIDTH - BBOX_WIDTH) / 2;
	vy = -JUMP_SPEED_Y;

	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)->SetPlayer(sophia);

}


void CJasonOW::GetOut()
{
	this->vy = -JUMP_CHANGE_PLAYER_SPEED;
	isJump = true;
}
