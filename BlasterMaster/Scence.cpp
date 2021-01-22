#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

int CScene::GetScreenWidth()
{
	switch (this->id)
	{
	case SECTION_A_ID:
		return SECTION_A_MAX_WIDTH;
		break;
	case SECTION_B_ID:
		return SECTION_B_MAX_WIDTH;
		break;
	case SECTION_C_ID:
		return SECTION_C_MAX_WIDTH;
		break;
	case SECTION_D_ID:
		return SECTION_D_MAX_WIDTH;
		break;
	case SECTION_E_ID:
		return SECTION_E_MAX_WIDTH;
		break;
	case SECTION_F_ID:
		return SECTION_F_MAX_WIDTH;
		break;
	case SECTION_G_ID:
		return SECTION_G_MAX_WIDTH;
		break;
	case SECTION_H_ID:
		return SECTION_H_MAX_WIDTH;
		break;
	case SECTION_I_ID:
		return SECTION_I_MAX_WIDTH;
		break;
	case SECTION_J_ID:
		return SECTION_J_MAX_WIDTH;
		break;
	case SECTION_L_ID:
		return SECTION_L_MAX_WIDTH;
		break;
	case SECTION_M_ID:
		return SECTION_M_MAX_WIDTH;
		break;
	case SECTION_N_ID:
		return SECTION_N_MAX_WIDTH;
		break;
	case SECTION_OVER_WORD_ID:
		return SECTION_OVER_WORD_MAX_WIDTH;
		break;
	case SECTION_FINAL_ID:
		return SECTION_FINAL_MAX_WIDTH;
		break;
	default:
		return 1024;
		break;
	}
}

int CScene::GetScreenHeight()
{
	switch (this->id)
	{
	case SECTION_A_ID:
		return SECTION_A_MAX_HEIGHT;
		break;
	case SECTION_B_ID:
		return SECTION_B_MAX_HEIGHT;
		break;
	case SECTION_C_ID:
		return SECTION_C_MAX_HEIGHT;
		break;
	case SECTION_D_ID:
		return SECTION_D_MAX_HEIGHT;
		break;
	case SECTION_E_ID:
		return SECTION_E_MAX_HEIGHT;
		break;
	case SECTION_F_ID:
		return SECTION_F_MAX_HEIGHT;
		break;
	case SECTION_G_ID:
		return SECTION_G_MAX_HEIGHT;
		break;
	case SECTION_H_ID:
		return SECTION_H_MAX_HEIGHT;
		break;
	case SECTION_I_ID:
		return SECTION_I_MAX_HEIGHT;
		break;
	case SECTION_J_ID:
		return SECTION_J_MAX_HEIGHT;
		break;
	case SECTION_L_ID:
		return SECTION_L_MAX_HEIGHT;
		break;
	case SECTION_M_ID:
		return SECTION_M_MAX_HEIGHT;
		break;
	case SECTION_N_ID:
		return SECTION_N_MAX_HEIGHT;
		break;
	case SECTION_OVER_WORD_ID:
		return SECTION_OVER_WORD_MAX_HEIGHT;
		break;
	case SECTION_FINAL_ID:
		return SECTION_FINAL_MAX_HEIGHT;
		break;
	default:
		return 1024;
		break;
	}
}