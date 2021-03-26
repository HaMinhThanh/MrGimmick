#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "KeyEventHandler.h"

class CScene
{
protected:
	int id;
	CKeyEventHandler* keyHandler;
	LPCWSTR sceneFilePath;

	D3DCOLOR  backColor;

public:

	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler* GetKeyEventHandler() { return keyHandler; }

	virtual void Update(DWORD dt) = 0;
	virtual void Load() = 0;
	virtual void Render() = 0;
	virtual void Unload() = 0;

	D3DCOLOR GetBackColor();

};

typedef CScene* LPSCENE;

class CScenceKeyHandler : public CKeyEventHandler
{
public:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};

