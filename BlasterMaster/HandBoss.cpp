#include "HandBoss.h"

CHandBoss::CHandBoss()
{

}


void CHandBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
        //    x += nx*abs(rdx); 

        // block every object first!
        x += min_tx * dx + nx * 0.4f;
        y += min_ty * dy + ny * 0.4f;

        if (nx != 0) vx = 0;
        if (ny != 0) vy = 0;
    }

    // clean up collision events
    for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHandBoss::Render()
{
    animation_set->at(0)->Render(x, y);
    RenderBoundingBox();
}


void CHandBoss::SetState(int state)
{


}

void CHandBoss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + 5;
    b = y + 5;
}


