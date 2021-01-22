#include "Enemies.h"

CEnemies::CEnemies(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CEnemies::RemoveHealth()
{
}

void CEnemies::HandleCollisionPlayer(LPCOLLISIONEVENT &coEvent, float nx, float ny) 
{
	if (coEvent->ny != 0)
	{
		y -= ny * 0.4f;
	}
	if (coEvent->nx != 0)
	{
		x -= nx * 0.4f;
	}

	CStaticHelpers* helpers = new CStaticHelpers();
	CPlayer* player = helpers->GetPlayer();
	if (!player->untouchable)
	{
		 /*player->setHealth(player->getHealth() - 1);
		 player->StartUntouchable();*/
	}
}