﻿#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Brick.h"
#include "Intro.h"
#include "Bullet.h"
#include "Worms.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_SOPHIA	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON:
	//case OBJECT_TYPE_sophia:

		if (player != NULL)
		{
			DebugOut(L"[ERROR] sophia object was created before!\n");
			return;
		}
		obj = CJason::GetInstance(x, y);
		player = (CJason*)obj;
//=======
//		obj = new CSophia(x, y);
//		player = (CSophia*)obj;
//
//>>>>>>> Player/Sophia
		DebugOut(L"[INFO] Player object created!\n");
		break;

	case OBJECT_TYPE_BRICK: {
		DebugOut(L"[BBOX] token size: %d\n", tokens[4]);
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());	
		DebugOut(L"[BBOX] width: %d\n", width);
		obj = new CBrick(height, width); break;
	}
	case OBJECT_TYPE_INTRO: obj = new CIntro(); break;
	// case OBJECT_TYPE_WORMS: obj = new CWorms(); break;
	case OBJECT_TYPE_ITEMS: obj = new CItems(); break;
	case OBJECT_TYPE_BACKGROUND: obj = new CBackground(); break;
	// case OBJECT_TYPE_PORTAL:
	// {
	// 	float r = atof(tokens[4].c_str());
	// 	float b = atof(tokens[5].c_str());
	// 	int scene_id = atoi(tokens[6].c_str());
	// 	obj = new CPortal(x, y, r, b, scene_id);
	// }
	// break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that sophia is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> brickObjects;
	vector<LPGAMEOBJECT> enemyObjects;


	for (size_t i = 1; i < objects.size(); i++)
	{
		if (dynamic_cast<CBrick*>(objects[i])) {
			brickObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CWorms*>(objects[i])) {
			enemyObjects.push_back(objects[i]);
		}

		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CJason*>(objects[i])) {
			vector<LPGAMEOBJECT> playerCoObjects;
			// player can colli with brick and enemy
			// so we create a CoObject from brick object
			// merge with enemy
			playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), enemyObjects.begin(), enemyObjects.end());
			objects[i]->Update(dt, &playerCoObjects);
		}
		if (dynamic_cast<CWorms*>(objects[i])) {
			// enemy can colli with brick only
			vector<LPGAMEOBJECT> enemyCoObjects = brickObjects;
			objects[i]->Update(dt, &enemyCoObjects);
		}
	}

	// skip the rest if scene was already unloaded (sophia::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow sophia
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::AddObject(LPGAMEOBJECT gameObject)
{
	objects.push_back(gameObject);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)scence)->GetObjects();
	CPlayer* player = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
		case DIK_A:
			player->Reset();
			break;
		case DIK_UP:
			player->KeyUp();
			break;
		case DIK_DOWN:
			player->KeyDown();
			break;
		case DIK_LEFT:
			player->KeyLeft();
			break;	
		case DIK_RIGHT:
			player->KeyRight();
			break;
		case DIK_Z:
			//sophia->fire(objects);
			break;
		case DIK_X:
			player->KeyX();
			break;
	}

	((CPlayScene*)scence)->UpdateObjects(objects);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//vector<LPGAMEOBJECT> objects = ((CPlayScene*)scence)->GetObjects();
	//CPlayer *player = ((CPlayScene*)scence)->GetPlayer();
	//if (player == NULL) return;		//intro screen.
	//switch (KeyCode)
	//{
	//case DIK_SPACE:
	//	player->SetState(STATE_JUMP);
	//	break;
	//case DIK_DOWN:
	//	player->SetState(STATE_CRAWL_IDLE);
	//	break;
	//case DIK_Z:
	//	player->fire(objects);
	//	break;

	//	CSophia* sophia = ((CPlayScene*)scence)->GetPlayer();
	//	switch (KeyCode)
	//	{
	//	case DIK_UP:
	//		sophia->MoveUpKeyUp();
	//		break;
	//	}
	//}
	//((CPlayScene*)scence)->UpdateObjects(objects);

}
void CPlayScenceKeyHandler::KeyState(BYTE * states)
{

	CGame* game = CGame::GetInstance();
	CPlayer* player = ((CPlayScene*)scence)->GetPlayer();
	//if (player->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		player->KeyRight();
	else if (game->IsKeyDown(DIK_LEFT))
		player->KeyLeft();
	else
		player->SetState(PLAYER_STATE_IDLE);
}
