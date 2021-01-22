#include "PlayerBullet.h"
#include "Mines.h"
#include "Utils.h"
#include "PlayScence.h"

CBullet::CBullet(float playerNX, int ani) : CGameObject()
{
	DebugOut(L"[RENDER INFO]this is render\n");
	nx = playerNX;
	animation = ani;
	SetState(BULLET_STATE_FLYING);
	timeDestroy = GetTickCount() + 500;
	this->x = x;
	this->y = y;
}
CBullet::CBullet(int state, int animation, bool brokenBrick = false)
{
	animation = animation;
	brokenBrick = brokenBrick;
	SetState(state);
	timeDestroy = GetTickCount() + TIME_LIVE;
	this->x = x;
	this->y = y;
}
void CBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	if (timeDestroy < GetTickCount() && state != BULLET_STATE_DESTROY && state != OBJECT_STATE_DELETE) {
		timeDestroy = GetTickCount();
		SetState(BULLET_STATE_DESTROY);
	}
	if (timeDestroy + TIME_ANI_DESTROY < GetTickCount() && state == BULLET_STATE_DESTROY) {
		SetState(OBJECT_STATE_DELETE);	
	}
	CGameObject::Update(dt, coObjects);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BULLET_STATE_DESTROY)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		vx = 0;
		vy = 0;

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0 || ny != 0) SetState(BULLET_STATE_DESTROY);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CEnemies*>(e->obj)) // if e->obj is enemies
			{
				spawnItem(e->obj->x, e->obj->y);
				e->obj->SetState(MINES_STATE_EXPLOSIVE);
				e->obj->SetPosition(-1000, 0);			//dirty way.
			}
			else if (dynamic_cast<CBreakable*>(e->obj)) // if e->obj is enemies
			{
				CBreakable* breakable = dynamic_cast<CBreakable*>(e->obj);
				breakable->health--;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBullet::Render()
{
	int ani = 0;
	if (state!= BULLET_STATE_DESTROY)
		ani = animation;
	else
		ani = ANI_DESTROY;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_FLYING:
		vx = BULLET_WALKING_SPEED
		if (nx < 0)
			vx = -BULLET_WALKING_SPEED;
		if (nx == 0)
		{
			vx = 0;
			vy = -BULLET_WALKING_SPEED;
		}
		break;

	case BULLET_STATE_RIGHT:
		vx = BULLET_WALKING_SPEED;
		vy = 0;
		break;

	case BULLET_STATE_LEFT:
		vx = -BULLET_WALKING_SPEED;
		vy = 0;
		break;

	case BULLET_STATE_UP:
		vy = -BULLET_WALKING_SPEED;
		vx = 0;
		break;

	case BULLET_STATE_DOWN:
		vy = BULLET_WALKING_SPEED;
		vx = 0;
		break;

	case BULLET_STATE_DESTROY:
		vx = 0;
		vy = 0;
		y -= BULLET_BBOX_HEIGHT;
		x -= BULLET_BBOX_WIDTH;
		break;
	}
}

void CBullet::spawnItem(float x, float y)
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