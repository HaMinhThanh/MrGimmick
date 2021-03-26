#pragma once

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include <map>
#include "Gimmick.h"
#include "Portal.h"

class CPlayScene: public CScene
{
public:
	std::map<int, bool> keys;
	CGimmick* player;

	void ParseSection_Textures(string line);
	void ParseSection_Sprites(string line);
	void ParseSection_Animations(string line);
	void ParseSection_Animation_Sets(string line);
	void ParseSection_Objects(string line);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CGimmick* GetPlayer() { return player; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

