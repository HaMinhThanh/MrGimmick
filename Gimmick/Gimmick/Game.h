#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "Define.h"
#include "Scene.h"

#define DIRECTINOUT_VERSION 0X0800
#define KEYBOARD_BUFFER_SIZE 1024

using namespace std;

class CGame
{
	static CGame* _instance;
	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddv = NULL;

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;

	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 didv;

	BYTE keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int ScreenWidth;
	int ScreenHeight;

	unordered_map<int, LPSCENE> scenes;
	int currentScene;

	void ParseSection_Setting(string line);
	void ParseSection_Scenes(string line);

public:
	D3DCOLOR background;

	void InitKeyBoard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);

	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	void DrawFlipX(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texturePath);

	int IsKeyDown(int KeyCode);
	void ProcessKeyBoard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene();
	int GetCurrentSceneId() { return currentScene; }

	void SwitchScene(int scene_id);

	void SetScreenWidth(int width);
	void SetScreenHeight(int height);

	int GetScreenWidth();
	int GetScreenHeight();

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; };
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	/*int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }*/

	static void SweptAABB(
		float ml,
		float mt,
		float mr,
		float mb,
		float dx,
		float dy,
		float sl,
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	bool checkAABB(float b1left, float b1top, float b1right, float b1bottom, float b2left, float b2top, float b2right, float b2bottom);

	void SetCamPosition(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }

	static CGame* GetInstance();

	~CGame();
};

