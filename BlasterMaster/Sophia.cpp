#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Sophia.h"
#include "Jason.h"
#include "Game.h"
#include "PlayScence.h"

#include "Portal.h"
#include "PlayerBullet.h"
#include "Brick.h"

CSophia* CSophia::__instance = NULL;

CSophia::CSophia(float x, float y) : CPlayer()
{
	level = SOPHIA_LEVEL_NORMAL;
	untouchable = 0;
	SetState(SOPHIA_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!isJumping) {
		vy += SOPHIA_GRAVITY * dt;
	}
	else {
		vy += JUMP_GRAVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SOPHIA_UNTOUCHABLE_TIME)
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

		// how to push back SOPHIA if collides with a moving objects, what if SOPHIA is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx!=0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (isJumping) {
					}
				}
				else if (e->nx != 0)
				{
					return;
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	//Jump checking
	//DebugOut(L"[INFO] vx: %d\n", isWalkAfterJump);
	//DebugOut(L"[INFO] jump: %d\n", isJumping);
	if (isJumping) {
		if (isJumpingWhileWalk) {
			if (GetTickCount() - jump_start < SOPHIA_JUMP_TIME) {
				if (nx > 0) {
					x += (SOPHIA_JUMP_WHILE_WALK_SPEED_X * dt);
				}
				if (nx < 0) {
					x -= (SOPHIA_JUMP_WHILE_WALK_SPEED_X * dt);
				}//fix this
				if (GetTickCount() - jump_start > SOPHIA_WALK_JUMP_TIME && GetTickCount() - jump_start <= SOPHIA_JUMP_TIME) {
					isWalkAfterJump = TRUE;
				}
			}
			if (GetTickCount() - jump_start > SOPHIA_JUMP_TIME)
			{
			}
		}
	}
	//Stand up checking
	if (isMoveUp && !isStandUp) {
		if (nx > 0) {
			state = SOPHIA_STATE_MOVE_UP_RIGHT;
		}
		else {
			state = SOPHIA_STATE_MOVE_UP_LEFT;
		}
		if (GetTickCount() - moveup_start > SOPHIA_MOVEUP_START)
		{
			isStandUp = TRUE;
		}
	}
	if (isStandUp) {
		if (vx == 0) {
			if (!isJumping) {
				if (nx > 0) {
					state = SOPHIA_STATE_STAND_UP_RIGHT;
				}
				else {
					state = SOPHIA_STATE_STAND_UP_LEFT;
				}
			}
			else {
				if (nx > 0) {
					state = SOPHIA_STATE_JUMP_UP_RIGHT;
				}
				else {
					state = SOPHIA_STATE_JUMP_UP_LEFT;
				}
			}
		}
		else {
			if (nx > 0) {
				state = SOPHIA_STATE_WALK_UP_RIGHT;
			}
			else {
				state = SOPHIA_STATE_WALK_UP_LEFT;
			}
		}
	}
	if (isWalkAfterJump) {
		if (nx > 0) {
			state = SOPHIA_STATE_AFTER_JUMP_RIGHT;
		}
		if (nx < 0) {
			state = SOPHIA_STATE_AFTER_JUMP_LEFT;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSophia::Render()
{
	int ani = -1;
	if (state == SOPHIA_STATE_DIE)
		ani = SOPHIA_ANI_DIE;
	else if (level == SOPHIA_LEVEL_NORMAL)
	{
		if (state == SOPHIA_STATE_UNTOUCHABLE) {
			if (nx > 0) {
				ani = SOPHIA_ANI_UNTOUCHABLE_RIGHT;
			}
			else 
				ani = SOPHIA_ANI_UNTOUCHABLE_LEFT;
		}
		if (vx == 0)
		{
			if (!isJumping) {
				if (nx > 0) {
					if (state == SOPHIA_STATE_IDLE)
						ani = SOPHIA_ANI_IDLE_RIGHT;
					else if (state == SOPHIA_STATE_MOVE_UP_RIGHT)
						ani = SOPHIA_ANI_MOVE_UP_RIGHT;
					else if (state == SOPHIA_STATE_STAND_UP_RIGHT) {
						ani = SOPHIA_ANI_STAND_UP_RIGHT;
					}
					else ani = SOPHIA_ANI_IDLE_RIGHT;
				}
				else {
					if (state == SOPHIA_STATE_IDLE)
						ani = SOPHIA_ANI_IDLE_LEFT;
					else if (state == SOPHIA_STATE_MOVE_UP_LEFT)
						ani = SOPHIA_ANI_MOVE_UP_LEFT;
					else if (state == SOPHIA_STATE_STAND_UP_LEFT) {
						ani = SOPHIA_ANI_STAND_UP_LEFT;
					}
					
					else ani = SOPHIA_ANI_IDLE_LEFT;
				}
			}
			if (isJumping) {
				if (nx > 0) {
					if (state == SOPHIA_STATE_JUMP_UP_RIGHT) {
						ani = SOPHIA_ANI_JUMP_UP_RIGHT;
					}
					else {
						ani = SOPHIA_ANI_JUMP_RIGHT;
					}
				}
				else {
					if (state == SOPHIA_STATE_JUMP_UP_LEFT) {
						ani = SOPHIA_ANI_JUMP_UP_LEFT;
					}
					else {
						ani = SOPHIA_ANI_JUMP_LEFT;
					}
				}
			}
		}
		else if (vx > 0) {
			if (state == SOPHIA_STATE_WALK_UP_RIGHT) {
				ani = SOPHIA_ANI_WALK_UP_RIGHT;
			}
			else if (state == SOPHIA_STATE_AFTER_JUMP_RIGHT) {
				ani = SOPHIA_ANI_AFTER_JUMP_RIGHT;
			}
			else {
				ani = SOPHIA_ANI_WALKING_RIGHT;
			}
		}
		else {
			if (state == SOPHIA_STATE_WALK_UP_LEFT) {
				ani = SOPHIA_ANI_WALK_UP_LEFT;
			}
			else if (state == SOPHIA_STATE_AFTER_JUMP_RIGHT) {
				ani = SOPHIA_ANI_AFTER_JUMP_LEFT;
			}
			else {
				ani = SOPHIA_ANI_WALKING_LEFT;
			}

		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}

void CSophia::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP:
		// TODO: need to check if SOPHIA is *current* on a platform before allowing to jump again
		if (isJumpingWhileWalk) {
			vy = -SOPHIA_JUMP_WHILE_WALK_SPEED_Y;
			if (nx > 0) {
				vx = SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
			else {
				vx = -SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
		}
		else {
			vy = -SOPHIA_JUMP_SPEED_Y;
		}
		break; 	
	case SOPHIA_STATE_JUMP_UP_LEFT:
		// TODO: need to check if SOPHIA is *current* on a platform before allowing to jump again
		if (isJumpingWhileWalk) {
			vy = -SOPHIA_JUMP_WHILE_WALK_SPEED_Y;
			if (nx > 0) {
				vx = SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
			else {
				vx = -SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
		}
		else {
			vy = -SOPHIA_JUMP_SPEED_Y;
		}
		break;
	case SOPHIA_STATE_JUMP_UP_RIGHT:
		// TODO: need to check if SOPHIA is *current* on a platform before allowing to jump again
		if (isJumpingWhileWalk) {
			vy = -SOPHIA_JUMP_WHILE_WALK_SPEED_Y;
			if (nx > 0) {
				vx = SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
			else {
				vx = -SOPHIA_JUMP_WHILE_WALK_SPEED_X;
			}
		}
		else {
			vy = -SOPHIA_JUMP_SPEED_Y;
		}
		break;
	case SOPHIA_STATE_IDLE:
		vx = 0;
		break;
	case SOPHIA_STATE_DIE:
		vy = -SOPHIA_DIE_DEFLECT_SPEED;
		break;
	case SOPHIA_STATE_MOVE_UP_RIGHT:
		level = SOPHIA_LEVEL_MOVE_UP;
		break;
	case SOPHIA_STATE_WALK_UP_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALK_UP_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	}
}


void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	float new_y;
	new_y = y + 15;
	left = x;
	top = new_y;

	if (level == SOPHIA_LEVEL_NORMAL)
	{
		right = x + SOPHIA_BBOX_WIDTH;
		bottom = new_y + SOPHIA_BBOX_HEIGHT;
	}
	else {
		right = x + SOPHIA_MOVE_UP_BBOX_WIDTH;
		bottom = y + SOPHIA_MOVE_UP_BBOX_HEIGHT;
	}
	
}

/*
	Reset SOPHIA status to the beginning state of a scene
*/
void CSophia::Reset()
{

}

void CSophia::SetNx(int a) {
	nx = a;
}
void CSophia::Walk() {
}
void CSophia::WalkUp() {
	if (nx > 0) {
		SetState(SOPHIA_STATE_WALK_UP_RIGHT);
	}
	else {
		SetState(SOPHIA_STATE_WALK_UP_LEFT);
	}
}
void CSophia::MoveUpKeyDown() {
	
}
void CSophia::MoveUpKeyUp() {
	isStandUp = FALSE;
	isMoveUp = FALSE;
	Reset();
}
void CSophia::fire(vector<LPGAMEOBJECT>& objects)
{
	if (isMoveUp == true && isStandUp != true) {
		return;
	}
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
void CSophia::SetWalk() {
	if (!isJumping) {
		if (isStandUp) {
			WalkUp();
		}
		else
			Walk();
	}
}
void CSophia::KeyRight()
{
	if (isStandUp) {
		SetState(SOPHIA_STATE_WALK_UP_RIGHT);
	}
	else {
		SetState(SOPHIA_STATE_WALKING_RIGHT);
	}
}

void CSophia::KeyLeft()
{
	if (isStandUp) {
		SetState(SOPHIA_STATE_WALK_UP_LEFT);
	} else{
		SetState(SOPHIA_STATE_WALKING_LEFT);
	}
}

void CSophia::KeyUp()
{
	moveup_start = GetTickCount();
	isMoveUp = TRUE;
}
void CSophia::KeyDown()
{

}

void CSophia::KeyX() {
	if (!isJumping) {
		isJumping = true;
		jump_start = GetTickCount();
		if (vx != 0) {
			isJumpingWhileWalk = TRUE;
		}
		if (nx > 0) {
			if (isStandUp) {
				SetState(SOPHIA_STATE_JUMP_UP_RIGHT);
			}
			else
				SetState(SOPHIA_STATE_JUMP);
		}
		else {
			if (isStandUp) {
				SetState(SOPHIA_STATE_JUMP_UP_LEFT);
			}
			else
				SetState(SOPHIA_STATE_JUMP);
		}
	}
}
void CSophia::KeyZ()
{
	if (isMoveUp == true && isStandUp != true) {
		return;
	}
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	obj = new CBullet(nx, ANI_SOPHIA_LEFT);

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
CSophia* CSophia::GetInstance(float x, float y)
{
	if (__instance == NULL) __instance = new CSophia(x, y);
	return __instance;
}
CSophia* CSophia::GetInstance()
{
	if (__instance == NULL) __instance = new CSophia();
	return __instance;
}

void CSophia::KeySHIFT()
{
	if (this->state != SOPHIA_STATE_IDLE)
		return;
	CJason* jason = dynamic_cast<CJason*> (
		CJason::GetInstance()
		);
	jason->x = this->x + (SOPHIA_BBOX_WIDTH + BBOX_WIDTH) / 2;
	jason->nx = this->nx;
	jason->y = this->y;
	jason->GetOut();
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)->SetPlayer(jason);

}
