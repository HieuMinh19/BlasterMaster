#include <iostream>
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
#include "Domes.h"
#include "Jumpers.h"
#include "Insect.h"
#include "Orbs.h"

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
	float _vx;
	float _vy;
	float _species;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = CJason::GetInstance(x, y);
		player = (CJason*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_INTRO: obj = new CIntro(); break;
	case OBJECT_TYPE_WORMS: obj = new CWorms(); break;
	case OBJECT_TYPE_ITEMS: obj = new CItems(); break;
	case OBJECT_TYPE_DOMES:
		_vx = atof(tokens[4].c_str());
		_vy = atof(tokens[5].c_str());
		obj = new CDomes(_vx, _vy);
		break;
	case OBJECT_TYPE_JUMPERS: 
		_vx = atof(tokens[4].c_str());
		obj = new CJumpers(_vx);
		break;
	case OBJECT_TYPE_INSECT:
		_vx = atof(tokens[4].c_str());
		obj = new CInsect(_vx);
		break;
	case OBJECT_TYPE_ORBS:
		_vx = atof(tokens[4].c_str());
		_species = atof(tokens[5].c_str());
		obj = new COrbs(_vx, _species);
		break;
	
	/*case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;*/
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
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
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
		if (dynamic_cast<CDomes*>(objects[i])) {
			enemyObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CJumpers*>(objects[i])) {
			enemyObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CInsect*>(objects[i])) {
			enemyObjects.push_back(objects[i]);
		}
		if (dynamic_cast<COrbs*>(objects[i])) {
			enemyObjects.push_back(objects[i]);
		}
		coObjects.push_back(objects[i]);
	}

	vector<LPGAMEOBJECT> playerCoObjects;
	// player can colli with brick and enemy
	// so we create a CoObject from brick object
	// merge with enemy
	playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
	//playerCoObjects.insert(playerCoObjects.end(), enemyObjects.begin(), enemyObjects.end());
	//
	vector<LPGAMEOBJECT> enemyCoObjects = brickObjects;
	//

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CJason*>(objects[i])) {
			objects[i]->Update(dt, &playerCoObjects);
		}
		if (dynamic_cast<CWorms*>(objects[i])) {
			// enemy can colli with brick only
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CDomes*>(objects[i])) {
			// enemy can colli with brick only
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CJumpers*>(objects[i])) {
			// enemy can colli with brick only
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CInsect*>(objects[i])) {
			// enemy can colli with brick only
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<COrbs*>(objects[i])) {
			// enemy can colli with brick only
			objects[i]->Update(dt, &enemyCoObjects);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
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
	CJason* jason = ((CPlayScene*)scence)->GetPlayer();
	if (jason == NULL) return;		//intro screen.
	switch (KeyCode)
	{
		case DIK_SPACE:
			jason->SetState(STATE_JUMP);
			break;
		case DIK_DOWN:
			jason->SetState(STATE_CRAWL_IDLE);
			break;
		case DIK_Z:
			jason->fire(objects);
			break;
			
	}
	((CPlayScene*)scence)->UpdateObjects(objects);
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CJason* jason = ((CPlayScene*)scence)->GetPlayer();
	if (jason == NULL) return;	//return if intro

	// disable control key when Mario die 
	if (jason->GetState() == STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT)) 
		jason->MoveRight();
	else if (game->IsKeyDown(DIK_LEFT))
		jason->MoveLeft();
	else
		jason->SetState(STATE_IDLE);
}
