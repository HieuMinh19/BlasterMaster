#pragma once
#include "GameObject.h"

#define TRAP_BBOX_WIDTH  16
#define TRAP_BBOX_HEIGHT 16

class CTrap : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};