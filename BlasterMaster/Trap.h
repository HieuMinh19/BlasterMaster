#pragma once
#include "GameObject.h"

#define TRAP_BBOX_WIDTH  16
#define TRAP_BBOX_HEIGHT 16

class CTrap : public CGameObject
{

	int trapboxWidth = TRAP_BBOX_WIDTH;
	int trapboxHeight = TRAP_BBOX_HEIGHT;
public:
	CTrap(int bboxHeight, int bboxWidth);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetBBox(int width, int height);
};