#pragma once
#include "Enemies.h"

class CWorms : public CEnemies
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	CWorms();
	virtual void SetState(int state);
};

