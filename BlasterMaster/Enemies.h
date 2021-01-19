#pragma once
#include "GameObject.h"

class CEnemies : public CGameObject
{
	int health;

public:
	CEnemies(float x = 0.0f, float y = 0.0f);
	int GetHealth() { return this->health; }

	virtual void RemoveHealth();
	virtual void SetHealth(int health) { this->health = health; }
};

