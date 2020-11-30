#pragma once
#include "Player.h"
#include "Game.h"
#include "PlayScence.h"
class CStaticHelpers
{
private:
	CPlayer* player;
public:
	CStaticHelpers();
	CPlayer* GetPlayer();
};

