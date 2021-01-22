#pragma once
#include "Enemies.h"
#include "Brick.h"
#include "Utils.h"
#include "Jason.h"
#include "PlayScence.h"
#include "MonsterBullet.h"

#define BOSS_WALKING_SPEED		0.03f
#define BOSS_MAX_GRAVITY			0.5f
#define BOSS_GRAVITY			0.003f

#define BOSS_STATE_WALKING_RIGHT	100
#define BOSS_STATE_WALKING_LEFT	200
#define BOSS_STATE_UNTOUCHABLE	300

#define BOSS_ANI_WALKING_RIGHT		0
#define BOSS_ANI_WALKING_LEFT			1

#define BOSS_BBOX_WIDTH  64
#define BOSS_BBOX_HEIGHT 78
#define TIME_RELOAD                500

#define BOSS_UNTOUCHABLE_TIME 1500
class CBoss : public CEnemies
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD lastFire;
	int health;
public:
	CBoss();
	DWORD untouchable_start;
	bool untouchable;
	static CBoss* __instance;
	virtual void SetState(int state);
	void Explosive();
	void Fire();
	void AddBullet(float fastSpeed, CAnimationSets* animation_sets);
	static CBoss* GetInstance();
	~CBoss() {
		__instance = NULL;
	}
};

