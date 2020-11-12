#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int bboxWidth = BRICK_BBOX_WIDTH;
	int bboxHeight = BRICK_BBOX_HEIGHT;
public:
	CBrick(int bboxHeight, int bboxWidth);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetBBox(int width, int height);
};