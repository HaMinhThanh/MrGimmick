#pragma once
#include "Enemy.h"
#include "GameObject.h"

#define BOMB_STATE_WALKING_RIGHT	100
#define BOMB_STATE_WALKING_LEFT		200
#define BOMB_STATE_FLY				300
#define BOMB_STATE_DIE				900

#define BOMB_GRAVITY				0.002f

#define BOMB_SPEED_X			0.01f
#define BOMB_SPEED_Y_NORMAL		0.01f
#define BOMB_SPEED_Y_COLLISION	0.03f

#define BOMB_BBOX_WIDTH		16
#define BOMB_BBOX_HEIGHT	16

class CBomb: public CGameObject
{
public:
	CBomb(float x, float y);
	~CBomb();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

