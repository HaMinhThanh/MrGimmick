#pragma once
#include "Enemy.h"

#define BOMB_BBOX_WIDTH		16
#define BOMB_BBOX_HEIGHT	16

class CBomb: public CEnemy
{
public:
	//CBomb(float x, float y);
	//~CBomb();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

