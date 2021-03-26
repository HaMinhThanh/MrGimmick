#include <map>
#include <iostream>
#include "PlayScene.h"
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6

#define MAX_SCENE_LINE 1024

#define OBJECT_TYPE_GIMMICK	1



#define SCENE_SECTION_MAP				7

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::ParseSection_Textures(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->AddTexture(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::ParseSection_Sprites(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

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

	CSprites::GetInstance()->AddSprite(ID, l, t, r, b, tex);
}

void CPlayScene::ParseSection_Animations(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->AddAnimation(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->AddAnimations(ani_id, ani);
}

void CPlayScene::ParseSection_Animation_Sets(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->AddAnimationSet(ani_set_id, s);
}

void CPlayScene::ParseSection_Objects(string line)
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
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		//obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_GIMMICK:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CGimmick(x, y);
		player = (CGimmick*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		//obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{

		{
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			
		}
	}
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath): CScene(id, filePath)
{
	keyHandler = new CPlayScenceKeyHandler(this);
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

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
		}
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
		case SCENE_SECTION_TEXTURES: ParseSection_Textures(line); break;
		case SCENE_SECTION_SPRITES: ParseSection_Sprites(line); break;
		case SCENE_SECTION_ANIMATIONS: ParseSection_Animations(line); break;
		case SCENE_SECTION_ANIMATION_SETS: ParseSection_Animation_Sets(line); break;
		case SCENE_SECTION_OBJECTS: ParseSection_Objects(line); break;
	
		}
	}

	f.close();

	//CTextures::GetInstance()->AddTexture(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
}

void CPlayScene::Render()
{
}

void CPlayScene::Unload()
{
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
}
