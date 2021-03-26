#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprites* CSprites::_instance = NULL;
CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void CSprite::DrawFlipX(float x, float y, int alpha)
{
	LPD3DXSPRITE spriteHandler = CGame::GetInstance()->GetSpriteHandler();

	D3DXMATRIX oldMt;
	spriteHandler->GetTransform(&oldMt);

	D3DXMATRIX newMt;

	D3DXVECTOR2 top_left = D3DXVECTOR2(x, y);

	D3DXVECTOR2 rotate = D3DXVECTOR2(-1, 1);

	D3DXMatrixTransformation2D(&newMt, &top_left, 0.0f, &rotate, NULL, 0.0f, NULL);
	D3DXMATRIX finalMt = newMt * oldMt;

	spriteHandler->SetTransform(&finalMt);

	x -= getwidth();

	this->DrawCam(x, y, alpha);

	spriteHandler->SetTransform(&oldMt);
}

void CSprite::DrawCam(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->DrawFlipX(x, y, texture, left, top, right, bottom, alpha);
}

void CSprites::AddSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	if (sprites[id] != NULL)
		DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

CSprites* CSprites::GetInstance()
{
	if (_instance == NULL) _instance = new CSprites();
	return _instance;
}
