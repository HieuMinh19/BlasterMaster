#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Jason.h"
#include "Items.h"
#include "Config.h"
#include "Sophia.h"
#define PLAYER_JASON		1
#define PLAYER_SOPHIA		2
#include "Background.h"

class CPlayScene : public CScene
{
protected:
	CPlayer * player;					// A play scene has to have player, right? 
//=======
	//CPlayer* sophia;				
	//CPlayer* jason;
	int state;
//>>>>>>> Player/Sophia

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void UpdateObjects(vector<LPGAMEOBJECT> objs) { objects = objs; };

	void AddObject(LPGAMEOBJECT gameObject);
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	CPlayer* GetPlayer() { return player; }
	void SetPlayer(CPlayer * main) { player = main; }
	//CSophia* GetPlayer() { return player; }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

