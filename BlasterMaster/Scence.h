#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

// define maximum and minimum every screen
#define SECTION_A_MAX_WIDTH		1024
#define SECTION_A_MAX_HEIGHT	240

#define SECTION_B_MAX_WIDTH		512
#define SECTION_B_MAX_HEIGHT	1320

#define SECTION_C_MAX_WIDTH		512
#define SECTION_C_MAX_HEIGHT	240

#define SECTION_D_MAX_WIDTH		512
#define SECTION_D_MAX_HEIGHT	1008

#define SECTION_E_MAX_WIDTH		1024
#define SECTION_E_MAX_HEIGHT	240

#define SECTION_F_MAX_WIDTH		384
#define SECTION_F_MAX_HEIGHT	1350

#define SECTION_G_MAX_WIDTH		512
#define SECTION_G_MAX_HEIGHT	254

#define SECTION_H_MAX_WIDTH		512
#define SECTION_H_MAX_HEIGHT	220

#define SECTION_I_MAX_WIDTH		512
#define SECTION_I_MAX_HEIGHT	527

#define SECTION_J_MAX_WIDTH		495
#define SECTION_J_MAX_HEIGHT	230

#define SECTION_L_MAX_WIDTH		776
#define SECTION_L_MAX_HEIGHT	221

#define SECTION_M_MAX_WIDTH		251
#define SECTION_M_MAX_HEIGHT	734

#define SECTION_N_MAX_WIDTH		512
#define SECTION_N_MAX_HEIGHT	528

#define SECTION_OVER_WORD_MAX_WIDTH		1536
#define SECTION_OVER_WORD_MAX_HEIGHT	2022
// end define width - height each section

// define id screen (must be map with main.txt file)
#define SECTION_A_ID	7
#define SECTION_B_ID	18
#define SECTION_C_ID	10
#define SECTION_D_ID	19
#define SECTION_E_ID	11
#define	SECTION_F_ID	12
#define	SECTION_G_ID	13
#define	SECTION_H_ID	14
#define	SECTION_I_ID	8
#define	SECTION_J_ID	15
// #define	SECTION_K_ID	12
#define	SECTION_L_ID	16
#define	SECTION_M_ID	17
#define	SECTION_N_ID	9
#define	SECTION_OVER_WORD_ID	20


class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	int GetScreenHeight();
	int GetScreenWidth();
	int getCurrentID() { return id; }
};
typedef CScene* LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};