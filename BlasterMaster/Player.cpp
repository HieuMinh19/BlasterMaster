#include "Player.h"

void CPlayer::Reset()
{
	SetState(PLAYER_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}