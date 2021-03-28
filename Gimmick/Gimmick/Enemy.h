#pragma once

#include "GameObject.h"

#define STATE_ENEMY_NOT_EXIST	0
#define STATE_ENEMY_EXIST		1

#define ENEMY_GRAVITY			0.002f

class CEnemy : public  CGameObject
{
protected:
	bool isStop;
public:
	CEnemy(float _x = 0, float _y = 0)
	{
		this->x = _x;
		this->y = _y;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void Render() = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
};

