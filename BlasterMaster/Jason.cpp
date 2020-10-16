#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Jason.h"
#include "Game.h"
#include "Bullet.h"
#include "Worms.h"
#include "items.h"

CJason::CJason(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(STATE_IDLE);
	isCrawl = false;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CJason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	}

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
		float worm_pos_x, worm_pos_y;
		//start collision with worm
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CWorms*>(e->obj)) // if e->obj is worm
			{
				CWorms* worm = dynamic_cast<CWorms*>(e->obj);
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				CGameObject* obj = new CItems();
				// General object setup
				obj->SetPosition(64, 100);
				LPANIMATION_SET ani_set = animation_sets->Get(3);
				DebugOut(L"Animation set %d\n", ani_set->size());
				obj->SetAnimationSet(ani_set);
				coObjects->push_back(obj);
				DebugOut(L"you've just touched worm to the top!! \n");
			}
			else {

			}
		}



	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CJason::Render()
{
	int ani = 0;
	if (state == STATE_DIE)
		ani = ANI_DIE;
	else
		if (isCrawl == false)
		{
			if (vx == 0)
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


	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CJason::SetState(int state)
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
		if (isCrawl) {
			y -= CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isCrawl = false;
		}
		else {
			y += CRAWL_BBOX_HEIGHT;
			RenderBoundingBox();
			isCrawl = true;
		}
		vx = 0;
		break;
	}
}

void CJason::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (!isCrawl)
	{
		right = x + BBOX_WIDTH;
		bottom = y + BBOX_HEIGHT;
	}
	else
	{
		right = x + CRAWL_BBOX_WIDTH;
		bottom = y + CRAWL_BBOX_HEIGHT;
	}
}

/*
Reset Mario status to the beginning state of a scene
*/
void CJason::Reset()
{
	SetState(STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CJason::MoveRight()
{
	if (isCrawl == false)
		SetState(STATE_WALKING_RIGHT);
	else
		SetState(STATE_CRAWL_WALKING_RIGHT);
}

void CJason::MoveLeft()
{
	if (isCrawl == false)
		SetState(STATE_WALKING_LEFT);
	else
		SetState(STATE_CRAWL_WALKING_LEFT);
}


void CJason::fire(vector<LPGAMEOBJECT> &objects)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	obj = new CBullet(nx);

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(6);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

}