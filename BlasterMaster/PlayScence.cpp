#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Brick.h"
#include "Trap.h"
#include "Intro.h"
#include "PlayerBullet.h"
#include "Worms.h"
#include "Breakable.h"
#include "JasonOW.h"
#include "Domes.h"
#include "Jumpers.h"
#include "Insect.h"
#include "Orbs.h"
#include "Floaters.h"
#include "MonsterBullet.h"
#include "Skulls.h"
#include "Mines.h"
#include "Boss.h"
#include "Cannon.h"
#include "Teleporter.h"
#include "Cannon.h"
#include "Teleporter.h"
#include "Boss.h"
#include "Eyeball.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath)
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

	if (tokens.size() < 5)
		return; // skip invalid lines

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

	if (tokens.size() < 6)
		return; // skip invalid lines

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

	if (tokens.size() < 3)
		return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2) // why i+=2 ?  sprite_id | frame_time
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

	if (tokens.size() < 2)
		return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

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

	if (tokens.size() < 3)
		return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());
	float _vx;
	float _vy;
	float _species;

	CAnimationSets *animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON:
		obj = CJason::GetInstance(x, y);
		DebugOut(L"[INFO] Player object created! %d\n");
		break;

	case OBJECT_TYPE_SOPHIA:
		obj = CSophia::GetInstance(x, y);
		player = (CSophia *)obj;
		break;

	case OBJECT_TYPE_JASON_OW:
		obj = CJasonOW::GetInstance(x, y);
		DebugOut(L"[INFO] Player object created!\n");
		player = (CJasonOW *)obj;
		break;
	case OBJECT_TYPE_BRICK:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new CBrick(height, width);
		break;
	}
	case OBJECT_TYPE_INTRO:
		obj = new CIntro();
		break;
	case OBJECT_TYPE_WORMS:
		obj = new CWorms();
		break;
	case OBJECT_TYPE_ITEMS:
		obj = new CItems();
		break;
	case OBJECT_TYPE_TRAP:
	{
		int width = atof(tokens[4].c_str());
		int height = atof(tokens[5].c_str());
		obj = new CTrap(height, width);
	}
	break;
	case OBJECT_TYPE_BACKGROUND:
		obj = new CBackground();
		break;
	case OBJECT_TYPE_BREAKABLE:
		obj = new CBreakable();
		break;
	case OBJECT_TYPE_UI:
		obj = new CUI();
		break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		float x_player = 0;
		float y_player = 0;
		if (tokens.size() >= 8) {
			x_player = atoi(tokens[7].c_str());
			y_player = atoi(tokens[8].c_str());
		}
		
		obj = new CPortal(x, y, r, b, scene_id, x_player, y_player);
	}
	break;
	//start merge enemies
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
	case OBJECT_TYPE_DOMES:
		_vx = atof(tokens[4].c_str());
		_vy = atof(tokens[5].c_str());
		obj = new CDomes(x, y, _vx, _vy);
		break;
	case OBJECT_TYPE_FLOATERS:
		_vx = atof(tokens[4].c_str());
		_vy = atof(tokens[5].c_str());
		obj = new CFloaters(_vx, _vy);
		break;
	case OBJECT_TYPE_SKULLS:
		obj = new CSkulls();
		break;
	case OBJECT_TYPE_MINES:
		obj = new CMines();
		break;
	case OBJECT_TYPE_BOSS:
		obj = new CBoss();
		break;
	case OBJECT_TYPE_CANNON:
		obj = new CCanon();
		break;
	case OBJECT_TYPE_TELEPORT:
		obj = new CTeleporter(0, 1000);
		break;
	case OBJECT_TYPE_EYEBAll:
		obj = new CEyeball();
		DebugOut(L"create eyeball: %i\n", OBJECT_TYPE_EYEBAll);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup

	obj->SetPosition(x, y);
	
	if (dynamic_cast<CPlayer*>(obj)) {
		float x, y;
		obj->GetPosition(x, y);
		DebugOut(L"Debug position X %f\n", x);
		DebugOut(L"Debug position Y %f\n", y);

	}

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
		if (line[0] == '#')
			continue; // skip comment lines

		if (line == "[TEXTURES]")
		{
			section = SCENE_SECTION_TEXTURES;
			continue;
		}
		if (line == "[SPRITES]")
		{
			section = SCENE_SECTION_SPRITES;
			continue;
		}
		if (line == "[ANIMATIONS]")
		{
			section = SCENE_SECTION_ANIMATIONS;
			continue;
		}
		if (line == "[ANIMATION_SETS]")
		{
			section = SCENE_SECTION_ANIMATION_SETS;
			continue;
		}
		if (line == "[OBJECTS]")
		{
			section = SCENE_SECTION_OBJECTS;
			continue;
		}
		if (line[0] == '[')
		{
			section = SCENE_SECTION_UNKNOWN;
			continue;
		}

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES:
			_ParseSection_TEXTURES(line);
			break;
		case SCENE_SECTION_SPRITES:
			_ParseSection_SPRITES(line);
			break;
		case SCENE_SECTION_ANIMATIONS:
			_ParseSection_ANIMATIONS(line);
			break;
		case SCENE_SECTION_ANIMATION_SETS:
			_ParseSection_ANIMATION_SETS(line);
			break;
		case SCENE_SECTION_OBJECTS:
			_ParseSection_OBJECTS(line);
			break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that sophia is the first object in the list hence we won't add him into the colliable object list

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> brickObjects;
	vector<LPGAMEOBJECT> enemyObjects;
	vector<LPGAMEOBJECT> enemyBulletObjects;
	vector<LPGAMEOBJECT> trapObjects;
	vector<LPGAMEOBJECT> itemObjects;
	vector<LPGAMEOBJECT> bulltetObjects;
	vector<LPGAMEOBJECT> breakableObjects;
	vector<LPGAMEOBJECT> portalObjects;
	vector<LPGAMEOBJECT> uiObjects;
	vector<LPGAMEOBJECT> playerObjects;

	for (size_t i = 1; i < objects.size(); i++)
	{
		if (dynamic_cast<CBrick *>(objects[i]))
		{
			brickObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CEnemies *>(objects[i]))
		{
			enemyObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CItems *>(objects[i]))
		{
			itemObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CTrap *>(objects[i]))
		{
			trapObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CBreakable *>(objects[i]))
		{
			breakableObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CBullet *>(objects[i]))
		{
			bulltetObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CPortal *>(objects[i]))
		{
			portalObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CMonsterBullet *>(objects[i]))
		{
			enemyBulletObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CUI *>(objects[i]))
		{
			uiObjects.push_back(objects[i]);
		}
		if (dynamic_cast<CPlayer *>(objects[i]))
		{
			playerObjects.push_back(objects[i]);
			uiObjects.push_back(objects[i]);
		}
		// if (dynamic_cast<CBoss *>(objects[i]))
		// {
		// 	enemyObjects.push_back(objects[i]);
		// }
		coObjects.push_back(objects[i]);
	}

	CStaticHelpers *helpers = new CStaticHelpers();
	CPlayer *player = helpers->GetPlayer();
	if(player != NULL){
		float xPlayer, yPlayer;
		player->GetPosition(xPlayer, yPlayer);
	
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//start phan hoach khong gian
		if (player != NULL) {
			float pos_x = abs(player->x - objects[i]->x);
			float pos_y = abs(player->y - objects[i]->y);
			if (
				!dynamic_cast<CBackground*>(objects[i]) &&
				!dynamic_cast<CUI*>(objects[i]) &&
				pos_x > SCREEN_WIDTH&&
				pos_y > SCREEN_HEIGHT
				)
			{
				continue;
			}
		}
		// end phan hoach khong gian

		if (objects[i]->state == OBJECT_STATE_DELETE)
		{
			objects[i]->deleteObject(objects, i);
		}

		if (dynamic_cast<CJason *>(objects[i]))
		{
			CGame* _cGame = CGame::GetInstance();
			int beforeUpdateScence = _cGame->GetCurrentSceneId();
			vector<LPGAMEOBJECT> playerCoObjects;
			playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), trapObjects.begin(), trapObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), breakableObjects.begin(), breakableObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), portalObjects.begin(), portalObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), itemObjects.begin(), itemObjects.end());
			
			objects[i]->Update(dt, &playerCoObjects);
			
			int afterUpdateScence = _cGame->GetCurrentSceneId();
			if (beforeUpdateScence != afterUpdateScence) {
				break;
			}
		}
		if (dynamic_cast<CJasonOW *>(objects[i]))
		{
			// enemy can colli with brick only
			CGame* _cGame = CGame::GetInstance();
			int beforeUpdateScence = _cGame->GetCurrentSceneId();
			
			vector<LPGAMEOBJECT> playerCoObjects;
			playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), trapObjects.begin(), trapObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), breakableObjects.begin(), breakableObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), portalObjects.begin(), portalObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), itemObjects.begin(), itemObjects.end());

			objects[i]->Update(dt, &playerCoObjects);
			
			int afterUpdateScence = _cGame->GetCurrentSceneId();
			if (beforeUpdateScence != afterUpdateScence) {
				break;
			}
		}
		if (dynamic_cast<CUI *>(objects[i]))
		{
			vector<LPGAMEOBJECT> enemyCoObjects = uiObjects;
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CEnemies *>(objects[i]))
		{
			// enemy can colli with brick only
			vector<LPGAMEOBJECT> enemyCoObjects;
			enemyCoObjects.insert(enemyCoObjects.end(), brickObjects.begin(), brickObjects.end());
			enemyCoObjects.insert(enemyCoObjects.end(), trapObjects.begin(), trapObjects.end());
			enemyCoObjects.insert(enemyCoObjects.end(), playerObjects.begin(), playerObjects.end());

			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CBreakable *>(objects[i]))
		{
			// enemy can colli with brick only
			objects[i]->Update(dt, &coObjects);
		}
		if (dynamic_cast<CMines *>(objects[i]))
		{
			// enemy can colli with brick only
			vector<LPGAMEOBJECT> enemyCoObjects = brickObjects;
			objects[i]->Update(dt, &enemyCoObjects);
		}

		if (dynamic_cast<CTeleporter *>(objects[i]))
		{
			vector<LPGAMEOBJECT> enemyCoObjects = brickObjects;
			objects[i]->Update(dt, &enemyCoObjects);
		}
		if (dynamic_cast<CBullet *>(objects[i]))
		{
			vector<LPGAMEOBJECT> bulltetCoObjects;
			bulltetCoObjects.insert(bulltetCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			bulltetCoObjects.insert(bulltetCoObjects.end(), enemyObjects.begin(), enemyObjects.end());
			bulltetCoObjects.insert(bulltetCoObjects.end(), breakableObjects.begin(), breakableObjects.end());

			objects[i]->Update(dt, &bulltetCoObjects);
		}
		if (dynamic_cast<CMonsterBullet *>(objects[i]))
		{
			vector<LPGAMEOBJECT> bulltetCoObjects;
			bulltetCoObjects.insert(bulltetCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			bulltetCoObjects.insert(bulltetCoObjects.begin(), trapObjects.begin(), trapObjects.end());
			bulltetCoObjects.insert(bulltetCoObjects.end(), breakableObjects.begin(), breakableObjects.end());

			objects[i]->Update(dt, &bulltetCoObjects);
		}
		if (dynamic_cast<CSophia*>(objects[i]))
		{
			CGame* _cGame = CGame::GetInstance();
			int beforeUpdateScence = _cGame->GetCurrentSceneId();
			vector<LPGAMEOBJECT> playerCoObjects;
			playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), trapObjects.begin(), trapObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), breakableObjects.begin(), breakableObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), portalObjects.begin(), portalObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), itemObjects.begin(), itemObjects.end());

			objects[i]->Update(dt, &playerCoObjects);
			
			int afterUpdateScence = _cGame->GetCurrentSceneId();
			if (beforeUpdateScence != afterUpdateScence) {
				break;
			}
		}
		if (dynamic_cast<CJason*>(objects[i]))
		{
			vector<LPGAMEOBJECT> playerCoObjects;
			playerCoObjects.insert(playerCoObjects.begin(), brickObjects.begin(), brickObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), trapObjects.begin(), trapObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), breakableObjects.begin(), breakableObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), portalObjects.begin(), portalObjects.end());
			playerCoObjects.insert(playerCoObjects.end(), itemObjects.begin(), itemObjects.end());
			
			objects[i]->Update(dt, &playerCoObjects);
		}
	}

	// skip the rest if scene was already unloaded (sophia::Update might trigger PlayScene::Unload)
	if (player == NULL)
		return;

	// Update camera to follow sophia
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	// start handle limit max and min x_cam, y_cam
	CScene* scene = CGame::GetInstance()->GetCurrentScene();

	int sceneHeight = scene->GetScreenHeight();
	int sceneWidth = scene->GetScreenWidth();

	if (cx < 0) cx = 0;

	if ((cx + SCREEN_WIDTH) > sceneWidth) {
		cx = sceneWidth - SCREEN_WIDTH;
	}

	if ((cy + SCREEN_HEIGHT) > sceneHeight) {
		cy = sceneHeight - SCREEN_HEIGHT;
	}

	if (cy < 0) cy = 0;
	// end handle limit camera

	LPSCENE curentScene = CGame::GetInstance()->GetCurrentScene();
	int sceneID = curentScene->getCurrentID();
	//DebugOut(L"[INFO] sceneID: %d\n", sceneID);
	if (sceneID == 99) {
		CGame::GetInstance()->SetCamPos(0, 0);
	}
	else {
		CGame::GetInstance()->SetCamPos(cx, cy);
	}
	
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++) {
		// start phan hoach khong gian
		if (player == NULL) {
			return;
		}
		float pos_x = abs(player->x - objects[i]->x);
		float pos_y = abs(player->y - objects[i]->y);
		if (
			!dynamic_cast<CBackground*>(objects[i]) && 
			!dynamic_cast<CUI*>(objects[i]) &&
			pos_x > SCREEN_WIDTH &&
			pos_y > SCREEN_HEIGHT
		) 
		{
			continue;
		}
	
		// end phan hoach khong gian
		objects[i]->Render();
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CSophia *>(objects[i]))
		{
			CSophia *sophia = CSophia::GetInstance();
			sophia->~CSophia();
			continue;
		}
		if (dynamic_cast<CJason *>(objects[i]))
		{
			CJason *json = CJason::GetInstance();
			json->~CJason();
			continue;
		}
		if (dynamic_cast<CJasonOW*>(objects[i]))
		{
			CJasonOW* jsonOW = CJasonOW::GetInstance();
			jsonOW->~CJasonOW();
			continue;
		}
		//DebugOut(L"[INDEX] object index %d\n", i);
		delete objects[i];
	}

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
	CPlayer *player = ((CPlayScene *)scence)->GetPlayer();
	if (player == NULL) return;
	LPSCENE curentScene = CGame::GetInstance()->GetCurrentScene();
	int sceneID = curentScene->getCurrentID();

	if (sceneID == END_SCENE) return;

	if (player->isDie) return;

	switch (KeyCode)
	{
	case DIK_A:
		player->Reset();
		break;
	case DIK_UP:
		player->KeyUp();
		break;
	/*case DIK_DOWN:
		player->KeyDown();
		break;*/
	case DIK_LEFT:
		player->KeyLeft();
		break;
	case DIK_RIGHT:
		player->KeyRight();
		break;
	case DIK_Z:
		player->KeyZ();
		break;
	case DIK_X:
		player->KeyX();
		break;
	case DIK_LSHIFT:
		player->KeySHIFT();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	vector<LPGAMEOBJECT> objects = ((CPlayScene *)scence)->GetObjects();
	CPlayer *player = ((CPlayScene *)scence)->GetPlayer();
	if (player == NULL) return;
	LPSCENE curentScene = CGame::GetInstance()->GetCurrentScene();
	int sceneID = curentScene->getCurrentID();
	if (sceneID == 99) return;
	if (dynamic_cast<CJasonOW *>(player))
	{
		player->SetState(PLAYER_STATE_IDLE);
	}
	switch (KeyCode)
	{
	case DIK_UP:
		player->OnKeyUp();
	case DIK_LEFT:
		player->OnKeyUpLeft();
	case DIK_RIGHT:
		player->OnKeyUpRight();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	LPSCENE curentScene = CGame::GetInstance()->GetCurrentScene();
	int sceneID = curentScene->getCurrentID();
	if (sceneID == 99){
		return;
	}
	else {
		CGame* game = CGame::GetInstance();
		CPlayer* player = ((CPlayScene*)scence)->GetPlayer();
		if (player == NULL) return;

		if (player->isDie) return;
		if (game->IsKeyDown(DIK_RIGHT))
			player->KeyRight();
		else if (game->IsKeyDown(DIK_LEFT))
			player->KeyLeft();
		else if (game->IsKeyDown(DIK_UP))
			player->KeyUp();
		else if (game->IsKeyDown(DIK_DOWN))
			player->KeyDown();
		else {
			if (!player->isJump && !dynamic_cast<CJasonOW *>(player)) {
				player->SetState(PLAYER_STATE_IDLE);
			}
		}
	}
}
