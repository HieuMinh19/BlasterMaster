#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
	float xPlayer;
	float yPlayer;
public:
	CPortal(float l, float t, float r, float b, int scene_id, float x_player, float y_player);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void GetPositionPlayer(float& xPlayer, float& yPlayer);
	void SetPositionPlayer(float xPlayer, float yPlayer);
	int GetSceneId() { return scene_id; }
};