#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Sophia.h"
#include "Utils.h"
#include "PlayScence.h"
#include "Scence.h"
#define UI_STATE_0	0
#define UI_STATE_1	1
#define UI_STATE_2	2
#define UI_STATE_3	3
#define UI_STATE_4	4
#define UI_STATE_5	5
#define UI_STATE_6	6
#define UI_STATE_7	7
#define UI_STATE_8	8

#define LOCATION_Y	150
#define LOCATION_X	20

#define ANI_UI_0	0
#define ANI_UI_1	1
#define ANI_UI_2	2
#define ANI_UI_3	3
#define ANI_UI_4	4
#define ANI_UI_5	5
#define ANI_UI_6	6
#define ANI_UI_7	7
#define ANI_UI_8	8

#define UI_BBOX_WIDTH	100
#define UI_BBOX_HEIGHT	80

class CUI : public CGameObject
{
public:
	CUI();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};