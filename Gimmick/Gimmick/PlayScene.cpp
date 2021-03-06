#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Define.h"

#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

#include "Portal.h"
#include "Star.h"

#include "Brick.h"
#include "ScrollBar.h"
#include "Slide.h"

#include "Bomb.h"


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
#define SCENE_SECTION_MAP_TEXTURES	7
#define SCENE_SECTION_MAP	8
#define SCENE_SECTION_CAMERA 9

#define OBJECT_TYPE_GIMMICK	1
#define	OBJECT_TYPE_STAR 2

// Ground
#define OBJECT_TYPE_BRICK	4
#define OBJECT_TYPE_SLIDE	5
#define OBJECT_TYPE_SCROLLBAR	6

// Enemy
#define OBJECT_TYPE_BOMB	21

// Item
// Effect



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
	
	case OBJECT_TYPE_GIMMICK:

		obj = CGimmick::GetInstance(x, y);

		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		/*obj = new CGimmick(x, y);
		player = (CGimmick*)obj;*/

		player = CGimmick::GetInstance(x, y);

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case OBJECT_TYPE_BRICK: 
		obj = new CBrick(); 
		break;
	case OBJECT_TYPE_STAR:
		obj = new CStar();
		break;

	case OBJECT_TYPE_SLIDE:
	{
		float x2 = atof(tokens[4].c_str());
		float y2 = atof(tokens[5].c_str());
		int state = atoi(tokens[6].c_str());

		obj = new CSlide(x, y, x2, y2, state);
		break;
	}

	case OBJECT_TYPE_SCROLLBAR:
		break;

	case OBJECT_TYPE_BOMB:
		obj = new CBomb(x, y);
		break;

	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
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

void CPlayScene::_ParseSection_MAP_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int IDMap = atoi(tokens[0].c_str());
	int row = atoi(tokens[1].c_str());
	int column = atoi(tokens[2].c_str());
	int IDTileSet = atoi(tokens[3].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(IDTileSet);

	int index = IDMap;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			DebugOut(L"ok");
			CSprites::GetInstance()->Add(index, j * 32, i * 32, j * 32 + 32, i * 32 + 32, tex);
			index = index + 1;
		}
	}
}

void CPlayScene::_ParseSection_CAMERA(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2)return;	// skip invalid lines
	_xLeft = atoi(tokens[0].c_str());
	_xRight = atoi(tokens[1].c_str());
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines
	wstring filePath = ToWSTR(tokens[0]);
	int row = atoi(tokens[2].c_str());
	int column = atoi(tokens[1].c_str());
	int index = atoi(tokens[3].c_str());
	int align = atoi(tokens[4].c_str());
	map->SetValueInMap(row, column, index, align);
	map->LoadMap(filePath);
}

void CPlayScene::Load()
{

	_xLeft = _xRight = -1;
	map = CMap::GetInstance();

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
		if (line == "[MAP_TEXTURES]") {
			section = SCENE_SECTION_MAP_TEXTURES; continue;
		}
		if (line == "[MAP]"){
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[CAMERA]") {
			section = SCENE_SECTION_CAMERA; continue;
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
		case SCENE_SECTION_MAP_TEXTURES: _ParseSection_MAP_TEXTURES(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_CAMERA: _ParseSection_CAMERA(line); break;
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
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (cx < _xLeft) 
		cx = _xLeft;

	if (cx > _xRight - SCREEN_WIDTH + 16) // cong them 16 vi thieu 1 frame
		cx = _xRight - SCREEN_WIDTH + 16;

	CGame::GetInstance()->SetCamPos((int)cx, 200);
}

void CPlayScene::Render()
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	map->DrawMap(cx, cy);

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

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (gimmick->GetJumping() == 0)
		{
			gimmick->SetState(GIMMICK_STATE_JUMP);
			gimmick->SetJumping(1);

			if (gimmick->GetDoubleJumpStart() == 0)
			{
				//gimmick->SetState(MARIO_STATE_JUMP_HIGH_SPEED);
				gimmick->SetDoubleJumpStart();

			}
		}
		break;
	
	} 
	
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();
	float x, y;
	gimmick->GetPosition(x, y);
	switch (KeyCode)
	{
	case DIK_S:
		gimmick->ResetDoubleJumpStart();
		break;
	case DIK_A:
		gimmick->SetHoldStar(0);
		if (gimmick->GetShoot() == 0)
		{
			gimmick->SetShoot(1);
		}
		break;
	}
	
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 

	if (game->IsKeyDown(DIK_RIGHT)) {

		gimmick->SetState(GIMMICK_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {

		gimmick->SetState(GIMMICK_STATE_WALKING_LEFT);
	}	
	else if (gimmick->vy == 0 && gimmick->vx != 0) {

		gimmick->SetState(GIMMICK_STATE_IDLE);
	}

	if (game->IsKeyDown(DIK_A))
	{
		/*if (gimmick->GetHoldStar() == 0)
			gimmick->SetHoldStar(1);*/

		if (gimmick->loading == 1 ) {

			gimmick->SetLoadingStar();
			// /
		}

	}
	else {

		if (gimmick->loading == 2 && gimmick->isCanShot) {

			// //
		}
		else {

			gimmick->ReSetLoading();
		}
	}
}