#include "Player.h"

void CPlayer::Reset()
{
	SetState(PLAYER_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CPlayer::GetNextPosition(float &x, float &y)
{
	x = this->next_x;
	y = this->next_y;
}

void CPlayer::SetNextPosition(float x, float y) 
{
	this->next_x = x;
	this->next_y = y;
}