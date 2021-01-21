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
#include "Trap.h"

CSophia* CSophia::__instance = NULL;

CSophia::CSophia(float x, float y) : CPlayer()
{
	level = SOPHIA_LEVEL_NORMAL;
	untouchable = 0;
	SetState(SOPHIA_STATE_IDLE);
	health = SOPHIA_HEALTH;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	isDie = FALSE;
}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!isJump) {
		vy += 0.0005 * dt;
	}
	else {
		if (!isJumpWhileWalk) {
			vy += JUMP_GRAVITY * dt;
		}
		else {
			vy += 0.00058 * dt;
		}
	}
	//Jump checking
	//DebugOut(L"[INFO] y: %d\n", y);
	//DebugOut(L"[INFO] jump: %d\n", isJump);
	if (isJump) {
		if (isJumpWhileWalk) {
			if (GetTickCount() - jump_start < SOPHIA_JUMP_TIME) {
				if (!jumpBack) {
					if (nx > 0) {
						vx = SOPHIA_JUMP_WHILE_WALK_SPEED_X;
					}
					if (nx < 0) {
						vx = -SOPHIA_JUMP_WHILE_WALK_SPEED_X;
					}
				}
				else {
					if (state == SOPHIA_JUMP_BACK_RIGHT) {
						vx = SOPHIA_JUMP_BACK_SPEED_X;
					}
					if (state == SOPHIA_JUMP_BACK_LEFT) {
						vx = -1* SOPHIA_JUMP_BACK_SPEED_X;
					}
				}
			}
			if (GetTickCount() - jump_start > SOPHIA_JUMP_TIME)
			{
				ResetJump();
			}
		}
		else {
			if (GetTickCount() - jump_start > SOPHIA_WALK_JUMP_TIME)
			{
				if (jumpBack) {
					if (state == SOPHIA_JUMP_BACK_RIGHT) {
						vx = SOPHIA_JUMP_BACK_SPEED_X;
					}
					if (state == SOPHIA_JUMP_BACK_LEFT) {
						vx = -1*SOPHIA_JUMP_BACK_SPEED_X;
					}
				}
			}

			if (GetTickCount() - jump_start > SOPHIA_JUMP_TIME)
			{
				ResetJump();
			}
		}
	}
	if (health <= 0 && !isDie) {
		die_start = GetTickCount();
		untouchable = TRUE;
		health = 0;
		isDie = TRUE;
	}
	if (isDie) {
		vx = 0;
		vy = 0;
		state = SOPHIA_STATE_DIE;
		if (GetTickCount() - die_start > SOPHIA_DIE_TIME)
		{
			CGame::GetInstance()->SwitchScene(END_SCENE);
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (untouchable) {
		if (GetTickCount() - untouchable_start > SOPHIA_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
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
				if (isJump) {
					ResetJump();
				}
			} // if Player
			else if (dynamic_cast<CTrap*>(e->obj))
			{
				CTrap* trap = dynamic_cast<CTrap*>(e->obj);
				if (!untouchable && health >0) {
					health= health-2;
					untouchable = TRUE;
					untouchable_start = GetTickCount();
				}
				

			}
			else if (dynamic_cast<CItems*>(e->obj))
			{
				CItems* items = dynamic_cast<CItems*>(e->obj);
				items->hasTaken();
				//health++;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				break;
			}
			//else if (dynamic_cast<CEnemies*>(e->obj)) // if e->obj is enemies
			//{
			//	 spawnItem(e->obj->x, e->obj->y);
			//	e->obj->SetState(OBJECT_STATE_DELETE);			//dirty way.

			//	if (untouchable == 0)
			//	{
			//		//health--;
			//		if (health > 0)
			//			StartUntouchable();
			//		
			//	}
			//}
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
			if (!isJump) {
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
	int ani = 0;
	if (state == SOPHIA_STATE_DIE) {
		ani = SOPHIA_ANI_DIE;
	}
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
			if (!isJump) {
				if (nx > 0) {
					if (state == SOPHIA_STATE_IDLE) {
						ani = SOPHIA_ANI_IDLE_RIGHT;
					}
					else if (state == SOPHIA_STATE_MOVE_UP_RIGHT) {
						// y = y - 16;
						ani = SOPHIA_ANI_MOVE_UP_RIGHT;
					}
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
			if (isJump) {
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
	animation_set->at(ani)->Render(x, y -16, alpha);
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
		if (isJumpWhileWalk) {
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
		if (isJumpWhileWalk) {
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
		if (isJumpWhileWalk) {
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
	new_y = y + SOPHIA_BBOX_UP;
	left = x;
	top = y;

	if (level == SOPHIA_LEVEL_NORMAL)
	{
		right = x + SOPHIA_BBOX_WIDTH;
		bottom = y + SOPHIA_BBOX_HEIGHT;
	/*	right = x + SOPHIA_BBOX_WIDTH;
		bottom = y + SOPHIA_BBOX_HEIGHT;*/
		/* if (state == SOPHIA_STATE_MOVE_UP_RIGHT || state == SOPHIA_STATE_MOVE_UP_LEFT) {
			bottom = new_y + SOPHIA_BBOX_HEIGHT;
		}*/
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
	SetState(SOPHIA_STATE_IDLE);
	SetLevel(SOPHIA_LEVEL_NORMAL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	//reset sate jump
	animation_set->at(SOPHIA_ANI_JUMP_RIGHT)->Reset();
}
//
void CSophia::ResetStandUp()
{
	isMoveUp = FALSE;
	isStandUp = FALSE;
	animation_set->at(SOPHIA_ANI_MOVE_UP_RIGHT)->Reset();
	animation_set->at(SOPHIA_ANI_MOVE_UP_LEFT)->Reset();
}
void CSophia::ResetJump()
{
	isJump = FALSE;
	isJumpWhileWalk = FALSE;
	isWalkAfterJump = FALSE;
	jumpBack = FALSE;
	SetState(SOPHIA_STATE_IDLE);
	//bug this -> can't add object
	SetLevel(SOPHIA_LEVEL_NORMAL);
	//SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	//reset sate jump
	animation_set->at(SOPHIA_ANI_JUMP_RIGHT)->Reset();
	animation_set->at(SOPHIA_ANI_JUMP_LEFT)->Reset();
	animation_set->at(SOPHIA_ANI_JUMP_UP_RIGHT)->Reset();
	animation_set->at(SOPHIA_ANI_JUMP_UP_LEFT)->Reset();

}
void CSophia::fire(vector<LPGAMEOBJECT>& objects)
{
	if (isMoveUp == true && isStandUp != true) {
		return;
	}
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
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
void CSophia::KeyRight()
{
	if (!isJump) {
		if (isStandUp) {
			SetState(SOPHIA_STATE_WALK_UP_RIGHT);
		}
		else {
			SetState(SOPHIA_STATE_WALKING_RIGHT);
		}
	}
	else {
		jumpBack = TRUE;
		SetState(SOPHIA_JUMP_BACK_RIGHT);
	}
}

void CSophia::KeyLeft()
{
	if (!isJump) {
		if (isStandUp) {
			SetState(SOPHIA_STATE_WALK_UP_LEFT);
		}
		else {
			SetState(SOPHIA_STATE_WALKING_LEFT);
		}
	}
	else {
		jumpBack = TRUE;
		SetState(SOPHIA_JUMP_BACK_LEFT);
	}
}

void CSophia::KeyUp()
{
	if (!isMoveUp) {
		moveup_start = GetTickCount();
		isMoveUp = TRUE;
	}
	/*else {
		ResetStandUp();
	}*/
}
void CSophia::KeyDown()
{

}
void CSophia::OnKeyUp() {
	ResetStandUp();
}
void CSophia::KeyX() {
	if (!isJump) {
		isJump = true;
		jump_start = GetTickCount();
		if (vx != 0) {
			isJumpWhileWalk = TRUE;
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
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();


	CGameObject* obj = NULL;
	if (isStandUp) {
		obj = new CBullet(0, ANI_SOPHIA_LEFT);
		// General object setup
		obj->SetPosition(x + CANNON_X, y);
	}
	else {
		obj = new CBullet(nx, ANI_SOPHIA_LEFT);
		// General object setup
		if (nx > 0)
			obj->SetPosition(x + SOPHIA_BBOX_WIDTH -21, y + CANNON_Y);
		else
			obj->SetPosition(x, y + CANNON_Y);

	}
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
	CJason* jason = CJason::GetInstance(this->x, this->y);
	jason->x = this->x + (SOPHIA_BBOX_WIDTH - SOPHIA_BBOX_WIDTH) / 2;
	jason->nx = this->nx;
	jason->y = this->y;
	jason->inTank = false;
	jason->setHealth(JASON_MAX_HEALTH - 3);
	jason->GetOut();
	dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)->SetPlayer(jason);

}

void CSophia::spawnItem(float x, float y)
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
void CSophia::OnKeyUpLeft()
{
	vx = 0;
}
void CSophia::OnKeyUpRight()
{
	vx = 0;
}