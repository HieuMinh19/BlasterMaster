#include "StaticHelpers.h"

CStaticHelpers::CStaticHelpers()
{
}

CPlayer* CStaticHelpers::GetPlayer()
{
	return dynamic_cast<CPlayScene*> (
		CGame::GetInstance()
		->GetCurrentScene()
		)->GetPlayer();
}
