#pragma once
#include "Utils.h"
#include "Enemies.h"


class CEnemiesBullet : public CEnemies
{
public:
    CEnemiesBullet();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void SetState(int state);
};
